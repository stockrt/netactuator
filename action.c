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


// Checks if an IP is on the whitelist or not
int is_whitelisted (tipostring info)
{
    int i;


    for(i=0; i<num_whitelist; i++)
    {
        if (strcmp(info, whitelist[i]) == 0)
        {
            // Found in the IP whitelist
            return 1;    // Whitelisted
        }
    }

    return 0;    // Not whitelisted
}


// Advises the administrative contacts on the action taken by netactuator
void avisar_contatos_adm (SearchTree comeca, long baseline)
{
    int i;
    FILE *mail_handler;
    tipostring mail_temp_file;
    tipostring comando;


    printf("Sending e-mail...\n");

    // If it is not whitelisted
    if (!is_whitelisted(comeca->info))
    {
        strcpy(mail_temp_file, "/tmp/netactuator_mail_temp_file.txt");

        for(i=0; i<num_admin_contacts; i++)
        {
            if (mail_handler = rfopen(mail_temp_file, "wt"))
            {
                fprintf(mail_handler, "Host \"%s\" with \"%ld\" conversations as origin was blocked by netactuator.\n", comeca->info, comeca->convs_as_source);
                fprintf(mail_handler, "Baseline: %ld\n", baseline);
                fprintf(mail_handler, "Limit: %.0f\n\n", baseline * threshold);
                fclose(mail_handler);
                printf("Sending mail to %s\n", admin_contacts[i]);
                sprintf(comando, "mail -s 'netactuator - Alert at Host \"%s\"' %s < %s", comeca->info, admin_contacts[i], mail_temp_file);
                if (flag_send_email)
                    system(comando);
            }
        }
    }
//    else
//    {
//        printf("Nada a ser feito, IP %s pertence à WhiteList\n", comeca->info);
//    }

    printf("\n");
    fflush(stdout);
}


// Apply the rule to the firewall with the host detected as an intruder
int bloquear_host (tipostring info, int expire)
{
    FILE *in_handler;
    tipostring comando;
    tipostring linha;
    int flag_bloqueado=0;


//    printf("Apllying to firewall...\n");

    // If it is not whitelisted
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

            printf("Blocking host: %s\n", info);
            if (flag_block_hosts)
                system(comando);
            inserir_blacklist(info, expire);

            fflush(stdout);
            return 1;
        }
    }
//    else
//    {
//        printf("Nada a ser feito, IP %s pertence à WhiteList\n", info);
//    }

    return 0;
}


// Apply the rule of liberation in firewall
void desbloquear_host (tipostring info)
{
    FILE *in_handler;
    tipostring comando;
    tipostring linha;
    int flag_bloqueado=0;


//    printf("Apply the rule of liberation in firewall...\n");

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

        printf("Releasing host: %s\n", info);
        if (flag_block_hosts)
            system(comando);
        fflush(stdout);
    }

    remover_blacklist(info);
    fflush(stdout);
}
