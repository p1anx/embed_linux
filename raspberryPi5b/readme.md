# send.sh

The script is applied to send the file from local to host.

```bash
#!/bin/bash
function send(){
    local FILE=$1
    local HOST="root@192.168.1.39"
    sudo rsync -av $FILE "$HOST:/home/neox/code"
    echo "=======file is sent======="
}
send $1

```
