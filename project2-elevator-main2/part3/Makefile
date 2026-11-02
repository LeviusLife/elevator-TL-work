ifneq ($(KERNELRELEASE),)
obj-m += elevator.o
elevator-y := src/elevator_main.o src/pet_list.o

else

KDIR ?= /usr/src/linux-6.16.12
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

endif


