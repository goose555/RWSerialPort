#include "RWSerialPort.h"

int main()
{

	RWSerialPort arduino(COM3);


	while (1)
	{
		arduino.readSerialPort();
	}

	return 0;
}
