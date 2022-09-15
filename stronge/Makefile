CROSS_COMPOLE=arm-linux-
CC=$(CROSS_COMPOLE)gcc
CFLAGS= -c -g
LDFLAGS= -lpthread

OBJS=main.o data_global.o pthread_transfer.o pthread_refresh.o\
	 pthread_client_request.o pthread_sqlite.o\
	 pthread_buzzer.o pthread_led.o pthread_seg.o pthread_fun.o

monitor_obj :$(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	mv *o ./obj
$(OBJS):%.o:%.c
	$(CC) $(CFLAGS) $< -o $@

install:
	sudo cp monitor_obj /opt/4412/rootfs/ 

.PHONY:clean
clean:
	rm *.o monitor_obj -rf 
