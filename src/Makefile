CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		mengine.o shader.o

LIBS = -lglfw -lGLEW -lGL -lGLU

TARGET =	mengine.a
	
#$(TARGET):	$(OBJS)
#	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)
$(TARGET):
	g++ -o $(TARGET) $(LIBS) shader.cpp mengine.cpp


all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
