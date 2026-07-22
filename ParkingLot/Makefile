CXX := g++
CXXFLAGS = -std=c++17 -MD -g -O0 -Wall -Werror -Wextra -pedantic -fsanitize=address
LDFLAGS =
TARGET = RunMe
SRCS   = main.cpp
OBJECTS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.depends)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.depends: %.cpp
	$(CXX) $(CXXFLAGS) $< > $@

clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET) *.d *.out RunMe

-include $(DEPS)
