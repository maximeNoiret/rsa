rsa: main.o terminalManagement.o rsa.o
	g++ -o rsa main.o terminalManagement.o rsa.o -lgmp -lgmpxx

main.o: main.cpp
	g++ -c main.cpp

rsa.o: rsa.cpp terminalManagement.o
	g++ -c rsa.cpp

terminalManagement.o: terminalManagement.cpp
	g++ -c terminalManagement.cpp

clean:
	rm *.o rsa
