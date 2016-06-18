obj-m += irq_disabler.o

all:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules

clean:
	make -C /lib/modules/`uname -r`/build M=`pwd` clean

install:
	cp irq_disabler.ko /lib/modules/`uname -r`/updates/dkms
	depmod -a
	modprobe irq_disabler
	#cp rc.local /etc/rc.local
