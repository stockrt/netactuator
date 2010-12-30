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


// Cadastra um IP bloqueado na lista de bloqueados
void inserir_blacklist (tipostring host, int expire)
{
    MYSQL *conn=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row;

    time_t data_hora;

    tipostring query;
    long time_bloqueio, expire_time;


    if ((conn = conectar()))
    {
        // Captura a data atual
        time(&data_hora);

        time_bloqueio = data_hora;
        expire_time = time_bloqueio + (block_time_min * 60);

        // Só insere se vir com 1, casos automáticos, não via web
        if (expire)
        {
            sprintf(query, "INSERT INTO %s (host,time_bloqueio,expire_time,description) VALUES('%s', '%ld', '%ld', 'Bloqueado pelo netactuator');", BLACKLIST_T, host, time_bloqueio, expire_time);
            res = sql(query, conn);
        }

        desconectar(conn);
    } // if conn
}


// Descadastra um host da lista de bloqueados
void remover_blacklist (tipostring host)
{
    MYSQL *conn=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row;

    tipostring query;


    if ((conn = conectar()))
    {
        sprintf(query, "DELETE FROM %s WHERE host = '%s';", BLACKLIST_T, host);
        res = sql(query, conn);

        desconectar(conn);
    } // if conn
}


// Monitora os expire times dos bloqueios (flutuação) e faz bloqueios da web
void on_the_fly (void)
{
    MYSQL *conn=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row;

    time_t data_hora;

    tipostring query;
    tipostring host;
    tipostring comando;
    long expire_time;
    FILE *in_handler;
    tipostring linha;
    int flag_consta=0;


    while (1)
    {
        // Captura a data atual
        time(&data_hora);

        if ((conn = conectar()))
        {
            sprintf(query, "SELECT host,expire_time FROM %s;", BLACKLIST_T);
            res = sql(query, conn);

            while ((row = mysql_fetch_row(res)))
            {
                strcpy(host, row[0]);
                expire_time = atol(row[1]);

                if (expire_time <= data_hora && expire_time != 0)
                    if (flag_block_hosts)
                    {
                        desbloquear_host(host);
                        registrar_evento(host, -1, -1, "Desbloqueado");
                    }
                else
                    if (flag_block_hosts)
                        bloquear_host(host, 0); // Será testado se já está ou não bloqueado
            }

            // Gera a lista dos atuais bloqueados no firewall e verifica quais deve liberar
            if (is_ipfw)
                sprintf(comando, "%s list | grep \"deny ip from\" | grep \"to any\" | awk '{print $5}'", fire_bin);
            else if (is_iptables)
                sprintf(comando, "%s -L -n -t filter | grep DROP | awk '{print $4}'", fire_bin);

            in_handler = rpopen(comando, "r");
            if (in_handler)
            {
                while (fgets(linha, MAX_TAM_LINHA, in_handler))
                {
                    flag_consta = 0; // para cada host, zerar a marcação e verificar na tabela
                    linha[strlen(linha)-1] = '\0';    // remove o \n do fim da string
                    sprintf(query, "SELECT * FROM %s WHERE host = '%s';", BLACKLIST_T, linha);
//printf("q: %s\n", query);
                    res = sql(query, conn);

                    // Verifica se o host consta como bloqueado, caso contrário o libera (foi liberado pela web)
                    if ((row = mysql_fetch_row(res)))
                        flag_consta = 1;
                    mysql_free_result(res);

                    if (!flag_consta)
                        if (flag_block_hosts)
                            desbloquear_host(linha);
//printf("while filter drops\n");
//fflush(stdout);
                }

                pclose(in_handler);
            }

            desconectar(conn);
        } // if conn

        fflush(stdout);
        sleep(DELAY_EXPIRE);
    } // while not the end of the world
}
