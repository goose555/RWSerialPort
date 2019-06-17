#include "RWSerialPort.h"

int main()
{
	RWSerialPort arduino(COM3);
	arduino.redSerialPort();

	return 0;
}
