CC = gcc
CFLAGS = -Wall -O2

all: sikradio-sender sikradio-receiver

sikradio-sender: sikradio-sender.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o sikradio-sender sikradio-sender.c

sikradio-receiver: sikradio-receiver.c
	$(CC) $(CFLAGS) -pthread $(LDFLAGS) -o sikradio-receiver sikradio-receiver.c

clean:
	rm -f sikradio-sender sikradio-receiver
