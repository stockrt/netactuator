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


// *** DEFINES ***

#define MAX_TAM_LINHA   5000
#define MAX_ARGS        128

// Intervalo entre cada leitura das configs do banco
#define DELAY_LEITURA_CONFIGS   20
// Intervalo entre cada verificação de hora para update do pattern
#define DELAY_UPDATE            60
// Pausa entre uma verificação de expire time de host bloqueado e outra
#define DELAY_EXPIRE            40
// Intervalo entre iterações do laço principal
// 2 s = 20 = ds = 200 cs = 2000 ms = 20000 = ?s = 200000 ?s = 2000000 us -> Usando 20 ms = 20000 us
#define DELAY_MAIN              20000

// Flags dos registros limpos e sujos do DB (ok e comprometidos)
#define FLAG_CLEAN    1
#define FLAG_DIRTY    0

// Número de tentativas de conexão com o MySQL antes de desistir
#define NUM_DB_TRY    5

// Tabelas
#define ADMIN_T         "config_admincontacts"
#define INTERFACES_T    "config_interfaces"
#define netactuator_T   "config_netactuator"
#define NETWORKS_T      "config_networks"

#define BLACKLIST_T     "net_blacklist"
#define TOPUSERS_T      "net_topusers"
#define WHITELIST_T     "net_whitelist"

#define MASS_T          "storage_mass"
#define PATTERN_T       "storage_pattern_def"

#define EVENTOS_T       "storage_eventos"

// *** FIM DEFINES ***


// *** GLOBAIS ***

typedef char tipostring[MAX_TAM_LINHA];
time_t ini, end;    // marcadores de tempo de execucao
long count_log=0;   // contador de registros nos logs, com repeticao


// Árvore
struct TreeNode;
typedef struct TreeNode *SearchTree;

struct TreeNode
{
    tipostring info;
    long recv_f;
    long sent_f;
    long recv_b;
    long sent_b;
    long convs_as_source;
    long convs_as_destin;
    tipostring iface;
    SearchTree esq;
    SearchTree dir;
};

SearchTree raiz=NULL;


// Lista
struct ListNode;
typedef struct ListNode *SeqList;

struct ListNode
{
    tipostring info;
    long recv_f;
    long sent_f;
    long recv_b;
    long sent_b;
    long convs_as_source;
    long convs_as_destin;
    tipostring iface;
    SeqList p;
};

SeqList top_users_frames_recv=NULL;
SeqList top_users_frames_sent=NULL;
SeqList top_users_bytes_recv=NULL;
SeqList top_users_bytes_sent=NULL;
SeqList top_users_convs_as_source=NULL;
SeqList top_users_convs_as_destin=NULL;


// Gaph Thread
struct graph_data;
typedef struct graph_data graph_t;
struct graph_data
{
    tipostring info;
    long recv_f;
    long sent_f;
    long recv_b;
    long sent_b;
    long convs_as_source;
    long convs_as_destin;
    tipostring iface;
    long baseline;
};


// Configurações
int flag_loading_config=1;  // inicia carregando configurações
int flag_limit_update=0;    // bloqueia geração de gráficos durante o limit (update rrd, evita acessos múltiplos aos rrds)

tipostring interfaces[MAX_ARGS];
int num_interfaces=0;

tipostring networks[MAX_ARGS][2]; // campo 0 = rede, campo 1 = máscara
int num_networks=0;
unsigned long networks_ranges[MAX_ARGS][3]; // campo 0 = ip_inicial, campo 1 = ip_final, campo 2 = maskbits (inseridos somente na função netmask_paul_davis())

int flow_capture_time_min=0;
int flow_capture_time_sec=0;

//int graph_update_time_min=45;   // minutos, colocar no banco
int graph_update_time_min=5;    // minutos, colocar no banco

tipostring blacklist[MAX_ARGS];
int num_blacklist=0;

tipostring whitelist[MAX_ARGS];
int num_whitelist=0;

tipostring admin_contacts[MAX_ARGS];
int num_admin_contacts=0;

tipostring pmacctd_bin;
tipostring pmacct_bin;

tipostring fire_bin;

int top_users=0;

tipostring base_www;

int block_time_min=0;

int days_to_keep=0;

int days_to_learn=0;

float threshold=0;

int flag_block_hosts=0;

int flag_send_email=0;


// Firewall
int is_ipfw=0;
int is_iptables=0;


// Database
tipostring db_host;
tipostring db_user;
tipostring db_password;
tipostring db_name;


// Datetime
time_t data_hora_global;    // Usado para fazer update com valor global nos RRDs
tipostring data;
tipostring hora;
tipostring week_day;        // se usar int vai dar erro ao atribuir o dia da semana a partir da struct sdata em netactuator.c

// *** FIM GLOBAIS ***
