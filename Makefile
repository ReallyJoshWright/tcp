TARGET := app

CXX := g++
STD := -std=c++23
DEBUG := -g -Wall -Wextra -pedantic
INCLUDE := -I./include

SRC := src/tcp_client.cpp src/tcp_client_async.cpp src/tcp_server.cpp \
	   src/tcp_server_async.cpp src/logger.cpp main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	@$(CXX) $(STD) $(DEBUG) $(INCLUDE) $(SRC) -o $(TARGET)

.PHONY: clean run

clean:
	@rm -rf $(TARGET)

run: $(TARGET)
	@./$(TARGET)
	@rm -rf $(TARGET)
