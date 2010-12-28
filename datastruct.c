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


// *** BIN PESQ ***

// Insere um novo nodo na arvore
SearchTree inserir_nodo_arvore (SearchTree comeca, SearchTree novo_nodo, SearchTree volta) // ok -> recursiva
{
	if (comeca) // se nao chegou ao final (ainda !NULL)
	{
		// jah existe - retorna nodo encontrado
		if (!strcmp(comeca->info, novo_nodo->info))
			return comeca;

		// se for maior, insere na direita
		else if (strcmp(comeca->info, novo_nodo->info) < 0) // se anterior menor, vai para a direita
		{
			if (!comeca->dir) // se vago, insere
			{
				comeca->dir = novo_nodo;
				return NULL;
			}
			else // senao, procura outro
				volta = inserir_nodo_arvore(comeca->dir, novo_nodo, volta);
		}

		// se for menor, insere na esquerda
		else if (strcmp(comeca->info, novo_nodo->info) > 0) // se anterior maior, vai para a esquerda
		{
			if (!comeca->esq) // se vago, insere
			{
				comeca->esq = novo_nodo;
				return NULL;
			}
			else // senao, procura proximo
				volta = inserir_nodo_arvore(comeca->esq, novo_nodo, volta);
		}
	}

	return volta;
}


// Mostra todos os registros cadastrados
void mostrar_registros_arvore (SearchTree comeca)
{
	if (comeca)
	{
		mostrar_registros_arvore(comeca->dir);
		mostrar_registros_arvore(comeca->esq);

		printf("IP: %s ->", comeca->info);
		printf(" Recv Frames: %ld", comeca->recv_f);
		printf(" Recv Bytes: %ld", comeca->recv_b);
		printf(" Sent Frames: %ld", comeca->sent_f);
		printf(" Sent Bytes: %ld", comeca->sent_b);
		printf(" Source: %ld", comeca->convs_as_source);
		printf(" Destino: %ld", comeca->convs_as_destin);
		printf(" Iface: %s", comeca->iface);
		printf("\n");
	}
}


// Destroi a arvore para liberar memoria
void destruir_arvore (SearchTree comeca)
{
	if (comeca)
	{
		destruir_arvore(comeca->dir);
		destruir_arvore(comeca->esq);

		free(comeca);
	}
}


// Contador de registros inseridos, sem repeticao
long contar_registros_arvore (SearchTree comeca, long count)
{
	if (comeca)
	{
		count = contar_registros_arvore(comeca->dir, count);
		count = contar_registros_arvore(comeca->esq, count);

		++count;
	}

	return count;
}

// *** FIM BIN PESQ ***



// *** LISTA ***

// Cria as listas de top users com top_users nodos alocados
SeqList criar_lista (int nodos)
{
	SeqList list=NULL;
	SeqList atual=NULL;
	int i;


	// Cria o primeiro nodo da lista
	if (!(list = (SeqList) malloc(sizeof(struct ListNode))))
	{
		printf("Sem memoria para continuar.\n");
		exit(1);
	}
	strcpy(list->info, "vago");
	list->recv_f = 0;
	list->recv_b = 0;
	list->sent_f = 0;
	list->sent_b = 0;
	list->convs_as_source = 0;
	list->convs_as_destin = 0;
	strcpy(list->iface, "");
	list->p = NULL;

	// Cria os nodos seguintes da lista
	for(i=0; i<nodos; i++)
	{
		// Percorre a lista até o seu fim e insere um nodo nodo
		atual = list;
		while (atual->p)
			atual = atual->p;

		if (!(atual->p = (SeqList) malloc(sizeof(struct ListNode))))
		{
			printf("Sem memoria para continuar.\n");
			exit(1);
		}
		strcpy(atual->p->info, "vago");
		atual->p->recv_f = 0;
		atual->p->recv_b = 0;
		atual->p->sent_f = 0;
		atual->p->sent_b = 0;
		atual->p->convs_as_source = 0;
		atual->p->convs_as_destin = 0;
		strcpy(atual->iface, "");
		atual->p->p = NULL;
	}

	return list;
}


// Mostra todos os registros cadastrados
void mostrar_registros_lista (SeqList comeca)
{
	if (comeca)
	{
		mostrar_registros_lista(comeca->p);

		printf("IP: %s ->", comeca->info);
		printf(" Recv Frames: %ld", comeca->recv_f);
		printf(" Recv Bytes: %ld", comeca->recv_b);
		printf(" Sent Frames: %ld", comeca->sent_f);
		printf(" Sent Bytes: %ld", comeca->sent_b);
		printf(" Source: %ld", comeca->convs_as_source);
		printf(" Destino: %ld", comeca->convs_as_destin);
		printf(" Iface: %s", comeca->iface);
		printf("\n");
	}
}


// Destroi a lista para liberar memoria
void destruir_lista (SeqList comeca)
{
	if (comeca)
	{
		destruir_lista(comeca->p);

		free(comeca);
	}
}

// *** FIM LISTA ***
