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

#define MAX_BUFFER_LENGTH 1024
#define MAX_SERIAL_BUFFER_LENGTH 64
#define READ_TIMEOUT      500      // milliseconds

#include <iostream>
#include <Windows.h>
#include <errno.h>
//#include <fileapi.h>

class RWSerialPort
{
public:
	RWSerialPort(LPCTSTR portName);
	
	void readSerialPort(void);
	void writeSerialPort(char* lpBuffer, DWORD dNoOFBytestoWrite);
	bool isConnected(void);
	
	~RWSerialPort();

private:
	DCB dcbSerialParams = { 0 };
	DWORD dwEventMask;
	DWORD NoBytesRead;
	DWORD dNoOFBytestoWrite;
	DWORD dNoOfBytesWritten;
	char tempChar;

protected:
	HANDLE hComm;
	BOOL Status;
	char SerialBuffer[MAX_SERIAL_BUFFER_LENGTH];
	char bufError[MAX_BUFFER_LENGTH];
};

#endif // !RWSERIALPORT_H
