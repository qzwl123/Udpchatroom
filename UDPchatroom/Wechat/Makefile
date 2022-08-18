CXX = g++
CFLAGS = -std=c++14 -O2

TARGET = run

OBJS = ../common/*.c ./*.c

aii: $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) -I ../common/  -pthread  -lcurses

clean:
	rm -rf ./$(OBJS) $(TARGET)
