CC := g++
CFLAGS = -std=c++17 -g -O0 -Wall -Werror -Wextra -pedantic -fsanitize=address
LDFLAGS =
TARGET = maintest
SRCS   = main.cpp
OBJECTS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.depends)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

%.depends: %.c
	$(CC) -M $(CFLAGS) $< > $@

clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)

-include $(DEPS)
