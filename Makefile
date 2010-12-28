GCC=/usr/bin/gcc

MAIN_INPUT=netactuator.c
MAIN_OUTPUT=netactuator
CONFIG_FILE=netactuator.conf-dist
MYSQL_LIBS=`mysql_config --libs`
COMP=$(GCC) -o $(MAIN_OUTPUT) -g $(MAIN_INPUT) $(MYSQL_LIBS) -lpthread -L/usr/local/lib -lrrd
INST_DEST=/usr/local/bin
INST_DEST_CONF=/usr/local/etc/netactuator
INST_DEST_WEB=/usr/local/netactuator

default:
	@echo

	@echo "As opções para make são:"
	@echo
	@echo "build/all	-> Compila o netactuator"
	@echo "instalar	-> Instala o netactuator (depois de compilado). Binário e interface Web"
	@echo "desinstalar	-> Desinstala o netactuator"
	@echo "clean		-> Limpa o workdir e remove o binário gerado do diretório atual"
	@echo

build: all
all:
	@echo

	@echo "Compilando com a linha $(COMP) ..."
	@$(COMP)
	@sleep 1
	@echo "Pronto"
	@echo

instalar:
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
	@echo "**********************************************************************************************************"
	@echo
	@echo "- Ajuste as configurações de netactuator.conf:"
	@echo "   # cp $(INST_DEST_CONF)/$(CONFIG_FILE) $(INST_DEST_CONF)/netactuator.conf"
	@echo
	@echo "- Utilize o script de inicialização do banco de dados para começar a usar o netactuator:"
	@echo "   # mysql < sql/netactuator-mysql.sql"
	@echo
	@echo "- Ajuste o seu httpd.conf:"
	@echo "   Alias /netactuator /usr/local/netactuator/web"
	@echo "   <Directory /usr/local/netactuator/web>"
	@echo "       AddDefaultCharset ISO-8859-1"
	@echo "       Options None"
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
	@echo "**********************************************************************************************************"
	@echo

desinstalar:
	@echo

	@echo "Removendo \"$(MAIN_OUTPUT)\" de \"$(INST_DEST)\""
	@rm -f $(INST_DEST)/$(MAIN_OUTPUT)
	@sleep 1
	@echo "Pronto"
	@echo
	@echo "**********************************************************************************************************"
	@echo
	@echo "Os arquivos de configuração serão mantidos em \"$(INST_DEST_CONF)\"."
	@echo "O diretório \"$(INST_DEST_WEB)\" foi mantido com suas bases históricas, remova-o manualmente, se desejar."
	@echo
	@echo "**********************************************************************************************************"
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
