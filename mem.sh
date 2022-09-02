#!/bin/bash

# como compilar
# chmod + mem.sh
# sh mem.sh

#cpu use threshold
cpu_threshold='80'


#---mem
mem_usage () {
#Porcentagem de memoria livre
mem_free=`free -m | grep 'Mem' | awk '{print $4/$2*100}'`
#memoria total
mem_total=`free -m | grep 'Mem' | awk '{print $2}'`
}


while : ; do
mem_usage
sleep 50
done
