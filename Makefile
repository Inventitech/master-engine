CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		mengine.a

LIBS = -lglfw -lGLEW -lGL -lGLU

TARGET =	mengine


force_look :
	true

$(TARGET):	$(OBJS)
	cp src/mengine.a mengine
	#$(CXX) -o $(TARGET) src/$(OBJS) $(LIBS)

mengine.a:	force_look
	cd src; make

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
