## board
raspberry pi 5b
## linux version
6.6.51+rpt-rpi-2712
## Usage with natively compiling
- use  command `make` to create `.dtbo` file by `Makefile`
- `make clean` command is applied to remove `.dtbo` file
- `sudo cp foo.dtbo /boot/firmware/overlays/`
- add `dtoverlay=myled3` to the tail of `/boot/firmware/config.txt` file
- check device node in `/proc/device-tree` directory using `ls /proc/device-tree`. If myled exists, it's no problem.

## further
- `sudo make install` command can add `foo.dtbo` to `/boot/firmware/overlays/` and add `dtoverlay=myled` to the tail of `/boot/firmware/config.txt` file
