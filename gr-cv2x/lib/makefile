CFLAGS = -std=c++11
CC = g++
 tester: tester.o SL_V2XCommunication.o
	$(CC) $(CFLAGS) -o tester tester.o SL_V2XCommunication.o
 tester.o: tester.cpp
	$(CC) $(CFLAGS) -c tester.cpp
 SL_V2XCommunication.o: SL_V2XCommunication.cpp
	$(CC) $(CFLAGS) -c SL_V2XCommunication.cpp
 clean:
	rm -f core *.o
