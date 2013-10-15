CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		mengine.o

LIBS =

TARGET =	mengine.a


force_look :
	true

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) src/$(OBJS) $(LIBS)
	
mengine.o:	force_look
	cd src; make 

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
