all: 
	doxygen Doxyfile > /dev/null ; scons
	
clean:
	scons -c; rm doc/* -r
