/*
 * This file is part of netactuator.
 *
 * netactuator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * netactuator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with netactuator.  If not, see <http://www.gnu.org/licenses/gpl.html>
 *
 */


// Checa se algum host ultrapassou os limites estabelecidos e atua
void checar_limites (SearchTree comeca)
{
	MYSQL_RES *res=NULL;
	MYSQL_ROW row;

	long avg_conv_host=0;
	long max_conv_host=0;
	int flag_prosseguir=0;

	tipostring query;
	graph_t graph;
	pthread_t thread;


	if (comeca)
	{
		checar_limites(comeca->dir);
		checar_limites(comeca->esq);

		// Verificar se já consta entrada em pattern_def, para poder prosseguir com a avaliação
		// Só terá entrada registrada em pattern_def caso já tenha atingido um valor mínimo de registros
		sprintf(query, "SELECT host FROM %s WHERE host = '%s';", PATTERN_T, comeca->info);
		res = sql(query, mysql_conn_global);

		if ((row = mysql_fetch_row(res)))
		{
			flag_prosseguir = 1;
		}
		mysql_free_result(res);

		// Se tiver cadastro de pattern, continua, senão insere como clean em mass
		if (flag_prosseguir)
		{
			// Pega valor definido no banco
			sprintf(query, "SELECT convs_as_source_avg,convs_as_source_max FROM %s WHERE host = '%s';", PATTERN_T, comeca->info);
			res = sql(query, mysql_conn_global);

			if ((row = mysql_fetch_row(res)))
			{
				if (row[0] != NULL)
					avg_conv_host = atol(row[0]);
				if (row[1] != NULL)
					max_conv_host = atol(row[1]);
			}
			mysql_free_result(res);

//printf("Host %s avg: %ld\n", comeca->info, avg_conv_host);
//printf("Host %s max: %ld\n", comeca->info, max_conv_host);

			// Somente executa se tiver algum valor registrado para o host como origem
			// Evita avaliar hosts que tenham entradas suficientes mas que sejam apenas como destino, sem valores de convs como origem, gerando valor nulo ou zero
			// Esta situação não vai ocorrer por conta do insert into com select convs_as_source > 0
			if (avg_conv_host > 0)
			{
				// Se for maior do que o limite especificado
				// DIRTY
				if (comeca->convs_as_source > (avg_conv_host * threshold))
				{
					sprintf(query, "INSERT INTO %s VALUES ('%s', '%ld', '%ld', '%ld', '%ld', '%ld', '%ld', '%s', '%s', '%s', '%d', '%d', '%s');",
						MASS_T, comeca->info, comeca->convs_as_source, comeca->convs_as_destin, comeca->recv_f, comeca->sent_f, comeca->recv_b, comeca->sent_b, data, hora, week_day, flow_capture_time_min, FLAG_DIRTY, comeca->iface);
					sql(query, mysql_conn_global);

/*
					printf("IP: %s ->", comeca->info);
					printf(" Recv Frames: %ld", comeca->recv_f);
					printf(" Recv Bytes: %ld", comeca->recv_b);
					printf(" Sent Frames: %ld", comeca-> sent_f);
					printf(" Sent Bytes: %ld", comeca->sent_b);
					printf(" Source: %ld", comeca->convs_as_source);
					printf(" Destino: %ld", comeca->convs_as_destin);
					printf("\n");
*/

					// Atuação
					if (bloquear_host(comeca->info, 1))
					{
						// Somente registra evento e avisa por mail, caso ainda não esteja bloqueado
						registrar_evento(comeca->info, comeca->convs_as_source, avg_conv_host, "Bloqueado");
						avisar_contatos_adm(comeca, avg_conv_host);
					}
				}
				// CLEAN
				else
				{
					sprintf(query, "INSERT INTO %s VALUES ('%s', '%ld', '%ld', '%ld', '%ld', '%ld', '%ld', '%s', '%s', '%s', '%d', '%d', '%s');",
						MASS_T, comeca->info, comeca->convs_as_source, comeca->convs_as_destin, comeca->recv_f, comeca->sent_f, comeca->recv_b, comeca->sent_b, data, hora, week_day, flow_capture_time_min, FLAG_CLEAN, comeca->iface);
					sql(query, mysql_conn_global);
				}

/*
// Desativado temporariamente
				if (comeca->convs_as_destin > max_convs_as_destin)
				{
					printf("IP: %s ->", comeca->info);
					printf(" Recv Frames: %ld", comeca->recv_f);
					printf(" Recv Bytes: %ld", comeca->recv_b);
					printf(" Sent Frames: %ld", comeca-> sent_f);
					printf(" Sent Bytes: %ld", comeca->sent_b);
					printf(" Source: %ld", comeca->convs_as_source);
					printf(" Destino: %ld", comeca->convs_as_destin);
					printf("\n");
				}
*/
			} // if avg_conv_host > 0
		} // if flag_prosseguir (cadastrado em pattern)
		else
		{
			// Este seria o caso das primeiras inserções do host no banco, insere clean.
				sprintf(query, "INSERT INTO %s VALUES ('%s', '%ld', '%ld', '%ld', '%ld', '%ld', '%ld', '%s', '%s', '%s', '%d', '%d', '%s');",
					MASS_T, comeca->info, comeca->convs_as_source, comeca->convs_as_destin, comeca->recv_f, comeca->sent_f, comeca->recv_b, comeca->sent_b, data, hora, week_day, flow_capture_time_min, FLAG_CLEAN, comeca->iface);
				sql(query, mysql_conn_global);
		} // else flag_prosseguir (cadastrado em pattern)


		// Cria gráficos com RRDTools para qualquer IP capturado que esteja cadastrado em NETWORKS_T
		strcpy(graph.info, comeca->info);
		graph.recv_f = comeca->recv_f;
		graph.sent_f = comeca->sent_f;
		graph.recv_b = comeca->recv_b;
		graph.sent_b = comeca->sent_b;
		graph.convs_as_source = comeca->convs_as_source;
		graph.convs_as_destin = comeca->convs_as_destin;
		strcpy(graph.iface, comeca->iface);
		graph.baseline = avg_conv_host;

//		flag_copia_local_ok = 0;
//		pthread_create(&thread, NULL, (void *) &rrd_rog, (graph_t *) &graph);
//		// Aguarda a cópia local ser concluída pela thread para prosseguir. Aqui existe a perda de contexto da struct, no 
//		// retorno da função, por isso a flag (wait_lock).
		rrd_rog(&graph);
//		while (!flag_copia_local_ok)
//			;
	} // if (comeca)
}
