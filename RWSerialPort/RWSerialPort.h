#ifndef RWSERIALPORT_H
#define RWSERIALPORT_H

#define _CRT_SECURE_NO_WARNINGS

#define COM1  L"\\\\.\\COM1"
#define COM2  L"\\\\.\\COM2"
#define COM3  L"\\\\.\\COM3"
#define COM4  L"\\\\.\\COM4"
#define COM5  L"\\\\.\\COM5"
#define COM6  L"\\\\.\\COM6"
#define COM7  L"\\\\.\\COM7"
#define COM8  L"\\\\.\\COM8"
#define COM9  L"\\\\.\\COM9"
#define COM10 L"\\\\.\\COM10"
#define COM11 L"\\\\.\\COM11"
#define COM12 L"\\\\.\\COM12"
#define COM13 L"\\\\.\\COM13"
#define COM14 L"\\\\.\\COM14"
#define COM15 L"\\\\.\\COM15"
#define COM16 L"\\\\.\\COM16"
#define COM17 L"\\\\.\\COM17"
#define COM18 L"\\\\.\\COM18"
#define COM19 L"\\\\.\\COM19"

#include <iostream>
#include <Windows.h>

class RWSerialPort
{
public:
	RWSerialPort(LPCTSTR portName);
	
	void redSerialPort(void);

	~RWSerialPort();

private:
	HANDLE hComm;
	DCB dcbSerialParams = { 0 };
	BOOL Status;
	DWORD dwEventMask;
	DWORD NoBytesRead;
	int Error;
	char tempChar;
	char SerialBuffer[256];
};

#endif // !RWSERIALPORT_H
