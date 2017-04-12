DIR_SRC = ./
DIR_INC = ./
DIR_OBJ = ./obj

OBJECTS = $(DIR_OBJ)/log.o \
          $(DIR_OBJ)/main.o \
		  $(DIR_OBJ)/multicastsvr.o \
		  $(DIR_OBJ)/throughput.o \
		  $(DIR_OBJ)/relativetime.o \
		  $(DIR_OBJ)/iniopr.o

BIN_TARGET = udpfilesent

CC = gcc

CFLAGS = -g -Wall -lrt -lpthread -I$(DIR_INC)

$(BIN_TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BIN_TARGET)

$(DIR_OBJ)/%.o : $(DIR_SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean
clean :
	find $(DIR_OBJ) -name *.o -exec rm {} \;
	find . -name $(BIN_TARGET) -exec rm {} \;
