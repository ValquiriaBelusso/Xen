#!/bin/bash

# como compilar
# chmod +x mem.sh
# sh mem.sh

#---mem
mem_usage () {
#Porcentagem de memoria livre

date +"%T"

free -m | grep 'Mem' | awk '{print "Livre:" $4/$2*100}'

#memoria total
free -m | grep 'Mem' | awk '{print "Total:" $2}'

echo -e "\n"
}

./teste2 > mem.dat2 &

while : ; do
mem_usage
sleep 1
done
