USE mysql;

-- Criação do usuário
--REPLACE INTO user (host, user, password) VALUES ('%', 'netactuator', OLD_PASSWORD('netdbpass'));
REPLACE INTO user (host, user, password) VALUES ('localhost', 'netactuator', OLD_PASSWORD('netdbpass'));

-- Permissões de acesso do usuário
--REPLACE INTO db (host, db, user, select_priv, insert_priv, update_priv, delete_priv, create_priv, drop_priv, index_priv)
--	VALUES (
--	'%',
--	'netactuator',
--	'netactuator',
--	'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'Y'
--);
REPLACE INTO db (host, db, user, select_priv, insert_priv, update_priv, delete_priv, create_priv, drop_priv, index_priv)
	VALUES (
	'localhost',
	'netactuator',
	'netactuator',
	'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'Y'
);

-- Atualiza os privilégios dados
FLUSH PRIVILEGES;

-- Criação do banco de dados e conexão com o banco
CREATE DATABASE IF NOT EXISTS netactuator;
USE netactuator;

-- Configurações da ferramenta
DROP TABLE IF EXISTS config_netactuator;
CREATE TABLE config_netactuator (
	id			INT UNSIGNED NOT NULL auto_increment,
	flow_capture_time_min	INT UNSIGNED NOT NULL,
	pmacctd_bin		TEXT NOT NULL,
	pmacct_bin		TEXT NOT NULL,
	fire_bin		TEXT NOT NULL,
	top_users		INT UNSIGNED NOT NULL,
	base_www		TEXT NOT NULL,
	block_time_min		INT UNSIGNED NOT NULL,
	days_to_keep		INT UNSIGNED NOT NULL,
	days_to_learn		INT UNSIGNED NOT NULL,
	threshold		INT UNSIGNED NOT NULL,
	PRIMARY KEY (id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
--INSERT INTO config_netactuator VALUES ('', 5, '/usr/local/sbin/pmacctd', '/usr/local/bin/pmacct', '/sbin/ipfw', 15, '/usr/local/netactuator/web', 30, 90, 7, 300);
INSERT INTO config_netactuator VALUES ('', 5, '/usr/local/sbin/pmacctd', '/usr/local/bin/pmacct', '/sbin/iptables', 15, '/usr/local/netactuator/web', 30, 90, 7, 300);

-- Configuração das interfaces de rede a serem monitoradas
DROP TABLE IF EXISTS config_interfaces;
CREATE TABLE config_interfaces (
	id		INT UNSIGNED NOT NULL auto_increment,
	interface	VARCHAR(7) NOT NULL,
	PRIMARY KEY (id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
INSERT INTO config_interfaces VALUES ('', 'eth0');
INSERT INTO config_interfaces VALUES ('', 'eth1');
INSERT INTO config_interfaces VALUES ('', 'xl0');
INSERT INTO config_interfaces VALUES ('', 'xl1');
INSERT INTO config_interfaces VALUES ('', 'fxp0');
INSERT INTO config_interfaces VALUES ('', 'fxp1');
INSERT INTO config_interfaces VALUES ('', 'vmnet1');
INSERT INTO config_interfaces VALUES ('', 'vmnet8');
INSERT INTO config_interfaces VALUES ('', 'lnc0');
INSERT INTO config_interfaces VALUES ('', 'lo');

-- Configuração das redes a serem avaliadas
DROP TABLE IF EXISTS config_networks;
CREATE TABLE config_networks (
	id		INT UNSIGNED NOT NULL auto_increment,
	network		TEXT NOT NULL,
	PRIMARY KEY (id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
INSERT INTO config_networks VALUES ('', '10.0.0.0/255.255.255.0');
INSERT INTO config_networks VALUES ('', '10.0.1.0/255.255.255.0');
INSERT INTO config_networks VALUES ('', '200.180.200.96/27');
INSERT INTO config_networks VALUES ('', '172.16.0.0/16');
INSERT INTO config_networks VALUES ('', '192.168.0.0/16');
INSERT INTO config_networks VALUES ('', '189.0.0.0/8');
INSERT INTO config_networks VALUES ('', '127.0.0.1/30');

-- Configuração dos contatos administrativos
DROP TABLE IF EXISTS config_admincontacts;
CREATE TABLE config_admincontacts (
	id		INT UNSIGNED NOT NULL auto_increment,
	contact		TEXT NOT NULL,
	email		TEXT NOT NULL,
	PRIMARY KEY (id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
INSERT INTO config_admincontacts VALUES ('', 'Rogério Schneider', 'stockrt@gmail.com');

-- IPs sempre liberados
DROP TABLE IF EXISTS net_whitelist;
CREATE TABLE net_whitelist (
	id		INT UNSIGNED NOT NULL auto_increment,
	host		VARCHAR(15) NOT NULL,
	description	TEXT NOT NULL,
	PRIMARY KEY (id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
INSERT INTO net_whitelist VALUES ('', '200.180.200.125', 'stockrt');
INSERT INTO net_whitelist VALUES ('', '200.180.200.126', 'island');

-- IPs bloqueados
DROP TABLE IF EXISTS net_blacklist;
CREATE TABLE net_blacklist (
	id		INT UNSIGNED NOT NULL auto_increment,
	host		VARCHAR(15) NOT NULL,
	time_bloqueio	INT UNSIGNED NOT NULL,
	expire_time	INT UNSIGNED NOT NULL,
	description	TEXT NOT NULL,
	PRIMARY KEY (id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- IPs com maior utilização de recurso
DROP TABLE IF EXISTS net_topusers;
CREATE TABLE net_topusers (
	host			VARCHAR(15) NOT NULL,
	convs_as_source		INT UNSIGNED NOT NULL,
	convs_as_destin		INT UNSIGNED NOT NULL,
	frames_in		INT UNSIGNED NOT NULL,
	frames_out		INT UNSIGNED NOT NULL,
	bytes_in		INT UNSIGNED NOT NULL,
	bytes_out		INT UNSIGNED NOT NULL,
	toptype			ENUM('1', '2', '3', '4', '5', '6') NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
# 1 - conversations_as_source
# 2 - conversations_as_destin
# 3 - frames_recv
# 4 - frames_sent
# 5 - bytes_recv
# 6 - bytes_sent

-- Armazenamento geral dos dados coletados
DROP TABLE IF EXISTS storage_mass;
CREATE TABLE storage_mass (
	host			VARCHAR(15) NOT NULL,
	convs_as_source		INT UNSIGNED NOT NULL,
	convs_as_destin		INT UNSIGNED NOT NULL,
	frames_in		INT UNSIGNED NOT NULL,
	frames_out		INT UNSIGNED NOT NULL,
	bytes_in		INT UNSIGNED NOT NULL,
	bytes_out		INT UNSIGNED NOT NULL,
	data			DATE NOT NULL,
	hora			TIME NOT NULL,
	week_day		ENUM('0', '1', '2', '3', '4', '5', '6') NOT NULL,
	flow_capture_time_min	TINYINT UNSIGNED NOT NULL,
	flag_clean		TINYINT UNSIGNED NOT NULL,
	interface		VARCHAR(5) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- Armazenamento dos padrões definidos
DROP TABLE IF EXISTS storage_pattern_def;
CREATE TABLE storage_pattern_def (
	host			VARCHAR(15) NOT NULL,
	convs_as_source_avg	INT UNSIGNED NOT NULL,
	convs_as_source_max	INT UNSIGNED NOT NULL,
	PRIMARY KEY (host)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- Armazenamento dos eventos ocorridos
DROP TABLE IF EXISTS storage_eventos;
CREATE TABLE storage_eventos (
	host			VARCHAR(15) NOT NULL,
	convs_as_source		INT NOT NULL,
	baseline		INT NOT NULL,
	data			DATE NOT NULL,
	hora			TIME NOT NULL,
	week_day		ENUM('0', '1', '2', '3', '4', '5', '6') NOT NULL,
	description		TEXT NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- Configuração de visualizacão/alteração das tabelas pela interface gráfica
DROP TABLE IF EXISTS config_tables;
CREATE TABLE config_tables (
	id		INT UNSIGNED NOT NULL auto_increment,
	table_name	VARCHAR(80) DEFAULT NULL,
	table_view	INT NOT NULL,
	id_view		INT NOT NULL,
	insert_view	INT NOT NULL,
	delete_view	INT NOT NULL,
	alter_view	INT NOT NULL,
	description	TEXT NOT NULL,
	PRIMARY KEY (id),
	UNIQUE KEY `table` (table_name)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
INSERT INTO config_tables VALUES ('', 'config_netactuator', 1, 0, 0, 0, 1, 'CONFIGURACOES');
INSERT INTO config_tables VALUES ('', 'config_interfaces', 1, 0, 1, 1, 1, 'INTERFACES');
INSERT INTO config_tables VALUES ('', 'config_networks', 1, 0, 1, 1, 1, 'NETWORKS');
INSERT INTO config_tables VALUES ('', 'config_admincontacts', 1, 0, 1, 1, 1, 'CONTATOS');
INSERT INTO config_tables VALUES ('', 'net_whitelist', 1, 0, 1, 1, 1, 'WHITELIST');
INSERT INTO config_tables VALUES ('', 'net_blacklist', 1, 0, 1, 1, 0, 'BLACKLIST');
INSERT INTO config_tables VALUES ('', 'net_topusers', 1, 0, 0, 0, 0, 'TOP USERS');
INSERT INTO config_tables VALUES ('', 'storage_mass', 0, 0, 0, 0, 0, 'STOR_MASS');
INSERT INTO config_tables VALUES ('', 'storage_pattern_def', 1, 0, 0, 0, 0, 'PADROES');
INSERT INTO config_tables VALUES ('', 'storage_eventos', 1, 0, 0, 0, 0, 'EVENTOS');
INSERT INTO config_tables VALUES ('', 'config_tables', 0, 0, 1, 1, 1, 'TABELAS');

-- Ajusta privilégios de acesso
--GRANT ALL PRIVILEGES ON netactuator.* TO 'netactuator'@'%';
GRANT ALL PRIVILEGES ON netactuator.* TO 'netactuator'@'localhost';

-- Atualiza os privilégios concedidos
FLUSH PRIVILEGES;
