# makefile for scheduling program
#
# make rr_p - for round-robin scheduling
# make edf - for edf scheduling

CC=gcc
CFLAGS=-Wall -pthread

clean:
	rm -rf *.o
	rm -rf rr_p
	rm -rf edf

rr_p: driver.o list.o CPU.o schedule_rr_p.o
	$(CC) $(CFLAGS) -o rr driver.o schedule_rr_p.o list.o CPU.o

rr: driver.o list.o CPU.o schedule_rr.o
	$(CC) $(CFLAGS) -o rr driver.o schedule_rr.o list.o CPU.o

pa: driver.o list.o CPU.o schedule_pa.o timer.o
	$(CC) $(CFLAGS) -o pa driver.o schedule_pa.o list.o CPU.o timer.o -lpthread

edf: driver_edf.o list.o CPU.o schedule_edf.o timer.o
	$(CC) $(CFLAGS) -o edf driver_edf.o schedule_edf.o list.o CPU.o timer.o -lpthread

driver.o: driver.c
	$(CC) $(CFLAGS) -c driver.c

driver_edf.o: driver_edf.c
	$(CC) $(CFLAGS) -c driver_edf.c

schedule_pa.o: schedule_pa.c
	$(CC) $(CFLAGS) -c schedule_pa.c


schedule_rr_p.o: schedule_rr_p.c
	$(CC) $(CFLAGS) -c schedule_rr_p.c

schedule_edf.o: schedule_edf.c
	$(CC) $(CFLAGS) -c schedule_edf.c


schedule_rr.o: schedule_rr.c
	$(CC) $(CFLAGS) -c schedule_rr.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

CPU.o: CPU.c CPU.h
	$(CC) $(CFLAGS) -c CPU.c

timer.o: timer.c timer.h
	$(CC) $(CFLAGS) -c timer.c