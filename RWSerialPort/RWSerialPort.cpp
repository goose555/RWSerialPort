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
	Status = GetCommState(hComm, &dcbSerialParams); //��������� �������� ����������������� �����
	//�������� ��� �������� � �����, ������� �����, ���������� ��������� / �������� �����
	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	//��������� ����� � ������������ �� ���������� DCB
	SetCommState(hComm, &dcbSerialParams);

	//��������� ����-�����
	COMMTIMEOUTS timeouts = { 0 };
	
	timeouts.ReadIntervalTimeout = 50; 
	/*
	���������� ������������ �������� ������� ����� ������������ ����
	������. ���� ����� �������� ��������� ��� �������, �������
	ReadFile () ������������.
	*/

	timeouts.ReadTotalTimeoutConstant = 50;
	/*
	������������ ��� ���������� ������ ������� �������� ��� �������� 
	������. ��� ������ �������� ������ ��� �������� ����������� � 
	�������� ����� ReadTotalTimeoutMultiplier � ������������ ���������� 
	������.
	*/
	
	timeouts.ReadTotalTimeoutMultiplier = 10;
	/*
	������������ ��� ���������� ������ ������� �������� ��� �������� 
	������. ��� ������ �������� ������ ��� �������� ���������� �� 
	����������� ���������� ������ ��� ������.
	*/

	timeouts.WriteTotalTimeoutConstant = 50;
	/*
	����� �� ReadTotalTimeoutConstant, �� ��� �������� ������.
	*/

	timeouts.WriteTotalTimeoutMultiplier = 10;
	/*
	���������� ReadTotalTimeoutMultiplier, �� ��� �������� ������.
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