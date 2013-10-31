OBJS = driver.o count.o

count : $(OBJS)
	cc -o count $(OBJS)

driver.o : driver.c count.c count.h

count.o : count.c count.h

.PHONY : clean
clean:
	rm count $(OBJS)
