KBUILD_CFLAGS += -w
obj-m += simple.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
unload:
	sudo rmmod simple
load:
	sudo insmod simple.ko
clear:
	sudo dmesg -c
view:
	dmesg
