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


// Função que padroniza procedimento de abertura de arquivos
FILE *rfopen (tipostring file_temp, tipostring modo)
{
    FILE *temp_handler;


    temp_handler = fopen(file_temp, modo);

    if (temp_handler == NULL)
    {
        printf("\n\n");
        printf("Problem opeing file: %s", file_temp);
        printf("\n\n");
        return NULL;
    }
    else
        return temp_handler;
}


// Função que padroniza procedimento de abertura de pipes de comandos
FILE *rpopen (tipostring cmd_temp, tipostring modo)
{
    FILE *temp_handler;


    temp_handler = popen(cmd_temp, modo);

    if (temp_handler == NULL)
    {
        printf("\n\n");
        printf("Problem executing command: %s", cmd_temp);
        printf("\n\n");
        return NULL;
    }
    else
        return temp_handler;
}


// Verifica se um IP pertence a uma das redes definidas pelo usuário
int is_network_defined (tipostring info)
{
        struct in_addr addr;
        unsigned long ip_avaliar=0;
    int i=0;


    /* Convert the string ip address to network order address */
    if (inet_aton(info, &addr))
    {
        ip_avaliar = ntohl(addr.s_addr);
        for(i=0; i<num_networks; i++)
            if (ip_avaliar >= networks_ranges[i][0] && ip_avaliar <= networks_ranges[i][1])
            {
                // Pertence a uma das redes definidas
//                printf("IP %s pertence a uma das redes definidas.\n", info);
                return 1;
            }
    }

    // Não pertence a nenhuma das redes definidas
//    printf("IP %s NAO pertence a uma das redes definidas.\n", info);
    return 0;
}


// Varre a coleta gerada pelo pmacctd para separar/contabilizar as conversações e dados de tráfego
void processar_coleta_gerada (tipostring iface)
{
    tipostring linha;
    tipostring comando;
    tipostring ip_src, ip_dst;
    long ip_recv_f, ip_recv_b, ip_sent_f, ip_sent_b, packets, bytes;
    FILE *in_handler;


    // Captura os dados já coletados e zera para a nova coleta iniciar imediatamente, nenhum byte é perdido neste modelo
    sprintf(comando, "%s -s -p /tmp/collect.pipe.%s -e | grep -v SRC | grep -v total | \
        grep -v INFO | grep -v OK", pmacct_bin, iface);
//printf("Lançando pmacct - %s\n", comando);
    in_handler = rpopen(comando, "r");
    if (in_handler)
    {
        while (fgets(linha, MAX_TAM_LINHA, in_handler)) // Captura as estatísticas
        {
            // Não processa a linha final do pmacct, uma linha em branco...
            if (linha[0] !=  '\n')
            {
                // Incrementa o contador dos registros nos logs (com repeticao)
                ++count_log;
//printf("linha : %s", linha);
//printf("leitura linha..\n");
                // Captura os dados vindos do rpopen
                sscanf(linha, "%s %s %ld %ld", ip_src, ip_dst, &packets, &bytes);

                // Antes de adicionar o registro na árvore deve-se verificar se o IP pertence a uma das redes definidas em netactuator.conf
                if (is_network_defined(ip_src))
                {
                    // Quando o IP é source ele fez UPLOAD
                    ip_sent_f = packets;
                    ip_sent_b = bytes;
                    adicionar_registro_arvore(ip_src, 0, 0, ip_sent_f, ip_sent_b, iface);
                }
                if (is_network_defined(ip_dst))
                {
                    // Quando o IP é destin ele fez DOWNLOAD
                    ip_recv_f = packets;
                    ip_recv_b = bytes;
                    adicionar_registro_arvore(ip_dst, ip_recv_f, ip_recv_b, 0, 0, iface);
                }
            } // if linha
        } // while fgets linha

        pclose(in_handler);
    } // in_handler rpopen pmacct
}
