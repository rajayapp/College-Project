all: final
final:Module_1.o Module_2.o Module_3.o Module_4.o Module_5.o Prosper.o
	g++ Module_1.o Module_2.o Module_3.o Module_4.o Module_5.o Prosper.o -o final
Module_1.o:Module_1.cpp
	g++ -c Module_1.cpp
Module_2.o:Module_2.cpp
	g++ -c Module_2.cpp
Module_3.o:Module_3.cpp
	g++ -c Module_3.cpp
Module_4.o:Module_4.cpp
	g++ -std=c++11 -c Module_4.cpp
Module_5.o:Module_5.cpp
	g++ -c Module_5.cpp
Prosper.o: Prosper.cpp
	g++ -c Prosper.cpp
clean:
	rm -rf *o final

