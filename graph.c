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


// Cria a base RRD
void rrd_create_rog (tipostring arquivo)
{
	tipostring step;
	tipostring ds_convs_as_source;
	tipostring ds_convs_as_destin;
	tipostring ds_frames_in;
	tipostring ds_frames_out;
	tipostring ds_bytes_in;
	tipostring ds_bytes_out;
	tipostring ds_baseline;
	tipostring ds_threshold;
	tipostring rra_day_avg;
	tipostring rra_week_avg;
	tipostring rra_month_avg;
	tipostring rra_year_avg;
	tipostring rra_week_max;
	tipostring rra_month_max;
	tipostring rra_year_max;
	int heartbeat;
	int day_steps;
	int week_steps;
	int month_steps;
	int year_steps;
	int day_rows;
	int week_rows;
	int month_rows;
	int year_rows;
	float fator=2.0;	// Número de dias, semanas, meses e anos completos a manter


	sprintf(step, "-s %d", flow_capture_time_sec);
	heartbeat = flow_capture_time_sec * 2;			// HB é maior do que step

	day_steps = 1;						// Resolução máxima diária (valor exato a cada step segundos)
	week_steps = (30.0 / flow_capture_time_sec * 60);	// Resolução de 30 minutos (média de 30 em 30 minutos, na consolidação)
	month_steps = (120.0 / flow_capture_time_sec * 60);	// Resolução de 2 horas (média de 2 em 2 horas, na consolidação)
	year_steps = (360.0 / flow_capture_time_sec * 60);	// Resolução de 6 horas (média de 6 em 6 horas, na consolidação)

	day_rows = (fator * 86400 / flow_capture_time_sec);	// num rows para step segundos
	week_rows = (fator * 100800 / flow_capture_time_sec);	// num rows para 30 minutos
	month_rows = (fator * 111600 / flow_capture_time_sec);	// num rows para 2 horas
	year_rows = (fator * 439200 / flow_capture_time_sec);	// num rows para 6 horas

	if (day_steps <= 0)
		day_steps = 1;
	if (week_steps <= 0)
		week_steps = 1;
	if (month_steps <= 0)
		month_steps = 1;
	if (year_steps <= 0)
		year_steps = 1;

	if (day_rows <= 0)
		day_rows = 1;
	if (week_rows <= 0)
		week_rows = 1;
	if (month_rows <= 0)
		month_rows = 1;
	if (year_rows <= 0)
		year_rows = 1;

	sprintf(ds_convs_as_source, "DS:convs_as_source:GAUGE:%d:0:U", heartbeat);
	sprintf(ds_convs_as_destin, "DS:convs_as_destin:GAUGE:%d:0:U", heartbeat);

	sprintf(ds_frames_in, "DS:frames_in:ABSOLUTE:%d:0:U", heartbeat);
	sprintf(ds_frames_out, "DS:frames_out:ABSOLUTE:%d:0:U", heartbeat);

	sprintf(ds_bytes_in, "DS:bytes_in:ABSOLUTE:%d:0:U", heartbeat);
	sprintf(ds_bytes_out, "DS:bytes_out:ABSOLUTE:%d:0:U", heartbeat);

	sprintf(ds_baseline, "DS:baseline:GAUGE:%d:0:U", heartbeat);
	sprintf(ds_threshold, "DS:threshold:GAUGE:%d:0:U", heartbeat);

	sprintf(rra_day_avg, "RRA:AVERAGE:0.5:%d:%d", day_steps, day_rows);
	sprintf(rra_week_avg, "RRA:AVERAGE:0.5:%d:%d", week_steps, week_rows);
	sprintf(rra_month_avg, "RRA:AVERAGE:0.5:%d:%d", month_steps, month_rows);
	sprintf(rra_year_avg, "RRA:AVERAGE:0.5:%d:%d", year_steps, year_rows);

	sprintf(rra_week_max, "RRA:MAX:0.5:%d:%d", week_steps, week_rows);
	sprintf(rra_month_max, "RRA:MAX:0.5:%d:%d", month_steps, month_rows);
	sprintf(rra_year_max, "RRA:MAX:0.5:%d:%d", year_steps, year_rows);

/*
printf("%s\n", ds_convs_as_source);
printf("%s\n", ds_convs_as_destin);
printf("%s\n", ds_frames_in);
printf("%s\n", ds_frames_out);
printf("%s\n", ds_bytes_in);
printf("%s\n", ds_bytes_out);
printf("%s\n", ds_baseline);
printf("%s\n", ds_threshold);
printf("%s\n", rra_day_avg);
printf("%s\n", rra_week_avg);
printf("%s\n", rra_month_avg);
printf("%s\n", rra_year_avg);
*/

	char *params[] = {
		"create",
		arquivo,
		step,
		ds_convs_as_source,
		ds_convs_as_destin,
		ds_frames_in,
		ds_frames_out,
		ds_bytes_in,
		ds_bytes_out,
		ds_baseline,
		ds_threshold,
		rra_day_avg,
		rra_week_avg,
		rra_month_avg,
		rra_year_avg,
		rra_week_max,
		rra_month_max,
		rra_year_max,
		NULL
	};

	optind = opterr = 0; /* Because rrdtool uses getopt() */
	rrd_clear_error();
	rrd_create(18, params);

/*
Com step de 300 (5 minutos) temos:
72 = a cada seis horas, o mrtg usa a cada 24 para year (288)
24 = a cada duas horas, para mes, mesmo do mrtg
6 = a cada meia hora, para semana, mesmo do mrtg
1 = a cada cinco minutos, resolucao maxima, usado no diario

Com step de 60 (1 minuto) temos:
72 = a cada 1.2 horas, para year
24 = a cada 24 minutos, para mes
6 = a cada 6 minutos, para semana
1 = a cada minuto, resolucao maxima, usado no diario
*/
}


