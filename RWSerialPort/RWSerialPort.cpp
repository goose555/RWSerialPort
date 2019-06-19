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
	Status = GetCommState(hComm, &dcbSerialParams); //��������� �������� ����������������� �����
	//�������� ��� �������� � �����, ������� �����, ���������� ��������� / �������� �����
	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

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
	//lpBuffer - ������ ������ ��� ������ � ���������������� ����

	Status = WriteFile(hComm,
		lpBuffer,
		dNoOFBytestoWrite,  //���������� ��������� ������
		&dNoOfBytesWritten, //������ ��������
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