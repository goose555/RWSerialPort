#include "RWSerialPort.h"

RWSerialPort::RWSerialPort(LPCTSTR portName)
{
	hComm = CreateFile(portName, 
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hComm == INVALID_HANDLE_VALUE){
		Error = GetLastError();
		std::cerr << "Error: " << strerror(Error) << std::endl;
	}else
		std::cout << "Opening serial port successful" << std::endl;

	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	Status = GetCommState(hComm, &dcbSerialParams); //ѕолучение настроек последовательного порта
	//«начени€ дл€ скорости в бодах, размера байта, количества начальных / конечных битов
	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

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

void RWSerialPort::redSerialPort(void) {
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

RWSerialPort::~RWSerialPort()
{
	CloseHandle(hComm);
}