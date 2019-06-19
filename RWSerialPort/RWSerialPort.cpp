#include "RWSerialPort.h"
#include <string>
RWSerialPort::RWSerialPort(LPCTSTR portName)
{
	errno = 0;

	hComm = CreateFile(portName, 
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hComm == INVALID_HANDLE_VALUE){
		errno = static_cast<int>(GetLastError());
		std::cerr << "Error read: " << strerror_s(bufError, sizeof(bufError), errno) << std::endl;
	}else
		std::cout << "Opening serial port successful" << std::endl;

	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	Status = GetCommState(hComm, &dcbSerialParams); //ѕолучение настроек последовательного порта
	//«начени€ дл€ скорости в бодах, размера байта, количества начальных / конечных битов
	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

	//Ќастройка порта в соответствии со структурой DCB
	SetCommState(hComm, &dcbSerialParams);

	//Ќастройка тайм-аутов
	COMMTIMEOUTS timeouts = { 0 };
	
	timeouts.ReadIntervalTimeout = 50; 
	/*
	ќпредел€ет максимальный интервал времени между поступлением двух
	байтов. ≈сли врем€ прибыти€ превышает эти пределы, функци€
	ReadFile () возвращаетс€.
	*/

	timeouts.ReadTotalTimeoutConstant = 50;
	/*
	»спользуетс€ дл€ вычислени€ общего периода ожидани€ дл€ операций 
	чтени€. ƒл€ каждой операции чтени€ это значение добавл€етс€ к 
	продукту члена ReadTotalTimeoutMultiplier и запрошенному количеству 
	байтов.
	*/
	
	timeouts.ReadTotalTimeoutMultiplier = 10;
	/*
	»спользуетс€ дл€ вычислени€ общего периода ожидани€ дл€ операций 
	чтени€. ƒл€ каждой операции чтени€ это значение умножаетс€ на 
	запрошенное количество байтов дл€ чтени€.
	*/

	timeouts.WriteTotalTimeoutConstant = 50;
	/*
	ѕохож на ReadTotalTimeoutConstant, но дл€ операции записи.
	*/

	timeouts.WriteTotalTimeoutMultiplier = 10;
	/*
	Ќалогичный ReadTotalTimeoutMultiplier, но дл€ операции записи.
	*/
}

void RWSerialPort::readSerialPort(void) {
	Status = SetCommMask(hComm, EV_RXCHAR);
	Status = WaitCommEvent(hComm, &dwEventMask, NULL);
	int i = 0;

	do
	{
		ReadFile(hComm,
			&tempChar,
			sizeof(tempChar),
			&NoBytesRead,
			NULL);

		SerialBuffer[i] = tempChar;
		std::cout << SerialBuffer[i];
		i++;
	} while (NoBytesRead > 0);
}

void RWSerialPort::writeSerialPort(char *lpBuffer, DWORD dNoOFBytestoWrite) {
	//lpBuffer - массив данных дл€ записи в последовательный порт

	Status = WriteFile(hComm,
		lpBuffer,
		dNoOFBytestoWrite,  // оличество записаных байтов
		&dNoOfBytesWritten, //Ѕайтов записано
		NULL);

	if (Status)
	{
		std::cout << "Write byte successful: Bytes "
			<< dNoOfBytesWritten << " from " << dNoOFBytestoWrite 
			<< std::endl;
	}
	else {
		errno = static_cast<int>(GetLastError());
		strerror_s(bufError, sizeof(bufError), errno);
		std::cerr << "Error write: " << bufError << std::endl;
	}
}

bool RWSerialPort::isConnected(void) {
	return this->Status;
}

RWSerialPort::~RWSerialPort()
{
	CloseHandle(hComm);
}