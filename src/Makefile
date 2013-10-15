CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		mengine.o

LIBS =

TARGET =	mengine.a

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
