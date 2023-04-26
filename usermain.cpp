#include <dos.h>
#include "system.h"

int userMain(int argc, char* argv[]);


int main(int argc, char* argv[]){
	System::turnOn();
	int value = userMain(argc, argv);
	System::turnOff();
	return value;
}