// Alimenta a base RRD
void rrd_update_rog (tipostring arquivo, long convs_as_source, long convs_as_destin, long frames_in, long frames_out, long bytes_in, long bytes_out, long baseline)
{
	tipostring dados;


//	sprintf(dados, "N:%ld:%ld:%ld:%ld:%ld:%ld:%ld:%0.f", convs_as_source, convs_as_destin, frames_in, frames_out, bytes_in, bytes_out, baseline, baseline * threshold);
	sprintf(dados, "%ld:%ld:%ld:%ld:%ld:%ld:%ld:%ld:%0.f",
		data_hora_global, convs_as_source, convs_as_destin, frames_in, frames_out, bytes_in, bytes_out, baseline, baseline * threshold);

	char *params[] = {
		"update",
		arquivo,
		dados,
		NULL
	};

printf("update: %s %s\n", arquivo, dados);
	optind = opterr = 0; /* Because rrdtool uses getopt() */
	rrd_clear_error();
	rrd_update(3, params);
}


// Gera os gráficos para o ip passado
//void rrd_graph_rog (int cod, tipostring host, long convs_as_source, long baseline)
void rrd_graph_rog (int tip, int cod, tipostring host)
{
	tipostring arquivo;
	tipostring arquivo_png;
	tipostring tittle;
	tipostring start;
	tipostring x_axis;
	tipostring def1;
	tipostring def2;
	tipostring def3;
	tipostring rrdmtime_comment;
	char **calcpr=NULL;
	int rrdargcount, xsize, ysize, result;
	double ymin, ymax;
	struct stat sb;


	// rrd a usar
	sprintf(arquivo, "%s/graph/%s_%d.rrd", base_www, host, flow_capture_time_sec);
	if (stat(arquivo, &sb) == -1) {
		perror("stat");
	}
	else {
		strftime(rrdmtime_comment, sizeof(rrdmtime_comment), "COMMENT:Ultima leitura\\:\t\t\t%a, %d %b %Y %H\\:%M\\:%S %Z", localtime(&sb.st_mtime));
	}

	if (tip == 0)
	{
		sprintf(def1, "DEF:convs_source=%s:convs_as_source:AVERAGE", arquivo);
		sprintf(def2, "DEF:baseline=%s:baseline:AVERAGE", arquivo);
		sprintf(def3, "DEF:threshold=%s:threshold:AVERAGE", arquivo);

		// Day
		if (cod == 0)
		{
			sprintf(arquivo_png, "%s/graph/%s_%d_base_day.png", base_www, host, flow_capture_time_sec);
			sprintf(tittle, "Diario - %s", host);
			strcpy(start, "-1day12hours");
			strcpy(x_axis, "HOUR:1:HOUR:1:HOUR:2:0:%H");
		}
		// Week
		if (cod == 1)
		{
			sprintf(arquivo_png, "%s/graph/%s_%d_base_week.png", base_www, host, flow_capture_time_sec);
			sprintf(tittle, "Semanal - %s", host);
			strcpy(start, "-1week2days");
			strcpy(x_axis, "HOUR:8:DAY:1:DAY:1:86400:%a");
		}
		// Month
		if (cod == 2)
		{
			sprintf(arquivo_png, "%s/graph/%s_%d_base_month.png", base_www, host, flow_capture_time_sec);
			sprintf(tittle, "Mensal - %s", host);
			strcpy(start, "-1month1week");
			strcpy(x_axis, "DAY:1:WEEK:1:WEEK:1:604800:Semana %W");
		}
		// Year
		if (cod == 3)
		{
			sprintf(arquivo_png, "%s/graph/%s_%d_base_year.png", base_www, host, flow_capture_time_sec);
			sprintf(tittle, "Anual - %s", host);
			strcpy(start, "-1year2months");
			strcpy(x_axis, "WEEK:1:MONTH:1:MONTH:1:2678400:%b");
		}

/*
printf("rrd: %s\n", arquivo);
printf("png: %s\n", arquivo_png);
printf("tittle: %s\n", tittle);
printf("start: %s\n\n", start);
fflush(stdout);
*/

		char *params[] = {
			"graph",
			arquivo_png,
			"-t", tittle,
			"-s", start,
			"-x", x_axis,
			"-v", "Convs as Source",
			"-z",
			"-a", "PNG",
			"-w", "400",
			"-c", "SHADEA#000000",
			"-c", "SHADEB#000000",
			"-c", "MGRID#999999",
			"-c", "FRAME#000000",
			"-c", "AXIS#000000",
			"-E",
			"-i",
			def1,
			def2,
			def3,
			"CDEF:acima_mark=convs_source,threshold,GT,threshold,0,IF",
			"CDEF:acima_print=convs_source,threshold,GT,convs_source,threshold,-,0,IF",
			"VDEF:base_fill=baseline,LAST",
			"CDEF:baseline_fill=baseline,UN,base_fill,baseline,IF",
			"VDEF:thres_fill=threshold,LAST",
			"CDEF:threshold_fill=threshold,UN,thres_fill,threshold,IF",
			"CDEF:normal=convs_source,threshold,LT,convs_source,0,IF",
			"CDEF:comprometido=convs_source,threshold,GT,convs_source,0,IF",
			"COMMENT:		   Ultimo   Maximo   Media\\n",
			"AREA:convs_source#00FF00:Normal	",
			"LINE0.4:convs_source#000000",
			"GPRINT:normal:LAST:%6.0lf",
			"GPRINT:normal:MAX:%6.0lf",
			"GPRINT:normal:AVERAGE:%6.0lf\\n",
			"AREA:acima_mark",
			"AREA:acima_print#FF0000:Comprometido	:STACK",
			"LINE0.4:acima_print#000000",
			"GPRINT:comprometido:LAST:%6.0lf",
			"GPRINT:comprometido:MAX:%6.0lf",
			"GPRINT:comprometido:AVERAGE:%6.0lf\\n",
			"LINE2:baseline_fill#5500FF",
			"LINE2:baseline#5500FF:Baseline	",
			"GPRINT:baseline:LAST:%6.0lf",
			"GPRINT:baseline:MAX:%6.0lf",
			"GPRINT:baseline:AVERAGE:%6.0lf\\n",
			"LINE2:threshold_fill#FF00FF",
			"LINE2:threshold#FF00FF:Limite	",
			"GPRINT:threshold:LAST:%6.0lf",
			"GPRINT:threshold:MAX:%6.0lf",
			"GPRINT:threshold:AVERAGE:%6.0lf\\n",
			"COMMENT:\\n",
			rrdmtime_comment,
			NULL
		};
//			"VDEF:leitura_ativ_last=convs_source,LAST" - Hora/Data da última atividade
//			"GPRINT:leitura_ativ_last:%c",

		optind = opterr = 0; /* Because rrdtool uses getopt() */
		rrd_clear_error();
		result = rrd_graph(62, params, &calcpr, &xsize, &ysize, NULL, &ymin, &ymax);
	} // tip 0
	else if (tip == 1)
	{
		sprintf(def1, "DEF:bytes_in=%s:bytes_in:AVERAGE", arquivo);
		sprintf(def2, "DEF:bytes_out=%s:bytes_out:AVERAGE", arquivo);

		// Day
		if (cod == 0)
		{
			sprintf(arquivo_png, "%s/graph/%s_%d_traf_day.png", base_www, host, flow_capture_time_sec);
			sprintf(tittle, "Diario - %s", host);
			strcpy(start, "-1day12hours");
			strcpy(x_axis, "HOUR:1:HOUR:1:HOUR:2:0:%H");
		}
		// Week
		if (cod == 1)
		{
			sprintf(arquivo_png, "%s/graph/%s_%d_traf_week.png", base_www, host, flow_capture_time_sec);
			sprintf(tittle, "Semanal - %s", host);
			strcpy(start, "-1week2days");
			strcpy(x_axis, "HOUR:8:DAY:1:DAY:1:86400:%a");
		}
		// Month
		if (cod == 2)
		{
			sprintf(arquivo_png, "%s/graph/%s_%d_traf_month.png", base_www, host, flow_capture_time_sec);
			sprintf(tittle, "Mensal - %s", host);
			strcpy(start, "-1month1week");
			strcpy(x_axis, "DAY:1:WEEK:1:WEEK:1:604800:Semana %W");
		}
		// Year
		if (cod == 3)
		{
			sprintf(arquivo_png, "%s/graph/%s_%d_traf_year.png", base_www, host, flow_capture_time_sec);
			sprintf(tittle, "Anual - %s", host);
			strcpy(start, "-1year2months");
			strcpy(x_axis, "WEEK:1:MONTH:1:MONTH:1:2678400:%b");
		}

/*
printf("rrd: %s\n", arquivo);
printf("png: %s\n", arquivo_png);
printf("tittle: %s\n", tittle);
printf("start: %s\n\n", start);
fflush(stdout);
*/

		char *params[] = {
			"graph",
			arquivo_png,
			"-t", tittle,
			"-s", start,
			"-x", x_axis,
			"-v", "Trafego",
			"-z",
			"-a", "PNG",
			"-w", "400",
			"-c", "SHADEA#000000",
			"-c", "SHADEB#000000",
			"-c", "MGRID#999999",
			"-c", "FRAME#000000",
			"-c", "AXIS#000000",
			"-E",
			"-i",
			def1,
			def2,
			"CDEF:bits_in=bytes_in,8,*",
			"CDEF:bits_out=bytes_out,8,*",
			"CDEF:kbits_in=bytes_in,8,*,1000,/",
			"CDEF:kbits_out=bytes_out,8,*,1000,/",
			"COMMENT:		    Ultimo   Maximo   Media     Total\\n",
			"AREA:bits_in#00FF00:Download (kbps)",
			"LINE0.4:bits_in#000000",
			"GPRINT:kbits_in:LAST:%6.0lf",
			"GPRINT:kbits_in:MAX:%6.0lf",
			"GPRINT:kbits_in:AVERAGE:%6.0lf",
			"CDEF:megas_in=bytes_in,1048576,/",
			"VDEF:total_in=megas_in,TOTAL",
			"GPRINT:total_in:%9.1lf (MB)\\n",
			"LINE1:bits_out#5500FF:Upload (kbps)  ",
			"GPRINT:kbits_out:LAST:%6.0lf",
			"GPRINT:kbits_out:MAX:%6.0lf",
			"GPRINT:kbits_out:AVERAGE:%6.0lf",
			"CDEF:megas_out=bytes_out,1048576,/",
			"VDEF:total_out=megas_out,TOTAL",
			"GPRINT:total_out:%9.1lf (MB)\\n",
			"COMMENT:\\n",
			rrdmtime_comment,
			NULL
		};
//			"VDEF:leitura_ativ_last=bytes_in,LAST" - Hora/Data da última atividade
//			Imprimir AREA e LINE com bits para mostrar o "k" no gráfico
//			Imprimir legendas em kbits para mostrar um valor mais inteligivel
//			"GPRINT:leitura_ativ_last:%c",

		optind = opterr = 0; /* Because rrdtool uses getopt() */
		rrd_clear_error();
		result = rrd_graph(51, params, &calcpr, &xsize, &ysize, NULL, &ymin, &ymax);
	} // tip 1

	// Liberar a memória
	if (calcpr)
	{
		int i;
		for (i=0; (calcpr[i]); i++)
			free(calcpr[i]);
		calcpr = NULL;
	}

	/* Was it OK ? */
	if (rrd_test_error() || (result != 0))
		// lazzy updates generates blank error, so we ignore it
		if (strcmp(rrd_get_error(), "") != 0)
			printf("Graph error: %s\n", rrd_get_error());
}


