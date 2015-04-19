CPPFLAGS = -O3
LDFLAGS = -lglog 

all: Main

clean :
	rm -f *.o Main

Main: Main.o peer.o peer_queue.o
	g++ -o $@ $^ $(LDFLAGS)
