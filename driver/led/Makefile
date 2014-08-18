KERNELDIR = /home/zuokong/bb/kernel/kernel
PWD := $(shell pwd)

obj-m := bb_led.o

modules:
	make -C $(KERNELDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-

clean:
	rm -rf *.ko *.order *.symvers *.cmd *.o *.mod.c *.tmp_versions


