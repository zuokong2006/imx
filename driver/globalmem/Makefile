KERNELDIR = /home/zuokong/imx/kernel/kernel
PWD := $(shell pwd)

obj-m := globalmem.o

modules:
	make -C $(KERNELDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-fsl-linux-gnueabi-

clean:
	rm -rf *.ko *.order *.symvers *.cmd *.o *.mod.c *.tmp_versions


