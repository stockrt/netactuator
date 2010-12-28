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


// Remove os espacos em branco do comeco e do fim da string
// e os \n do fim da string
void trim (char *inputstring)
{
	int space_inicial=0;
	int space_final=0;
	tipostring str_aux;


	// Remove os spaces do inicio
	while (inputstring[space_inicial] == ' ')
		space_inicial++;
	strcpy(str_aux, &inputstring[space_inicial]);
	strcpy(inputstring, str_aux);


	// Remove os spaces do final
	space_final = strlen(inputstring) - 1;
	while (inputstring[space_final] == ' ' || inputstring[space_final] == '\n')
	{
		inputstring[space_final] = '\0';
		space_final--;
	}
}


// Remove o comentario de uma linha antes de avalia-la
void remover_comentarios (char *inputstring)
{
	char *ponto;


	ponto = strstr(inputstring, "#");

	if (ponto)
	{
		*ponto = '\0';
		inputstring = ponto;
	}
}


// Separa uma string em argumentos com o separador regexp ","
int sep_arg_vet (char *arg[MAX_ARGS], char *inputstring, tipostring regexp)
{
	int num_arg=0, i=0;
	char **ap;


	// Separa argumentos em um vetor
	for (ap = arg; (*ap = strsep(&inputstring, regexp)) != NULL;)
		if (**ap != '\0')
		{
			if (++ap >= &arg[MAX_ARGS])
				break;
			num_arg += 1;
		}

	for(i=0; i<num_arg; i++)
		trim(arg[i]);

	return num_arg;
}


// Mostra as configurações carregadas na tela
void mostrar_configuracoes (void)
{
	int i=0;


	printf("\n\n");
	printf("DB settings:\n");
	printf("Host: %s\n", db_host);
	printf("User: %s\n", db_user);
	printf("Password: %s\n", db_password);
	printf("DB_Name: %s\n", db_name);

	printf("\n");
	printf("Interfaces de Rede: %d\n", num_interfaces);
	for(i=0; i<num_interfaces; i++)
		printf("%s\n", interfaces[i]);

	printf("\n");
	printf("Networks: %d\n\n", num_networks);
	for(i=0; i<num_networks; i++)
	{
		printf("Network/Mask %s/", my_ntoa(networks_ranges[i][0] - 1));
//		printf("%s\n", networks[i][1]);		// mask
		printf("%d\n", networks_ranges[i][2]);	// mask bits

//		printf("Network      %s\n", my_ntoa(networks_ranges[i][0] - 1));
//		printf("Netmask      %s\n", networks[i][1]);
//		printf("Netmask bits %d\n", networks_ranges[i][2]);
//		printf("Broadcast    %s\n", my_ntoa(networks_ranges[i][1] + 1));
//		printf("Hosts        %s", my_ntoa(networks_ranges[i][0]));
//		printf(" to %s\n", my_ntoa(networks_ranges[i][1]));
//		printf("Host count   %d\n", networks_ranges[i][1] - networks_ranges[i][0] + 1);
//		printf("\n");
	}

	printf("\n");
	printf("Flow Capture Time: %d minuto%s\n", flow_capture_time_min, flow_capture_time_min > 1 ? "s" : "");

	printf("\n");
	printf("Admin Contacts: %d\n", num_admin_contacts);
	for(i=0; i<num_admin_contacts; i++)
		printf("%s\n", admin_contacts[i]);

	printf("\n");
	printf("Blacklist: %d\n", num_blacklist);
	for(i=0; i<num_blacklist; i++)
		printf("%s\n", blacklist[i]);

	printf("\n");
	printf("Whitelist: %d\n", num_whitelist);
	for(i=0; i<num_whitelist; i++)
		printf("%s\n", whitelist[i]);

	printf("\n");
	printf("pmacctd Binary: %s\n", pmacctd_bin);
	printf("pmacct Binary: %s\n", pmacct_bin);

	printf("\n");
	printf("Firewall Binary: %s\n", fire_bin);

	printf("\n");
	printf("Top Users: %d first\n", top_users);

	printf("\n");
	printf("Base www: %s\n", base_www);

	printf("\n");
	printf("Block Time: %d minuto%s\n", block_time_min, block_time_min > 1 ? "s" : "");

	printf("\n");
	printf("Days to Keep: %d dia%s\n", days_to_keep, days_to_keep > 1 ? "s" : "");

	printf("\n");
	printf("Days to Learn: %d dia%s\n", days_to_learn, days_to_learn > 1 ? "s" : "");

	printf("\n");
	printf("Threshold: %.2f(fator) | %.0f%%\n", threshold, (threshold - 1) * 100);

	printf("\n\n");

	fflush(stdout);
}


