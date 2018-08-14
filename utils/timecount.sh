#!/bin/bash

#read ip from iplist and ping to check it's alive or not
ipfile="/tmp/iplist"
macfile="/tmp/maclist.txt"
loop=1
ipaddr=localhost
init_text="working_minutes
0
working_hrs
0"
reset_time=0
function monday_check(){
  dayofweek=`date +"%w"`
  hour=`date +"%H"`
  minute=$(date +"%M")
  if [ "$dayofweek" = "1" ] && [ "$hour" = "0" ] && [ $minute -lt 15 ]; then
          reset_time=1
         #
  fi
#  reset_time=1
}


function check_all_ips(){
  echo "123"
  echo $ipfile
  while IFS= read -r var 
    do
      echo "check ip $var"
      filename="/tmp/${var}.txt"
      echo "filename=$filename"
      #create file with ip from iplist if it wasnt created yet
      echo "reset_time=$reset_time"
      if [ ! -f "$filename" ] || [ "$reset_time" = 1 ]; then
        echo "File not found!"
        touch "$filename"
        echo "$init_text" >  "$filename"
        # echo "working_minutes" > $filename
        # echo "0" > $filename
        # echo "working_hrs" > $filename
        # echo "0" > $filename

      fi
      #set ip before check
      ip=$var
      check_alive
    done <"$ipfile"
}

function check_all_mac(){
  echo $macfile
  while IFS= read -r mac 
    do
      echo "check mac addr $mac"
      filename="/tmp/${mac}.txt"
      echo "filename=$filename"
      #create file with ip from iplist if it wasnt created yet
      echo "reset_time=$reset_time"
      if [ ! -f "$filename" ] || [ "$reset_time" = 1 ]; then
        echo "File not found!"
        touch "$filename"
        echo "$init_text" >  "$filename"
      
      fi
      #set ip before check
      ipaddr=$(arp | grep "$mac" | awk ' { print $1 } ')
      echo "get ip from $mac : $ipaddr"
      check_alive
    done <"$macfile"
}
#ping the ip and if success increase time
function check_alive(){
    echo "checking if ip $ip is alive "

    ping -c 1 $ipaddr                      # Try once.
    rc=$?
    filename="/tmp/${mac}.txt"
    echo "file name contain timesheet is $filename"
    if [[ $rc -eq 0 ]] ; then
        #read current minutes from the timesheet file
        #read  2nd line
         working_minutes=$(sed -n '2p' "$filename")
          #$(awk 'NR==2 ' "$filename") 
         echo "working_minutes=$working_minutes" 

 	       working_minutes=$((working_minutes+10))
         echo "working_minutes=$working_minutes" 
	       working_hrs=$((working_minutes/60))
	       echo "working_hrs=$working_hrs"
	       # echo "working_times " > filename
        #  echo "$working_minutes" >> filename
        #  echo "working_hrs " >> filename
        #  echo "$working_hrs" >> filename
        echo "working_minutes
$working_minutes
working_hrs
$working_hrs" > $filename
    fi;
    #sleep 60
}


while ((loop = 1))
do       
        reset_time=0
        monday_check
        #check_all_ips
        check_all_mac
        echo "sleep for 10minutes"
        sleep 1m
done
