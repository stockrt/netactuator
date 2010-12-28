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


// Verifica se um IP está na lista de whitelisteds ou não
int is_whitelisted (tipostring info)
{
	int i;


	for(i=0; i<num_whitelist; i++)
	{
		if (strcmp(info, whitelist[i]) == 0)
		{
			// Achou o IP na whitelist
			return 1;	// Eh whitelisted
		}
	}

	return 0;	// Nao eh whitelisted
}


// Avisa aos contatos administrativos sobre a ação tomada pelo netactuator
void avisar_contatos_adm (SearchTree comeca, long baseline)
{
	int i;
	FILE *mail_handler;
	tipostring mail_temp_file;
	tipostring comando;


	printf("Enviando e-mail...\n");

	// Se não for whitelisted
	if (!is_whitelisted(comeca->info))
	{
		strcpy(mail_temp_file, "/tmp/netactuator_mail_temp_file.txt");

		for(i=0; i<num_admin_contacts; i++)
		{
			if (mail_handler = rfopen(mail_temp_file, "wt"))
			{
				fprintf(mail_handler, "Host \"%s\" com \"%ld\" conversações como origem foi bloqueado pelo netactuator.\n", comeca->info, comeca->convs_as_source);
				fprintf(mail_handler, "Baseline: %ld\n", baseline);
				fprintf(mail_handler, "Limite: %.0f\n\n", baseline * threshold);
				fclose(mail_handler);
				printf("Mandando mail para %s\n", admin_contacts[i]);
				sprintf(comando, "mail -s 'netactuator - Alerta Host \"%s\"' %s < %s", comeca->info, admin_contacts[i], mail_temp_file);
//				system(comando);
			}
		}
	}
//	else
//	{
//		printf("Nada a ser feito, IP %s pertence à WhiteList\n", comeca->info);
//	}

	printf("\n");
	fflush(stdout);
}


// Aplica a regra de firewall ao host detectado como intruso
int bloquear_host (tipostring info, int expire)
{
	FILE *in_handler;
	tipostring comando;
	tipostring linha;
	int flag_bloqueado=0;


//	printf("Aplicando regra de firewall...\n");

	// Se não for whitelisted
	if (!is_whitelisted(info))
	{
		if (is_ipfw)
			sprintf(comando, "%s list | grep \"deny ip from %s to any\"", fire_bin, info);
		else if (is_iptables)
			sprintf(comando, "%s -L -n -t filter | grep %s | grep DROP", fire_bin, info);

		in_handler = rpopen(comando, "r");
		if (in_handler)
		{
			while (fgets(linha, MAX_TAM_LINHA, in_handler)) // Se tiver entradas para este IP em deny, não bloqueia novamente
				flag_bloqueado = 1;
			pclose(in_handler);
		}

		if (!flag_bloqueado)
		{
			if (is_ipfw)
				sprintf(comando, "%s add deny ip from %s to any", fire_bin, info);
			else if (is_iptables)
				sprintf(comando, "%s -I INPUT -t filter -s %s -j DROP", fire_bin, info);

			printf("Bloqueando host: %s\n", info);
//			system(comando);
			inserir_blacklist(info, expire);

			fflush(stdout);
			return 1;
		}
	}
//	else
//	{
//		printf("Nada a ser feito, IP %s pertence à WhiteList\n", info);
//	}

	return 0;
}


// Aplica a regra de liberação no firewall
void desbloquear_host (tipostring info)
{
	FILE *in_handler;
	tipostring comando;
	tipostring linha;
	int flag_bloqueado=0;


//	printf("Aplicando regra de liberação de firewall...\n");

	if (is_ipfw)
		sprintf(comando, "%s list | grep \"deny ip from %s to any\"", fire_bin, info);
	else if (is_iptables)
		sprintf(comando, "%s -L -n -t filter | grep %s | grep DROP", fire_bin, info);

	in_handler = rpopen(comando, "r");
	if (in_handler)
	{
		while (fgets(linha, MAX_TAM_LINHA, in_handler)) // Se tiver entradas para este IP em deny, não bloqueia novamente
			flag_bloqueado = 1;
		pclose(in_handler);
	}

	if (flag_bloqueado)
	{
		if (is_ipfw)
			sprintf(comando, "%s delete $(%s list | grep \"deny ip from %s to any\" | awk '{print $1}')", fire_bin, fire_bin, info);
		else if (is_iptables)
			sprintf(comando, "%s -D INPUT -t filter -s %s -j DROP", fire_bin, info);

//		printf("Liberando host: %s\n", info);
//		system(comando);
		fflush(stdout);
	}

	remover_blacklist(info);
	fflush(stdout);
}
