SRC=view.c control.c model.c # change these to your source file names
HEADERS= control.h view.h model.h # change these to your header file names
I  # you should use your hw2 code as a library
CFLAGS = -Wall -Wpedantic -std=c11 
IFLAGS =  -I../hw2 -I../jansson/include
GTKCFLAGS = `pkg-config --cflags gtk+-3.0`
LDFLAGS =  -L../hw2 -L../jansson/lib -lhw2 -l:libjansson.a
GTKLDFLAGS = `pkg-config --libs gtk+-3.0`

all: hw3

hw3: $(SRC) $(HEADERS) $(LIBS)
	gcc $(CFLAGS) -g -O0 $(IFLAGS) $(GTKCFLAGS) -o hw3 $(SRC) $(LDFLAGS) $(GTKLDFLAGS)

clean:
	rm -f *.o *~ hw3
