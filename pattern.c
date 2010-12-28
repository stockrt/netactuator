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


// Atualiza a tabela de padrão de comportamento
void update_pattern_def (void)
{
	MYSQL *conn=NULL;
	MYSQL_RES *res=NULL;
	MYSQL_ROW row;

	tipostring query;
	time_t data_hora;
	struct tm *sdata;
	int last_value_hour=-1;         // em cada laço do while principal ele é avaliado


	while (1)
	{
		// Captura a data atual
		time(&data_hora);
		sdata = localtime(&data_hora);

		// 6 - ATUALIZAÇÃO DE PADRÃO COMPORTAMENTAL
		// Se o valor da hora for diferente do valor anterior
		// então é uma nova hora, e o update da tabela pattern_def
		// deverá ser realizado
		if (sdata->tm_hour != last_value_hour)
//		if (sdata->tm_min != last_value_hour)
		{
//printf("pattern... pattern... pattern... pattern... pattern... pattern... \n");
inicial();
			if ((conn = conectar()))
			{
				// Limpa os mais antigos de MASS_T
				sprintf(query, "DELETE FROM %s WHERE data < DATE_SUB(CURDATE(), INTERVAL %d DAY);", MASS_T, days_to_keep);
				res = sql(query, conn);

				// Zera PATTERN_T
				sprintf(query, "DELETE FROM %s;", PATTERN_T);
				res = sql(query, conn);

				// Atualiza
				sprintf(query, "INSERT INTO %s () \
					SELECT host, AVG(convs_as_source), MAX(convs_as_source) \
					FROM %s WHERE \
					convs_as_source > 0 AND \
					flow_capture_time_min = %d AND \
					flag_clean = %d \
					GROUP BY host HAVING MIN(data) < DATE_SUB(CURDATE(), INTERVAL %d DAY);",
					PATTERN_T, MASS_T, flow_capture_time_min, FLAG_CLEAN, days_to_learn);
				res = sql(query, conn);

				desconectar(conn);
			} // if conn

final("PATTERN");
			last_value_hour = sdata->tm_hour;
//			last_value_hour = sdata->tm_min;

		}

		sleep(DELAY_UPDATE);
	} // while not the end of the world
}
