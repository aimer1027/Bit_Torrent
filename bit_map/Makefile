CPPFLAGS = -O3
LDFLAGS  = -lglog 

all:Main

clean :
	rm -f *.o Main
Main :Main.o bit_map.o
	g++ -o $@ $^ $(LDFLAGS) 
