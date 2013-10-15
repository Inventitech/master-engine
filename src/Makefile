CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		mengine.o

LIBS = -lglfw -lGLEW -lGL -lGLU

TARGET =	mengine.a

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
