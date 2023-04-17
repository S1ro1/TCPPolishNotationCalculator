CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++2a

SRCS = $(wildcard *.cpp)
OBJS := $(SRCS:%.cpp=%.o)
DEPS := $(SRCS:%.cpp=%.d)

TARGET = ipkcpd

.phony: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp %.d %.h
	$(CXX) -MT $@ -MMD -MP -MF $*.d $(CFLAGS) $(CXXFLAGS) -c $(OUTPUT_OPTION) $<

$(DEPS):
include $(wildcard $(DEPS))

clean:
	rm -f $(OBJS) $(TARGET) $(DEPS)