// Gerencia os arquivos rrd
void rrd_rog (graph_t *graph)
{
	tipostring arquivo;
	graph_t graph_local;


	// Copia os valores um a um para uma estrutura local
	strcpy(graph_local.info, graph->info);
	graph_local.recv_f = graph->recv_f;
	graph_local.sent_f = graph->sent_f;
	graph_local.recv_b = graph->recv_b;
	graph_local.sent_b = graph->sent_b;
	graph_local.convs_as_source = graph->convs_as_source;
	graph_local.convs_as_destin = graph->convs_as_destin;
	strcpy(graph_local.iface, graph->iface);
	graph_local.baseline = graph->baseline;

//printf("HOST %s -> %ld:%ld:%0.f\n", graph_local.info, graph_local.convs_as_source, graph_local.baseline, graph_local.baseline * threshold);
	sprintf(arquivo, "%s/graph/%s_%d.rrd", base_www, graph_local.info, flow_capture_time_sec);
	if (access(arquivo, F_OK)) // Se o arquivo .rrd não existir, cria.
		rrd_create_rog(arquivo);

	rrd_update_rog(arquivo, graph_local.convs_as_source, graph_local.convs_as_destin, graph_local.recv_f, graph_local.sent_f, graph_local.recv_b, graph_local.sent_b, graph_local.baseline);
}


