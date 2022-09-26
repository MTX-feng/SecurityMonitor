
SRCFILE=app-zigbeea9.c
APP=zigbeefantest

NFSDIR=/opt/4412/rootfs

all:
	arm-none-linux-gnueabi-gcc $(SRCFILE) -o $(APP)
install:
		cp $(APP) $(NFSDIR)
clean:
	rm $(APP)
