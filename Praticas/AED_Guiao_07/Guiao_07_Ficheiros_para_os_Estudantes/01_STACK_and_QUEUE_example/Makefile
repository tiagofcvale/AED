# To compile all programs, run:
#   make
# 
# JMR 2021, JMadeira 2023


# Options for the C compiler. 
CFLAGS += -g -Wall -Wextra

TARGETS = 01_example

all: $(TARGETS)

01_example.o: 01_example.c IntegersQueue.h IntegersStack.h

IntegersQueue.o: IntegersQueue.c IntegersQueue.h

IntegersStack.o: IntegersStack.c IntegersStack.h

01_example: 01_example.o IntegersQueue.o IntegersStack.o

clean:
	rm -f *.o
	rm -f $(TARGETS)
