CC = gcc
CFLAGS = -Wall -Wextra 
TARGET = binary_search
SRCDIR = src

all: run

$(TARGET): $(SRCDIR)/main.c
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCDIR)/main.c

run: $(TARGET)
	./$(TARGET)
	gnuplot $(SRCDIR)/plot.gp

clean:
	rm -f $(TARGET) data.dat binary_search_plot.png
