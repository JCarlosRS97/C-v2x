CFLAGS = -std=c++14
CC = g++
 tester: tester.o SL_V2XCommunication.o SL_V2XConfig.o
	$(CC) $(CFLAGS) -o tester tester.o SL_V2XCommunication.o
 tester.o: tester.cpp
	$(CC) $(CFLAGS) -c tester.cpp
 SL_V2XCommunication.o: SL_V2XCommunication.cpp
	$(CC) $(CFLAGS) -c SL_V2XCommunication.cpp
 SL_V2XConfig.o: SL_V2XConfig.cpp
	$(CC) $(CFLAGS) -c SL_V2XConfig.cpp
 clean:
	rm -f core *.o
