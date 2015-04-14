CPPFLAGS = -O3
LDFLAGS  = -lglog
all: Main
clean:
	rm -f *.o
Main: Main.o parserUtil.o torrentParser.o
	g++ -o $@ $^ $(LDFLAGS)
