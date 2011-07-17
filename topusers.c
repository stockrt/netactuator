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


// Adiciona um novo registro na arvore (um novo IP foi encontrado e precisa ser cadastrado)
void adicionar_registro_arvore (tipostring ip, long recv_f, long recv_b, long sent_f, long sent_b, tipostring iface)
{
    SearchTree novo_nodo=NULL;
    SearchTree retorno=NULL;


    // Se ainda nao tiver raiz
    if (!raiz)
    {
        if (!(raiz = (SearchTree) malloc(sizeof(struct TreeNode))))
        {
            printf("Not enough memory to continue.\n");
            exit(1);
        }

        strcpy(raiz->info, ip);
        raiz->recv_f = recv_f;
        raiz->recv_b = recv_b;
        raiz->sent_f = sent_f;
        raiz->sent_b = sent_b;
        strcpy(raiz->iface, iface);

        // Se enviou e nao recebeu frames
        if (sent_f && !recv_f)
        {
            raiz->convs_as_source = 1;
            raiz->convs_as_destin = 0;
        }
        // Se recebeu e nao enviou frames
        else if (recv_f && !sent_f)
        {
            raiz->convs_as_source = 0;
            raiz->convs_as_destin = 1;
        }
        // Se enviou e recebeu frames
        else if (sent_f && recv_f)
        {
            raiz->convs_as_source = 1;
            raiz->convs_as_destin = 1;
        }

        raiz->dir = NULL;
        raiz->esq = NULL;
    }
    else // Novo nodo
    {
        if (!(novo_nodo = (SearchTree) malloc(sizeof(struct TreeNode))))
        {
            printf("Not enough memory to continue.\n");
            exit(1);
        }

        strcpy(novo_nodo->info, ip);
        novo_nodo->recv_f = recv_f;
        novo_nodo->recv_b = recv_b;
        novo_nodo->sent_f = sent_f;
        novo_nodo->sent_b = sent_b;
        strcpy(novo_nodo->iface, iface);

        // Se enviou e nao recebeu frames
        if (sent_f && !recv_f)
        {
            novo_nodo->convs_as_source = 1;
            novo_nodo->convs_as_destin = 0;
        }
        // Se recebeu e nao enviou frames
        else if (recv_f && !sent_f)
        {
            novo_nodo->convs_as_source = 0;
            novo_nodo->convs_as_destin = 1;
        }
        // Se enviou e recebeu frames
        else if (sent_f && recv_f)
        {
            novo_nodo->convs_as_source = 1;
            novo_nodo->convs_as_destin = 1;
        }

        novo_nodo->dir = NULL;
        novo_nodo->esq = NULL;


        retorno = inserir_nodo_arvore(raiz, novo_nodo, NULL); // chama a recursao para situar o novo nodo na arvore

        if (retorno != NULL) // Jah existe, soma os valores
        {
            retorno->recv_f += novo_nodo->recv_f;
            retorno->recv_b += novo_nodo->recv_b;
            retorno->sent_f += novo_nodo->sent_f;
            retorno->sent_b += novo_nodo->sent_b;

            // Se enviou e nao recebeu frames
            if (novo_nodo->sent_f && !novo_nodo->recv_f)
            {
                retorno->convs_as_source += 1;
                retorno->convs_as_destin += 0;
            }
            // Se recebeu e nao enviou frames
            else if (novo_nodo->recv_f && !novo_nodo->sent_f)
            {
                retorno->convs_as_source += 0;
                retorno->convs_as_destin += 1;
            }
            // Se enviou e recebeu frames
            else if (novo_nodo->sent_f && novo_nodo->recv_f)
            {
                retorno->convs_as_source += 1;
                retorno->convs_as_destin += 1;
            }

/*
// Outra forma de somar as conversacoes
            // Se enviou frames foi uma conversação onde atuou como source
            if (novo_nodo->sent_f)
                retorno->convs_as_source += 1;
            // Se recebeu frames foi uma conversação onde atuou como destino
            if (novo_nodo->recv_f)
                retorno->convs_as_destin += 1;
*/

            // nao inseriu pos jah existia, mas jah somou os valores ao nodo previamente existente
            free(novo_nodo);
        }
    }
}


