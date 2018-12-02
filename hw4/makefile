SRC=view.c control.c # change these to your source file names
SRCPLUS=model.cc manager.cc
HEADERS=control.h view.h # change these to your header file names
HEADERSCPLUS=manager.h model.h
LIBS=../hw2/libhw2.a  # you should use your hw2 code as a library
CFLAGS = -Wall -Wpedantic -std=c11 
IFLAGS =  -I../hw2 -I../jansson/include
GTKCFLAGS = `pkg-config --cflags gtk+-3.0`
LDFLAGS =  -L../hw2 -L../jansson/lib -lhw2 -l:libjansson.a
GTKLDFLAGS = `pkg-config --libs gtk+-3.0`

all: hw4
view.o: view.c $(HEADERS) $(LIBS)
	gcc $(IFLAGS) $(CFLAGS) $(GTKCFLAGS) -c $< -o $@ $(GTKLDFLAGS)
control.o: control.c $(HEADERS) $(LIBS) 
	gcc $(IFLAGS) $(CFLAGS) $(GTKCFLAGS) -c $< -o $@ $(GTKLDFLAGS)
model.o: model.cc $(HEADERS) $(HEADERSCPLUS) $(LIBS)
	g++ $(IFLAGS) $(GTKCFLAGS) -c $< -o $@ $(LDFLAGS) $(GTKLDFLAGS)
manager.o: manager.cc $(HEADERS) $(HEADERSCPLUS) $(LIBS)
	g++ $(IFLAGS) $(GTKCFLAGS) -c $< -o $@ $(LDFLAGS) $(GTKLDFLAGS)
hw4: model.o manager.o view.o control.o $(LIBS) 
	g++ -g $(GTKCFLAGS) -o  $@ $^ $(LDFLAGS) $(GTKLDFLAGS)
clean:
	rm -f *.o *~ hw4
