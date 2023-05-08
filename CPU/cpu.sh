#!/bin/bash

# como compilar
# chmod + cpu.sh
# sh cpu.sh

#cpu use threshold
cpu_threshold='80'


#---cpu
cpu_usage () {
cpu_idle=`top -b -n 1 | grep 'Cpu' | awk '{print $8}'|cut -f 1 -d ","`

if [ $cpu_idle = "id" ]
   then cpu_idle=0
fi

cpu_use=`expr 100 - $cpu_idle`
cpu_online=`nproc`
ncpu=`expr $cpu_online + 1`

echo "$(date +%F_%T) USE: $cpu_use CPUS: $ncpu" >> cpu_log.dat

echo "cpu utilization: $cpu_use"
echo "cpus online: $cpu_online" 


if [ $cpu_use -gt $cpu_threshold ]
    then
        echo "cpu warning!"
        sudo xenstore-write -s /local/domain/3/cpu/$ncpu/availability online 
                
    else
        echo "cpu ok!"
fi
}