// Insere um Top User na lista caso ele seja maior em seu quesito do que um dos atuais integrantes da lista
void inserir_top_user (SearchTree to_insert, tipostring tipo)
{
    SeqList comeca=NULL;


    if (!strcmp(tipo, "frames_recv"))
    {
        comeca = top_users_frames_recv;
        while (comeca)
        {
            // If the value is higher than proposed in the node, we are insterting it
            if (to_insert->recv_f > comeca->recv_f)
            {
                // Inserção
                strcpy(comeca->info, to_insert->info);
                strcpy(comeca->iface, to_insert->iface);
                comeca->recv_f = to_insert->recv_f;
                break;
            //    comeca = NULL; // sai do laço
            }
            // Vai ao próximo nodo
            else
                comeca = comeca->p;
        }
    }
    else if (!strcmp(tipo, "frames_sent"))
    {
        comeca = top_users_frames_sent;
        while (comeca)
        {
            // If the value is higher than proposed in the node, we are insterting it
            if (to_insert->sent_f > comeca->sent_f)
            {
                // Inserção
                strcpy(comeca->info, to_insert->info);
                strcpy(comeca->iface, to_insert->iface);
                comeca->sent_f = to_insert->sent_f;
                break;
            //    comeca = NULL; // sai do laço
            }
            // Vai ao próximo nodo
            else
                comeca = comeca->p;
        }
    }
    else if (!strcmp(tipo, "bytes_recv"))
    {
        comeca = top_users_bytes_recv;
        while (comeca)
        {
            // If the value is higher than proposed in the node, we are insterting it
            if (to_insert->recv_b > comeca->recv_b)
            {
                // Inserção
                strcpy(comeca->info, to_insert->info);
                strcpy(comeca->iface, to_insert->iface);
                comeca->recv_b = to_insert->recv_b;
                break;
            //    comeca = NULL; // sai do laço
            }
            // Vai ao próximo nodo
            else
                comeca = comeca->p;
        }
    }
    else if (!strcmp(tipo, "bytes_sent"))
    {
        comeca = top_users_bytes_sent;
        while (comeca)
        {
            // If the value is higher than proposed in the node, we are insterting it
            if (to_insert->sent_b > comeca->sent_b)
            {
                // Inserção
                strcpy(comeca->info, to_insert->info);
                strcpy(comeca->iface, to_insert->iface);
                comeca->sent_b = to_insert->sent_b;
                break;
            //    comeca = NULL; // sai do laço
            }
            // Vai ao próximo nodo
            else
                comeca = comeca->p;
        }
    }
    else if (!strcmp(tipo, "convs_as_source"))
    {
        comeca = top_users_convs_as_source;
        while (comeca)
        {
            // If the value is higher than proposed in the node, we are insterting it
            if (to_insert->convs_as_source > comeca->convs_as_source)
            {
                // Inserção
                strcpy(comeca->info, to_insert->info);
                strcpy(comeca->iface, to_insert->iface);
                comeca->convs_as_source = to_insert->convs_as_source;
                break;
            //    comeca = NULL; // sai do laço
            }
            // Vai ao próximo nodo
            else
                comeca = comeca->p;
        }
    }
    else if (!strcmp(tipo, "convs_as_destin"))
    {
        comeca = top_users_convs_as_destin;
        while (comeca)
        {
            // If the value is higher than proposed in the node, we are insterting it
            if (to_insert->convs_as_destin > comeca->convs_as_destin)
            {
                // Inserção
                strcpy(comeca->info, to_insert->info);
                strcpy(comeca->iface, to_insert->iface);
                comeca->convs_as_destin = to_insert->convs_as_destin;
                break;
            //    comeca = NULL; // sai do laço
            }
            // Vai ao próximo nodo
            else
                comeca = comeca->p;
        }
    }
}


// Register Top Users nas listas varrendo toda a arvore a partir da raiz
void gerar_top_users (SearchTree comeca)
{
    if (comeca)
    {
        gerar_top_users(comeca->dir);
        gerar_top_users(comeca->esq);

        inserir_top_user(comeca, "frames_recv");
        inserir_top_user(comeca, "frames_sent");
        inserir_top_user(comeca, "bytes_recv");
        inserir_top_user(comeca, "bytes_sent");
        inserir_top_user(comeca, "convs_as_source");
        inserir_top_user(comeca, "convs_as_destin");
    }
}


