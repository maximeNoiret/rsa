CXX = g++
LDFLAGS = -lgmp -lgmpxx
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = rsa

$(TARGET): main.o terminalManagement.o rsa.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXGLAGS) -c $< -o $@

clean:
	rm *.o $(TARGET)
