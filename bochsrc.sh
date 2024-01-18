###############################################################
# Configuration file for Bochs
###############################################################

# how many memory the machine will have
megs: 32

# filename of ROM images
romimage: file=$BXSHARE/BIOS-bochs-latest, options=fastboot
vgaromimage: file=$BXSHARE/VGABIOS-lgpl-latest

# what disk images will be used 

# floppya: 1_44 = D:\Code\C\OS\oranges-master\chapter10\e\a.img, status=inserted
floppya: 1_44 = D:\Code\C\OS\Final\a.img, status=inserted
boot: a

# choose the boot disk
# boot: floppy

# where do we send log messages?
log: others/bochsout.txt

# disable the mouse 
mouse: enabled = 0 

keyboard: keymap=$BXSHARE/keymaps/x11-pc-us.map

# clock: sync=realtime, rtc_sync=1

magic_break: enabled=1

# hard disk
ata0: enabled=1, ioaddr1=0x1f0, ioaddr2=0x3f0, irq=14
# !! Remember to change these if the hd img is changed:
#    1. include/sys/config.h::MINOR_BOOT
#    2. boot/include/load.inc::ROOT_BASE
#    3. Makefile::HD
#    4. commands/Makefile::HD
ata0-master: type=disk, path="80m.img", mode=flat, cylinders=162, heads=16, spt=63
# D:\Code\C\OS\oranges-master\chapter10\e\

# romimage: file=$BXSHARE/BIOS-bochs-latest, options=fastboot
# vgaromimage: file=$BXSHARE/VGABIOS-lgpl-latest
# keyboard: keymap=$BXSHARE/keymaps/x11-pc-us.map
# floppya: 1_44 = D:\Code\C\OS\oranges-master\chapter10\e\a.img, status=inserted
