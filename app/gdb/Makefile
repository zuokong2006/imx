EXEC	= gdb_test
OBJS    = gdb_test.o

CROSS	= arm-linux-gnueabihf-
CC		= $(CROSS)gcc 
STRIP	= $(CROSS)strip
CFLAGS	= -Wall -g -O2

all:  clean $(EXEC)

$(EXEC):$(OBJS)
	$(CC) -g -o $@ $(OBJS)
	#$(CC) $(CFLAGS) -g -o $@ $(OBJS)
	#$(STRIP) $@

clean:
	-rm -f $(EXEC) *.o
