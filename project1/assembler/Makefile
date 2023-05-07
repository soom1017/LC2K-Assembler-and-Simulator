CC = gcc
OBJS = assembler.o
TARGET = assembler
RESULTS = spec.mc mult.mc test1.mc test2.mc test3.mc test4.mc test5.mc
 
.SUFFIXES : .c .o
 
all : $(TARGET)
 
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)
 
clean :
	rm -f $(OBJS) $(TARGET)
	rm -f $(RESULTS)