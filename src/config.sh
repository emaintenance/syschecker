#!/bin/bash

echo -n "Adresse du poller : "
read poller

echo -n "Nom de l'hote : "
read host

sed -i -e "s/=POLLER/=$poller/g" /usr/local/syschecker/syschecker.cfg
sed -i -e "s/HOSTNAME/$host/g" /usr/local/syschecker/syschecker.cmd
