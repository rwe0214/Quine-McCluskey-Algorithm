SRCDIR = src
OUTDIR ?= .out
CC	= gcc
CFLAGS	=-g -Wall

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OUTDIR)/%.o, $(SRCS)) 
EXE = hw1.o
INPUT = sample_input
OUTPUT = sample_output

all: $(EXE)

$(EXE) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ 

$(OUTDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
SHELL_HACK := $(shell mkdir -p $(OUTDIR))

.PHONY: all clean run debug OUT

format:
	clang-format -i src/*.c src/*.h

demo:
	./$(EXE) $(INPUT) $(OUTPUT)

debug:
	gdb -q $(EXE)

clean:
	rm -f $(OUTDIR)/* ./$(EXE)
	rmdir ./$(OUTDIR)