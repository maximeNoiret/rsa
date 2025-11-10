CXX = g++
LDFLAGS = -lgmp -lgmpxx -pthread
CXXFLAGS = -Wall -Wextra -std=c++11
COMPSCRIPT = rsa
TARGET = rsa
OBJECTS = main.o rsa.o terminalManagement.o 

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

windows: $(OBJECTS)
	$(CXX) -static $(CXXFLAGS) -o $(TARGET).exe $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)

uninstall:
	rm -f /usr/local/bin/$(TARGET)
	rm /usr/share/bash-completion/completions/$(COMPSCRIPT)

install:
	install -m 755 $(TARGET) /usr/local/bin/
	cp completions/$(COMPSCRIPT) /usr/share/bash-completion/completions/
	. completions/$(COMPSCRIPT)
