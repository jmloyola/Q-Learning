ejemplo: ejemplo.o apstring.o serial.o kRobot.o
	g++ ejemplo.o apstring.o serial.o kRobot.o -o ejemplo
ejemplo.o: ejemplo.cpp
	g++ ejemplo.cpp -c
apstring.o: apstring.cpp apstring.h
	g++ apstring.cpp -c
serial.o: serial.cpp serial.h
	g++ serial.cpp -c
kRobot.o: kRobot.cpp kRobot.h
	g++ kRobot.cpp -c
