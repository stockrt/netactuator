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


#include <mysql/mysql.h>


MYSQL *mysql_conn_global;


//int conectar (void)
MYSQL * conectar (void)
{
    MYSQL *mysql_conn;
    int i, flag_conectou=0;


    mysql_conn = mysql_init(NULL);
    if (mysql_conn == NULL)
    {
        printf("mysql_init() failed (probably out of memory)\n");
        return NULL;
    }

    // mysql_conn, host, user, passwd, db
    for(i=0; i<NUM_DB_TRY; i++)
    {
//printf("tentando conectar %d\n", i);
        if (mysql_real_connect(mysql_conn, db_host, db_user, db_password, db_name, 0, NULL, 0))
        {
            flag_conectou = 1;
            break;
        }

        sleep(1);
    } // for

    if (!flag_conectou)
    {
        printf("Erro ao conectar ao banco: %s\n", mysql_error(mysql_conn));
        mysql_close(mysql_conn);
        return NULL;
    }
    else
    {
//        printf("Conectado...\n");
        return mysql_conn;
    }
}


//void desconectar (void)
void desconectar (MYSQL *mysql_conn)
{
    mysql_close(mysql_conn);
//    printf("Desconectado...\n");
}


// Executa um SQL e retorna o pointer dos valores encontrados
MYSQL_RES * sql(tipostring query, MYSQL *mysql_conn)
{
    MYSQL_RES *res=NULL;
    int t;


    t = mysql_real_query(mysql_conn, query, (unsigned int) strlen(query));
    if (t)
    {
        printf("Erro ao fazer a query: %s\n", mysql_error(mysql_conn));
        return NULL;
    }
    else
    {
        res = mysql_use_result(mysql_conn);
        return res;
    }
}
