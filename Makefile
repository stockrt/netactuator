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
	@echo "deinstall  -> remove netactuator from system."
	@echo "clean        -> cleanup compile dir."
	@echo

build: all
all:
	@echo

	@echo "Compiling withe the line $(COMP) ..."
	@$(COMP)
	@sleep 1
	@echo "Done"
	@echo

install:
	@echo

	@echo "Installing \"$(MAIN_OUTPUT)\" in \"$(INST_DEST)\""
	@mkdir -p $(INST_DEST)
	@cp -v $(MAIN_OUTPUT) $(INST_DEST)
	@sleep 1
	@echo "Done"
	@echo

	@echo "Installing \"$(CONFIG_FILE)\" in \"$(INST_DEST_CONF)\""
	@mkdir -p $(INST_DEST_CONF)
	@cp -v $(CONFIG_FILE) $(INST_DEST_CONF)
	@sleep 1
	@echo "Done"
	@echo

	@echo "Installing web files into \"$(INST_DEST_WEB)/web\""
	@mkdir -p $(INST_DEST_WEB)
	@cp -Rv web $(INST_DEST_WEB)
	@sleep 1
	@echo "Done"
	@echo

	@echo "Fixing permissions of $(INST_DEST)/$(MAIN_OUTPUT)"
	@chmod 555 $(INST_DEST)/$(MAIN_OUTPUT)
	@sleep 1
	@echo "Done"
	@echo

	@echo
	@echo "****************************************************************"
	@echo
	@echo "- Copy and change setting for database for netactuator.conf:"
	@echo "   # cp $(INST_DEST_CONF)/$(CONFIG_FILE) $(INST_DEST_CONF)/netactuator.conf"
	@echo
	@echo "- Edit first the right user/pass/database in .sql and import it into MySQL:"
	@echo "   # mysql < sql/netactuator-mysql.sql"
	@echo
	@echo "- Add an alias for Apache in httpd.conf:"
	@echo "   Alias /netactuator /usr/local/netactuator/web"
	@echo "   <Directory /usr/local/netactuator/web/>"
	@echo "       #AddDefaultCharset ISO-8859-1"
	@echo "       Options FollowSymLinks"
	@echo "       AllowOverride None"
	@echo "       Order deny,allow"
	@echo "       Allow from all"
	@echo "   </Directory>"
	@echo "- or copy this setup to your Apache /etc/conf.d/ dir:"
	@echo "   # cp netactuator-apache.conf-dist /etc/httpd/conf.d/netactuator-apache.conf"
	@echo
	@echo "- Check php.ini and alter it's configuration to match those:"
	@echo "   output_buffering = On"
	@echo "   variables_order = \"EGPCS\""
	@echo "   register_globals = On"
	@echo "   session.auto_start = 1"
	@echo
	@echo "- Copy Netactuator script to your /etc/init.d/ dir"
	@echo "   # ./scripts/netactuator.sh"
	@echo
	@echo "****************************************************************"
	@echo

desinstall:
	@echo

	@echo "Removing \"$(MAIN_OUTPUT)\" of \"$(INST_DEST)\""
	@rm -f $(INST_DEST)/$(MAIN_OUTPUT)
	@sleep 1
	@echo "Done"
	@echo
	@echo "****************************************************************"
	@echo
	@echo "The configuration files are kept in \"$(INST_DEST_CONF)\"."
	@echo "Directory \"$(INST_DEST_WEB)\" was kept with its files/dirs, remove manually if desired so."
	@echo
	@echo "****************************************************************"
	@echo

clean:
	@echo

	@echo "Removing binary objects and locations from $(MAIN_OUTPUT) ..."
	@rm -f $(MAIN_OUTPUT)
	@rm -f $(MAIN_OUTPUT).o
	@rm -f $(MAIN_OUTPUT).core
	@sleep 1
	@echo "Done"
	@echo
