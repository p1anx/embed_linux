#!/bin/bash
function send(){
    local FILE=$1
    local HOST="root@192.168.1.44"
    rsync -av --info=progress2 $FILE "$HOST:/home/pi"
    echo "=======file is sent======="
}
send $1
