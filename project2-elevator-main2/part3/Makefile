ifneq ($(KERNELRELEASE),)
obj-m += elevator.o
elevator-y := src/elevator_main.o src/pet_list.o

else

KDIR ?= /lib/modules/$(shell uname -r)/builld
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

endif


