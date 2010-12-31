GCC=/usr/bin/gcc

MAIN_INPUT=netactuator.c
MAIN_OUTPUT=netactuator
CONFIG_FILE=netactuator.conf-dist
MYSQL_LIBS=`mysql_config --libs`
MYSQL_INCLUDE=`mysql_config --include`
COMP=$(GCC) -o $(MAIN_OUTPUT) -g $(MAIN_INPUT) $(MYSQL_LIBS) -lpthread -lrrd -L/usr/local/lib -L/usr/lib -L/opt/local/lib -L/opt/lib $(MYSQL_INCLUDE) -I/usr/local/include -I/usr/include -I/opt/local/include -I/opt/include
INST_DEST=/usr/local/bin
INST_DEST_CONF=/usr/local/etc/netactuator
INST_DEST_WEB=/usr/local/netactuator

default:
	@echo

	@echo "Usage:"
	@echo
	@echo "build/all    -> compile netactuator."
	@echo "install     -> install netactuator with interface Web."
	@echo "desinstalar  -> remove netactuator from system."
	@echo "clean        -> cleanup compile dir."
	@echo

build: all
all:
	@echo

	@echo "Compilando com a linha $(COMP) ..."
	@$(COMP)
	@sleep 1
	@echo "Pronto"
	@echo

install:
	@echo

	@echo "Instalando \"$(MAIN_OUTPUT)\" em \"$(INST_DEST)\""
	@mkdir -p $(INST_DEST)
	@cp -v $(MAIN_OUTPUT) $(INST_DEST)
	@sleep 1
	@echo "Pronto"
	@echo

	@echo "Instalando \"$(CONFIG_FILE)\" em \"$(INST_DEST_CONF)\""
	@mkdir -p $(INST_DEST_CONF)
	@cp -v $(CONFIG_FILE) $(INST_DEST_CONF)
	@sleep 1
	@echo "Pronto"
	@echo

	@echo "Instalando o sistema web do netactuator em \"$(INST_DEST_WEB)/web\""
	@mkdir -p $(INST_DEST_WEB)
	@cp -Rv web $(INST_DEST_WEB)
	@sleep 1
	@echo "Pronto"
	@echo

	@echo "Fixando permissões de $(INST_DEST)/$(MAIN_OUTPUT)"
	@chmod 555 $(INST_DEST)/$(MAIN_OUTPUT)
	@sleep 1
	@echo "Pronto"
	@echo

	@echo
	@echo "****************************************************************"
	@echo
	@echo "- Ajuste as configurações de netactuator.conf:"
	@echo "   # cp $(INST_DEST_CONF)/$(CONFIG_FILE) $(INST_DEST_CONF)/netactuator.conf"
	@echo
	@echo "- Utilize o script de inicialização do banco de dados para começar a usar o netactuator:"
	@echo "   # mysql < sql/netactuator-mysql.sql"
	@echo
	@echo "- Ajuste o seu httpd.conf:"
	@echo "   Alias /netactuator /usr/local/netactuator/web"
	@echo "   <Directory /usr/local/netactuator/web/>"
	@echo "       #AddDefaultCharset ISO-8859-1"
	@echo "       Options FollowSymLinks"
	@echo "       AllowOverride None"
	@echo "       Order deny,allow"
	@echo "       Allow from all"
	@echo "   </Directory>"
	@echo "- Ou utilize o arquivo fornecido:"
	@echo "   # cp netactuator-apache.conf-dist /etc/httpd/conf.d/netactuator-apache.conf"
	@echo
	@echo "- Altere os seguintes parâmetros no php.ini:"
	@echo "   output_buffering = On"
	@echo "   variables_order = \"EGPCS\""
	@echo "   register_globals = On"
	@echo "   session.auto_start = 1"
	@echo
	@echo "- Utilize o script de start/stop/status do netactuator:"
	@echo "   # ./scripts/netactuator.sh"
	@echo
	@echo "****************************************************************"
	@echo

desinstalar:
	@echo

	@echo "Removendo \"$(MAIN_OUTPUT)\" de \"$(INST_DEST)\""
	@rm -f $(INST_DEST)/$(MAIN_OUTPUT)
	@sleep 1
	@echo "Pronto"
	@echo
	@echo "****************************************************************"
	@echo
	@echo "Os arquivos de configuração serão mantidos em \"$(INST_DEST_CONF)\"."
	@echo "O diretório \"$(INST_DEST_WEB)\" foi mantido com suas bases históricas, remova-o manualmente, se desejar."
	@echo
	@echo "****************************************************************"
	@echo

clean:
	@echo

	@echo "Removendo binários e objetos locais de $(MAIN_OUTPUT) ..."
	@rm -f $(MAIN_OUTPUT)
	@rm -f $(MAIN_OUTPUT).o
	@rm -f $(MAIN_OUTPUT).core
	@sleep 1
	@echo "Pronto"
	@echo
