#!/bin/bash
###################################################################
#File Name      : install.sh
#Description    : automated installation of InfluxDB.
#Args           :
#Author         : Yves Wetter, Ricardo Bras
#Email          : yves.wetter@edu.tbz.ch, ricardo.bras@edu.tbz.ch
###################################################################
#default settings
    sudo apt-get update && sudo apt-get upgrade
    sudo apt-get -y install debconf-utils ufw
# installation
    sudo su
    echo "deb https://repos.influxdata.com/ubuntu bionic stable" | sudo tee /etc/apt/sources.list.d/influxdb.list
    sudo curl -sL https://repos.influxdata.com/influxdb.key | sudo apt-key add -
    sudo apt-get install -y influxdb influxdb-client
    cp /mnt/influxdb.conf /etc/influxdb/influxdb.conf
    influxd -config /etc/influxdb/influxdb.conf
    sudo systemctl enable --now influxdb
    sudo systemctl is-enabled influxdb
    sudo systemctl start influxdb
    head /dev/urandom | tr -dc A-Za-z0-9 | head -c 30 > /mnt/influxdb_iotkit.txt
    head /dev/urandom | tr -dc A-Za-z0-9 | head -c 30 > /mnt/influxdb_superuser.txt
    dbiotkit=$(cat /mnt/influxdb_iotkit.txt)
    dbsuperuser=$(cat /mnt/influxdb_superuser.txt)
    curl -XPOST "http://localhost:8086/query" --data-urlencode "q=CREATE USER \ iotkit WITH PASSWORD 'supersecure22' WITH ALL PRIVILEGES"
#generate selfSigned Cert
    ip=$(ip -4 addr show enp0s8 | grep -oP '(?<=inet\s)\d+(\.\d+){3}')
    sudo openssl req -x509 -nodes -newkey rsa:2048 \
    -keyout /etc/ssl/influxdb-selfsigned.key \
    -out /etc/ssl/influxdb-selfsigned.crt \
    -subj "/C=CH/ST=private/L=private/O=Zuerich/CN=$ip" \
    -days 3650
    sudo chown influxdb:influxdb /etc/ssl/influxdb*
    sudo chmod 644 /etc/ssl/influxdb-selfsigned.crt
    sudo chmod 600 /etc/ssl/influxdb-selfsigned.key
    sudo systemctl restart influxdb
    sudo cp /etc/ssl/influxdb-selfsigned.crt /mnt
#DB settings - create db, admin- and iot-user
    sleep 10
    sudo influx -precision rfc3339 -unsafeSsl -ssl  -execute "CREATE USER superuser WITH PASSWORD '$dbsuperuser' WITH ALL PRIVILEGES"
    sudo influx -precision rfc3339 -unsafeSsl -ssl -username "superuser" -password "$dbsuperuser" -execute "CREATE USER iotkit WITH PASSWORD '$dbiotkit' "
    sudo influx -precision rfc3339 -unsafeSsl -ssl -username "superuser" -password "$dbsuperuser" -execute "CREATE DATABASE m242"
    sudo influx -precision rfc3339 -unsafeSsl -ssl -username "superuser" -password "$dbsuperuser" -execute "GRANT WRITE ON "m242" TO "iotkit" "
# Firewall
    sudo ufw default deny incoming
    sudo ufw allow 8086/tcp
    #sudo ufw allow 8088
    sudo ufw allow 22/tcp
    sudo ufw -f enable