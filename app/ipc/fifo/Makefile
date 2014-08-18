EXEC1	= rdfifo
OBJS1	= rdfifo.o
EXEC2	= wrfifo
OBJS2	= wrfifo.o

CROSS	= arm-linux-gnueabihf-
CC		= $(CROSS)gcc 
STRIP	= $(CROSS)strip
CFLAGS	= -Wall -g -O2

all:  clean $(EXEC1) $(EXEC2)

$(EXEC1):$(OBJS1)
	$(CC) $(CFLAGS) -o $@ $(OBJS1)
	$(STRIP) $@

$(EXEC2):$(OBJS2)
	$(CC) $(CFLAGS) -o $@ $(OBJS2)
	$(STRIP) $@

clean:
	-rm -f $(EXEC1) *.o
	-rm -f $(EXEC2) *.o
