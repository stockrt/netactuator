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


// Cadastra um evento
void registrar_evento (tipostring host, long convs_as_source, long baseline, tipostring description)
{
    MYSQL *conn=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row;

    tipostring query;


    if ((conn = conectar()))
    {
        printf("Registrando novo evento %s %ld %ld %s\n", host, convs_as_source, baseline, description);
        sprintf(query, "INSERT INTO %s VALUES('%s', '%ld', '%ld', '%s', '%s', '%s', '%s');",
            EVENTOS_T, host, convs_as_source, baseline, data, hora, week_day, description);
        res = sql(query, conn);

        desconectar(conn);
    } // if conn
}
