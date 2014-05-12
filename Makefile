cpp_server_robots: cpp_server_robots.o apstring.o serial.o kRobot.o 
	g++ cpp_server_robots.o apstring.o serial.o kRobot.o -o cpp_server_robots
cpp_server_robots.o: cpp_server_robots.cpp
	g++ cpp_server_robots.cpp -c
apstring.o: apstring.cpp apstring.h
	g++ apstring.cpp -c
serial.o: serial.cpp serial.h
	g++ serial.cpp -c
kRobot.o: kRobot.cpp kRobot.h
	g++ kRobot.cpp -c
