#!/bin/bash

mkdir -p /usr/local/syschecker
cp -f syschecker /usr/local/syschecker/syschecker
chmod +x /usr/local/syschecker/syschecker
yes n | cp syschecker.cfg /usr/local/syschecker/
yes n | cp syschecker.cmd /usr/local/syschecker/
cp -f config.sh /usr/local/syschecker/config.sh

cp -f syschecker.init /etc/init.d/syschecker
chmod +x /etc/init.d/syschecker

chown nagios.nagios -R /usr/local/syschecker

/sbin/chkconfig --level 345 syschecker on


#echo "Renseigner l IP poller dans /usr/local/syschecker/syschecker.cfg"
#echo "Renseigner le nom d hote Nagios dans /usr/local/syschecker/syschecker.cmd"
echo "Executer /usr/local/syschecker/config.sh pour terminer"

#/etc/init.d/syschecker start
