all: doc
	scons
	
doc:
	doxygen Doxyfile > /dev/null	
	
clean:
	scons -c; rm doc/* -r
