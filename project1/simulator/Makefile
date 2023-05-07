CC = gcc
OBJS = simulate.o
TARGET = simulate
RESULTS = spec.out mult.out test1.out test2.out test3.out test4.out test5.out
 
.SUFFIXES : .c .o
 
all : $(TARGET)
 
$(TARGET): $(OBJS)
	   $(CC) -o $@ $(OBJS)
 
clean :
	rm -f $(OBJS) $(TARGET)
	rm -rf $(RESULTS)
