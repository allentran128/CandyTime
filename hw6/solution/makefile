SOCKDIR = ../socketCode
STD = -g -std=c++11
SRC=view.c serverControl.c # change these to your source file names
SRCPLUS=model.cc manager.cc thread.cc clientMessage.cc messageManager.cc
HEADERS=serverControl.h view.h # change these to your header file names
HEADERSCPLUS=manager.h thread.h controlTransfer.h clientMessage.h messageManager.h $(SOCKDIR)/CSE333.h $(SOCKDIR)/CSE333Socket.h serverMessage.h
OBJS = $(SOCKDIR)/CSE333Socket.o $(SOCKDIR)/Verify333.o $(SOCKDIR)/ClientSocket.o $(SOCKDIR)/ServerSocket.o
LIBS=../../hw2/libhw2.a  # you should use your hw2 code as a library
CFLAGS = -Wall -Wpedantic -std=c11 
IFLAGS =  -I../../hw2 -I../../jansson/include -I../socketCode
GTKCFLAGS = `pkg-config --cflags gtk+-3.0`
LDFLAGS =  -L../../hw2 -L../../jansson/lib -l:libjansson.a
GTKLDFLAGS = `pkg-config --libs gtk+-3.0`

all: client
#view.o: view.c $(HEADERS) $(LIBS)
#	gcc $(IFLAGS) $(CFLAGS) $(GTKCFLAGS) -c $< -o $@ $(GTKLDFLAGS)
#controlTransfer.o: controlTransfer.cc $(HEADERSCPLUS)
#	g++ $(STD) $(IFLAGS) $(GTKCFLAGS) -c $< -o $@ $(LDFLAGS) $(GTKLDFLAGS)
#serverControl.o: serverControl.c $(HEADERS) controlTransfer.h $(LIBS)  
#	gcc $(IFLAGS) $(CFLAGS) $(GTKCFLAGS) -c $< -o $@ $(GTKLDFLAGS)
model.o: model.cc $(HEADERS) $(HEADERSCPLUS) $(LIBS)
	g++ $(STD) $(IFLAGS) -c $< -o $@ $(LDFLAGS) 
thread.o: thread.cc $(HEADERS) $(HEADERSCPLUS) $(LIBS)
	g++ $(STD) $(IFLAGS) -c $< -o $@ $(LDFLAGS) 
#manager.o: thread.cc $(HEADERS) $(HEADERSCPLUS) $(LIBS)
#	g++ $(STD) $(IFLAGS) $(GTKCFLAGS) -c $< -o $@ $(LDFLAGS) $(GTKLDFLAGS)
manager.o: manager.cc $(HEADERS) $(HEADERSCPLUS) $(LIBS)
	g++ $(STD) $(IFLAGS) $(GTKCFLAGS) -c $< -o $@ $(LDFLAGS) $(GTKLDFLAGS)
messageManager.o: messageManager.cc $(HEADERS) $(HEADERSCPLUS) $(LIBS)
	g++ $(STD) $(IFLAGS) $(GTKCFLAGS) -c $< -o $@ $(LDFLAGS) $(GTKLDFLAGS)
clientMessage.o: clientMessage.cc $(HEADERS) $(HEADERSCPLUS) $(LIBS)
	g++ $(STD) $(IFLAGS) $(GTKCFLAGS) -c $< -o $@ $(LDFLAGS) $(GTKLDFLAGS)

client: model.o thread.o manager.o messageManager.o clientMessage.o $(LIBS)
	cd $(SOCKDIR) && make
	g++ $(STD) -g $(GTKCFLAGS) -o  $@ $^ $(LDFLAGS) $(GTKLDFLAGS) $(OBJS)

#serverMessage.o: serverMessage.cc $(HEADERS) $(HEADERSCPLUS) $(LIBS)
#	g++ $(STD) $(IFLAGS) $(GTKCFLAGS) -c $< -o $@ $(LDFLAGS) $(GTKLDFLAGS)

#serverMain.o: serverMain.cc $(HEADERSCPLUS)
#	g++  $(STD) $(IFLAGS) $(GTKCFLAGS) -c $< -o $@ $(LDFLAGS) $(GTKLDFLAGS)

#server: serverMessage.o manager.o view.o serverControl.o controlTransfer.o serverMain.o $(LIBS)
#	cd $(SOCKDIR) && make
#	g++ $(STD) $(IFLAGS) -g $(GTKCFLAGS) -o $@ $^ $(LDFLAGS) $(GTKLDFLAGS) $(OBJS)

test.o: test.cc thread.h manager.h $(LIBS)
	g++ $(STD) $(IFLAGS) -c $< -o $@

test: test.o thread.o manager.o $(LIBS)
	g++ $(STD) -o $@ $^
clean:
	rm -f *.o *~ client