// Carrega as configurações do arquivo para as variáveis globais
//void carregar_configuracoes (FILE *in_handler)
void carregar_configuracoes (tipostring arquivo)
{
	MYSQL *conn=NULL;
	MYSQL_RES *res=NULL;
	MYSQL_ROW row;

	tipostring query;

	int i=0, num_arg=0;
	tipostring linha, tag;
	char *argum[MAX_ARGS];
	char *net_argum_temp[MAX_ARGS];


	FILE *in_handler;
	if (in_handler = rfopen(arquivo, "rt"))
	{
	// Ler linha a linha em um buffer inicial
	while (fgets(linha, MAX_TAM_LINHA, in_handler))
	{
		remover_comentarios(linha);
		// Separa as entradas em tag e valores
		num_arg = sep_arg_vet(argum, linha, "=");

		// Se achou uma tag com um valor
		if (num_arg == 2)
		{
			// Database
			if (!strcmp(argum[0], "db_host"))
				strcpy(db_host, argum[1]);
			else if (!strcmp(argum[0], "db_user"))
				strcpy(db_user, argum[1]);
			else if (!strcmp(argum[0], "db_password"))
				strcpy(db_password, argum[1]);
			else if (!strcmp(argum[0], "db_name"))
				strcpy(db_name, argum[1]);
		}
	}

	fclose(in_handler); // aberto em main
	} // if in_handler


//	while (1)
//	{
		flag_loading_config = 1; // carregando configurações, bloqueia outros acessos

		if ((conn = conectar()))
		{
			num_interfaces = 0;
			sprintf(query, "SELECT interface FROM %s;", INTERFACES_T);
			res = sql(query, conn);
			while ((row = mysql_fetch_row(res)))
			{
				strcpy(interfaces[num_interfaces], row[0]);
				num_interfaces++;
			}


			num_networks = 0;
			sprintf(query, "SELECT network FROM %s;", NETWORKS_T);
			res = sql(query, conn);
			while ((row = mysql_fetch_row(res)))
			{
				// Rede/Máscara
				sep_arg_vet(net_argum_temp, row[0], "/");
				strcpy(networks[num_networks][0], net_argum_temp[0]);
				strcpy(networks[num_networks][1], net_argum_temp[1]);
				num_networks++;
			}
			// Carrega os valores de ip_range para networks_ranges usando a função netmask() de Paul Davis, modificada por mim.
			if (num_networks)
				netmask_paul_davis();


			sprintf(query, "SELECT flow_capture_time_min,pmacctd_bin,pmacct_bin,fire_bin,top_users,base_www,block_time_min,days_to_keep,days_to_learn,threshold FROM %s;", netactuator_T);
			res = sql(query, conn);
			while ((row = mysql_fetch_row(res)))
			{
				flow_capture_time_min = atoi(row[0]);
				if (flow_capture_time_min < 1 || flow_capture_time_min > 10)
				{
					printf("Valor de flow_capture_time=%d está fora do intervalo permitido\n", flow_capture_time_min);
				}
				flow_capture_time_sec = flow_capture_time_min * 60; // transforma os minutos em segundos

				strcpy(pmacctd_bin, row[1]);
				strcpy(pmacct_bin, row[2]);
				strcpy(fire_bin, row[3]);
				top_users = atoi(row[4]);
				strcpy(base_www, row[5]);
				block_time_min = atoi(row[6]);
				days_to_keep = atoi(row[7]);
				days_to_learn = atoi(row[8]);
				threshold = 1 + (atof(row[9]) / 100);
			}


			num_admin_contacts = 0;
			sprintf(query, "SELECT email FROM %s;", ADMIN_T);
			res = sql(query, conn);
			while ((row = mysql_fetch_row(res)))
			{
				strcpy(admin_contacts[num_admin_contacts], row[0]);
				num_admin_contacts++;
			}


			num_whitelist = 0;
			sprintf(query, "SELECT host FROM %s;", WHITELIST_T);
			res = sql(query, conn);
			while ((row = mysql_fetch_row(res)))
			{
				strcpy(whitelist[num_whitelist], row[0]);
				num_whitelist++;
			}


			// Determina o firewall a usar
			if (strstr(fire_bin, "ipfw"))
			{
				is_ipfw = 1;
				is_iptables = 0;
			}
			if (strstr(fire_bin, "iptables"))
			{
				is_iptables = 1;
				is_ipfw = 0;
			}

			desconectar(conn);
		} // if conn

		// Sinaliza ao main que já pode prosseguir (banco ok, configs ok)
		flag_loading_config = 0; // configurações carregadas, libera outros acessos
		//mostrar_configuracoes();
//		sleep(DELAY_LEITURA_CONFIGS);
//	} // while not the end of the world
}
