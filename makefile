TARGET = connected
C_SOURCES = connect.cpp
C_OBJS = connect.o
CCX = g++
CXXFLAGS = -g -std=c++11

all: $(TARGET)

$(TARGET): $(C_SOURCES)
	$(CCX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(TARGET) $(C_OBJS) core

tar:
	tar cvf $(TARGET).tar $(C_SOURCES) Makefile

$(TARGET).ps: $(C_SOURCES)
	enscript -p code.ps $(C_SOURCES)