// Gera os gráficos em thread, a cada intervalo regular de tempo
void graphing (void)
{
	MYSQL *conn=NULL;
	MYSQL_RES *res=NULL;
	MYSQL_ROW row;

	tipostring query;
	tipostring host;
	time_t data_hora;
	time_t data_hora_last=0;	// hora da última atualização


	while (1)
	{
		// Captura a data atual
		time(&data_hora);

		// 6 - ATUALIZAÇÃO DOS GRÁFICOS
		// Se a hora atual for maior que a anterior somada ao delay escolhido, update nos graphs
		if (data_hora > data_hora_last + (graph_update_time_min * 60))
		{
			// Marca data da última atualização
			// marcar logo no começo para que independentemente do tempo que levar para gerar os gráficos o software
			// não leve mais do que graph_update_time_min entre um start e outro do graphing, exceto, é claro, quando o 
			// tempo de graphing for maior do que o tempo de graph_update_time_min.
			time(&data_hora_last);
inicial();
			if ((conn = conectar()))
			{
				// Somente gera gráficos dos hosts que tiveram atividade dentro das últimas 3 execuções de coleta
				sprintf(query, "SELECT host from (SELECT host,hora FROM %s WHERE data = '%s' GROUP BY host HAVING TIME_TO_SEC(TIMEDIFF(CURTIME(), MAX(hora))) <= %d AND TIME_TO_SEC(TIMEDIFF(CURTIME(), MAX(hora))) >= 0) as A;", MASS_T, data, flow_capture_time_sec * 3);
//printf("Graph query: %s", query);

				res = sql(query, conn);

				while ((row = mysql_fetch_row(res)))
				{
					while (flag_limit_update) // enquanto estiver fazendo updates, dentro da checagem de limites, não gera gráficos
					{
						usleep(DELAY_MAIN);
//printf("aguardando limit terminar..\n");
//fflush(stdout);
					}

					strcpy(host, row[0]);
printf("graphing for %s\n", host);

// VER AQUI - talvez verificar se flag_limit_update estah 1 antes de CADA chamada ao graph

					// BASE
					rrd_graph_rog(0, 0, host); // day
					rrd_graph_rog(0, 1, host); // week
					rrd_graph_rog(0, 2, host); // month
					rrd_graph_rog(0, 3, host); // year

					// TRAF
					rrd_graph_rog(1, 0, host); // day
					rrd_graph_rog(1, 1, host); // week
					rrd_graph_rog(1, 2, host); // month
					rrd_graph_rog(1, 3, host); // year

					fflush(stdout);
				}

				desconectar(conn);
			} // if conn
final("GRAPHING");
		}

		sleep(DELAY_UPDATE);
	} // while not the end of the world
}
