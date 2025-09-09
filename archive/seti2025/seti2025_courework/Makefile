CXX = gcc
CXXFLAGS = -Wall -Wextra

all: server client

server: server.c
			$(CXX) $(CXXFLAGS) -o server server.c

client: client.c
	$(CXX) $(CXXFLAGS) -o client client.c -pthread

clean:
	rm -f server client 