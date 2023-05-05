all: batch

clean:
	rm *.o batch

batch: \
	shell_batch.o \
	shell.o \
	dos_files.o \
	setup.o \
	support.o \
	programs.o \
	debug.o
	g++ -g -o batch \
		shell_batch.o \
		shell.o \
		dos_files.o \
		setup.o \
		support.o \
		programs.o \
		debug.o

shell.o:
	g++ -c -g shell.cpp

dos_files.o:
	g++ -c -g dos_files.cpp

setup.o:
	g++ -c -g setup.cpp

support.o:
	g++ -c -g support.cpp

programs.o:
	g++ -c -g programs.cpp

debug.o:
	g++ -c -g debug.cpp

shell_batch.o:
	g++ -c -g shell_batch.cpp
