#include <iostream>
#include <Windows.h>
#include <WinIoCtl.h>
#include "ctl_code.h"
using namespace std; 

int add(HANDLE hDevice ,int a, int b) {
	int port[2];
	int buffret;
	ULONG dwWrite;
	port[0] = a;
	port[1] = b;
	::DeviceIoControl(hDevice, add_code , &port, 8, &buffret, 4, &dwWrite, NULL);
	return buffret;
}

int sub(HANDLE hDevice ,int a, int b) {
	int port[2];
	int buffret;
	ULONG dwWrite;
	port[0] = a;
	port[1] = b;
	::DeviceIoControl(hDevice, sub_code , &port, 8, &buffret, 4, &dwWrite, NULL);
	return buffret;
}

int hook(HANDLE hDevice ,ULONG pid) {
	int port[2];
	int buffret;
	ULONG dwWrite;
	port[0] = pid;
	::DeviceIoControl(hDevice, hook_code , &port, 8, &buffret, 4, &dwWrite, NULL);
	return buffret;
}

int unhook(HANDLE hDevice) {
	int port[2];
	int buffret;
	ULONG dwWrite;
	::DeviceIoControl(hDevice, unhook_code , &port, 8, &buffret, 4, &dwWrite, NULL);
	return buffret;
}

int main() {
	HANDLE hDevice = ::CreateFile("\\\\.\\MySysLinkName_20170411", 
															GENERIC_READ | GENERIC_WRITE,
															0,
															NULL,
															OPEN_EXISTING,
															FILE_ATTRIBUTE_NORMAL,
															NULL );
	printf("��ʼ\n");

	if (hDevice == INVALID_HANDLE_VALUE) {
		printf("��ȡ�������ʧ��: %s with Win32 error code: %d\n","MyDriver", GetLastError() );
		getchar();
		return -1;
	}
	 
	printf("����:a, b:\n");
	int a, b, r;
	scanf("%d%d", &a, &b);
	r = add(hDevice, a, b);
	printf("%d+%d=%d\n",a, b, r);
	r = sub(hDevice, a, b);
	printf("%d-%d=%d\n",a, b, r);

	ULONG pid;
	printf("����Ҫ�����Ľ���:pid:\n");
	scanf("%d", &pid);
	hook(hDevice, pid);
	getchar();
	return 0;
}