#include "Archiver.h"

int main(int argc, char * argv[])
{
	Archiver * arch = new Archiver;
	arch->startProgram(argc, argv);
	delete arch;

	return 0;
}