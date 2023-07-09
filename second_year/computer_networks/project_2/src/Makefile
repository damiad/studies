CC = gcc
CFLAGS = -Wall -O2 -pthread

all: sikradio-sender sikradio-receiver

sikradio-sender: sikradio-sender.c
	$(CC) $(CFLAGS) -o sikradio-sender sikradio-sender.c

sikradio-receiver: sikradio-receiver.c
	$(CC) $(CFLAGS) -o sikradio-receiver sikradio-receiver.c

clean:
	rm -f sikradio-sender sikradio-receiver
