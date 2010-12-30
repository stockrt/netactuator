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


void pmacctd_wrapper (tipostring iface)
{
    tipostring comando;


    // Monta o comando a ser executado.
    sprintf(comando, "%s \
            -c src_host,dst_host \
            -p /tmp/collect.pipe.%s \
            -P memory \
            -i %s \
            -F /var/run/pmacctd.pid.%s \
            -D", pmacctd_bin, iface, iface, iface);
    system(comando);
}
