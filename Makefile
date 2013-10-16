clean:
	rm -f mengine;

all:	
	cd src; scons; mv mengine ../mengine
	