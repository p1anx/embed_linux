# This is cross-compile

# Usage
1. To compiple the file and sent to pi
```bash
make 
gcc mainApp.c -o mainApp
#then send the `.ko` and `mainApp` file to pi, and
sudo insmod mod_auto_cdev
```
2. To check if the file `/dev/hello_test0` exists in `/dev` 

3. Send message to `/dev/hello_test0`

```bash 
echo "hello world" | sudo tee /dev/hello_test0 
sudo ./mainApp
```
