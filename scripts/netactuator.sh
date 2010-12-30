#!/bin/bash
#
# start/stop/restart/status do netactuator
#
# chkconfig:   2345 85 15
# description: netactuator
#
# Author: Rogério Carvalho Schneider (stockrt@gmail.com)
# http://stockrt.github.com

PID=""
export LANG="en_US"
export LANGUAGE="en_US"
export LC_ALL="C"

getpid()
{
    PID=$(pgrep -f /usr/local/bin/netactuator)
}

start()
{
    getpid

    if [ "$PID" == "" ]
    then
        if [ -x /usr/local/bin/netactuator ]
        then
            /usr/local/bin/netactuator >> /var/log/netactuator.log 2>> /var/log/netactuator.log
            getpid
            echo "O netactuator foi iniciado sob o pid $PID"
        fi
    else
        echo "O netactuator já está rodando sob o pid $PID"
    fi
}

stop()
{
    getpid

    if [ "$PID" == "" ]
    then
        echo "O netactuator estava rodando?"
        pkill pmacctd > /dev/null 2>&1
        pkill pmacct > /dev/null 2>&1
        pkill netactuator > /dev/null 2>&1
    else
        echo "Encerrando o netactuator sob o pid $PID"
        pkill pmacctd > /dev/null 2>&1
        pkill pmacct > /dev/null 2>&1
        pkill netactuator > /dev/null 2>&1
        kill -9 $PID > /dev/null 2>&1
        echo "O netactuator foi finalizado"
    fi
}

restart()
{
    stop
    start
}

status()
{
    getpid

    if [ "$PID" == "" ]
    then
        echo "O netactuator não está rodando"
    else
        echo "O netactuator está rodando sob o pid $PID"
    fi
}

case "$1" in
    start)
        start
    ;;

    stop)
        stop
    ;;

    restart)
        stop
        start
    ;;

    status)
        status
    ;;

    *)
        echo "Modo de uso: `basename $0` {start stop restart status}" >&2
        exit 64
    ;;
esac

exit 0
