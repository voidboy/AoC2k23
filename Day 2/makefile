INCDIRS=./inc/
SRCDIRS=./src
CC=gcc
OPTS=-O3
DEPFLAGS=-MP -MD
CFLAGS=-Wall -Wextra -ggdb $(STD) $(OPTS) $(foreach D, $(INCDIRS),-I$(D)) $(DEPFLAGS)
PROJECT=aoc
SOURCES=$(foreach D, $(SRCDIRS), $(wildcard $(D)/*.c))
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
DEPFILES=$(patsubst %.c,%.d,$(SOURCES))

all: $(PROJECT)

$(PROJECT): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(PROJECT) $(OBJECTS) $(DEPFILES)

-include $(DEPFILES)
