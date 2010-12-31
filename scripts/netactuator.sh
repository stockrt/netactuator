#!/bin/bash
#
# start/stop/restart/status do netactuator
#
# chkconfig:   2345 85 15
# description: netactuator
#
# Author: Rogério Carvalho Schneider (stockrt@gmail.com)
# http://stockrt.github.com
# Translation: Peter Okupski okupski@widzew.net

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
            /usr/local/bin/netactuator >> /var/log/netactuator.log 2>&1
            getpid
            echo "Netactuator has started with pid $PID"
        fi
    else
        echo "Netactuator is already running with pid $PID"
    fi
}

stop()
{
    getpid

    if [ "$PID" == "" ]
    then
        echo "No PID for Netactuator making sure it's not running"
        pkill pmacctd > /dev/null 2>&1
        pkill pmacct > /dev/null 2>&1
        pkill netactuator > /dev/null 2>&1
    else
        echo "Closing Netactuator with pid $PID"
        pkill pmacctd > /dev/null 2>&1
        pkill pmacct > /dev/null 2>&1
        pkill netactuator > /dev/null 2>&1
        kill -9 $PID > /dev/null 2>&1
        echo "Netactuator was shutdown"
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
        echo "Netactuator is not running"
    else
        echo "Netactuator is running with pid $PID"
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
        echo "Usage: `basename $0` {start stop restart status}" >&2
        exit 64
    ;;
esac

exit 0
