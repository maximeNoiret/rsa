CXX = g++
LDFLAGS = -lgmp -lgmpxx -pthread
CXXFLAGS = -Wall -Wextra -std=c++11
TARGET = rsa

$(TARGET): main.o terminalManagement.o rsa.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)

uninstall:
	rm -f /usr/local/bin/$(TARGET)

install:
	install -m 755 $(TARGET) /usr/local/bin/