// Shows list of acording to peroid of time
void mostrar_top_users (tipostring tipo)
{
    SeqList comeca=NULL;


    if (!strcmp(tipo, "frames_recv"))
    {
        printf("Top Users Frames Recived:\n");
        comeca = top_users_frames_recv;
        while (comeca)
        {
            if (strcmp(comeca->info, "vague"))
            {
                printf("IP: %s ->", comeca->info);
                printf(" Recv Frames: %ld", comeca->recv_f);
                printf(" Iface: %s", comeca->iface);
                printf("\n");
            }
            comeca = comeca->p;
        }
    }
    else if (!strcmp(tipo, "frames_sent"))
    {
        printf("Top Users Frames Sent:\n");
        comeca = top_users_frames_sent;
        while (comeca)
        {
            if (strcmp(comeca->info, "vague"))
            {
                printf("IP: %s ->", comeca->info);
                printf(" Sent Frames: %ld", comeca->sent_f);
                printf(" Iface: %s", comeca->iface);
                printf("\n");
            }
            comeca = comeca->p;
        }
    }
    else if (!strcmp(tipo, "bytes_recv"))
    {
        printf("Top Users Bytes Recived:\n");
        comeca = top_users_bytes_recv;
        while (comeca)
        {
            if (strcmp(comeca->info, "vague"))
            {
                printf("IP: %s ->", comeca->info);
                printf(" Recv Bytes: %ld", comeca->recv_b);
                printf(" Iface: %s", comeca->iface);
                printf("\n");
            }
            comeca = comeca->p;
        }
    }
    else if (!strcmp(tipo, "bytes_sent"))
    {
        printf("Top Users Bytes Sent:\n");
        comeca = top_users_bytes_sent;
        while (comeca)
        {
            if (strcmp(comeca->info, "vago"))
            {
                printf("IP: %s ->", comeca->info);
                printf(" Sent Bytes: %ld", comeca->sent_b);
                printf(" Iface: %s", comeca->iface);
                printf("\n");
            }
            comeca = comeca->p;
        }
    }
    else if (!strcmp(tipo, "convs_as_source"))
    {
        printf("Top Users Conversations as Source:\n");
        comeca = top_users_convs_as_source;
        while (comeca)
        {
            if (strcmp(comeca->info, "vago"))
            {
                printf("IP: %s ->", comeca->info);
                printf(" Convs as Source: %ld", comeca->convs_as_source);
                printf(" Iface: %s", comeca->iface);
                printf("\n");
            }
            comeca = comeca->p;
        }
    }
    else if (!strcmp(tipo, "convs_as_destin"))
    {
        printf("Top Users Conversations as Destin:\n");
        comeca = top_users_convs_as_destin;
        while (comeca)
        {
            if (strcmp(comeca->info, "vago"))
            {
                printf("IP: %s ->", comeca->info);
                printf(" Convs as Destin: %ld", comeca->convs_as_destin);
                printf(" Iface: %s", comeca->iface);
                printf("\n");
            }
            comeca = comeca->p;
        }
    }

    printf("\n");
}


// Gera os top users para dentro do DB
gerar_top_users_db (void)
{
    MYSQL *conn=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row;

    tipostring query;


    if ((conn = conectar()))
    {
        // Limpa
        sprintf(query, "DELETE FROM %s;", TOPUSERS_T);
        res = sql(query, conn);


        // Gera os top
        sprintf(query, "INSERT INTO %s () SELECT host, convs_as_source, convs_as_destin, frames_in, frames_out, bytes_in, bytes_out, \"1\" FROM %s WHERE data = '%s' AND hora = '%s' ORDER BY convs_as_source DESC limit %d;", TOPUSERS_T, MASS_T, data, hora, top_users);
        res = sql(query, conn);

        sprintf(query, "INSERT INTO %s () SELECT host, convs_as_source, convs_as_destin, frames_in, frames_out, bytes_in, bytes_out, \"2\" FROM %s WHERE data = '%s' AND hora = '%s' ORDER BY convs_as_destin DESC limit %d;", TOPUSERS_T, MASS_T, data, hora, top_users);
        res = sql(query, conn);

        sprintf(query, "INSERT INTO %s () SELECT host, convs_as_source, convs_as_destin, frames_in, frames_out, bytes_in, bytes_out, \"3\" FROM %s WHERE data = '%s' AND hora = '%s' ORDER BY frames_in DESC limit %d;", TOPUSERS_T, MASS_T, data, hora, top_users);
        res = sql(query, conn);

        sprintf(query, "INSERT INTO %s () SELECT host, convs_as_source, convs_as_destin, frames_in, frames_out, bytes_in, bytes_out, \"4\" FROM %s WHERE data = '%s' AND hora = '%s' ORDER BY frames_out DESC limit %d;", TOPUSERS_T, MASS_T, data, hora, top_users);
        res = sql(query, conn);

        sprintf(query, "INSERT INTO %s () SELECT host, convs_as_source, convs_as_destin, frames_in, frames_out, bytes_in, bytes_out, \"5\" FROM %s WHERE data = '%s' AND hora = '%s' ORDER BY bytes_in DESC limit %d;", TOPUSERS_T, MASS_T, data, hora, top_users);
        res = sql(query, conn);

        sprintf(query, "INSERT INTO %s () SELECT host, convs_as_source, convs_as_destin, frames_in, frames_out, bytes_in, bytes_out, \"6\" FROM %s WHERE data = '%s' AND hora = '%s' ORDER BY bytes_out DESC limit %d;", TOPUSERS_T, MASS_T, data, hora, top_users);
        res = sql(query, conn);


        desconectar(conn);
    } // if conn
}
