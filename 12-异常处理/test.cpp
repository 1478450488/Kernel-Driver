#pragma once
#ifdef __cplusplus
	extern "C" {
#endif
	#include <ntddk.h>
#ifdef __cplusplus
	}
#endif
#define INITCODE code_seg("INIT")

void Memaccess_Test() {
	int i=3, *pi=NULL;
	bool isOk = true;
	KdPrint(("================\n"));
	KdPrint(("����*NULL=3;�Ƿ�ᷢ���쳣\n"));
	__try {
		*pi=i;
	} __except(1) {
		KdPrint(("�������쳣\n"));
		isOk = false;
	}
	if (isOk) {
		KdPrint(("û�����쳣\n"));
	}
	KdPrint(("================\n"));
}

void ProbeForRead_Test() {
	int *pi=NULL;
	bool isOk = true;
	KdPrint(("================\n"));
	KdPrint(("ʹ��ProbeForRead(NULL,14,4);�����ڴ��Ƿ�ɶ�\n"));
	__try {
		ProbeForRead(pi,14,4);
	} __except(EXCEPTION_EXECUTE_HANDLER) {
		KdPrint(("�������쳣\n"));
		isOk = false;
	}
	if (isOk) {
		KdPrint(("û�����쳣\n"));
	}
	KdPrint(("================\n"));
}

void ProbeForWrite_Test() {
	int *pi=NULL;
	bool isOk = true;
	KdPrint(("================\n"));
	KdPrint(("ʹ��ProbeForWrite(NULL,14,4);�����ڴ��Ƿ��д\n"));
	__try {
		ProbeForWrite(pi,4,1);
	} __except(EXCEPTION_EXECUTE_HANDLER) {
		KdPrint(("�������쳣\n"));
		isOk = false;
	}
	if (isOk) {
		KdPrint(("û�����쳣\n"));
	}
	KdPrint(("================\n"));
}

void finally_test() {
	int *pi=NULL;
	bool isOk = true;
	KdPrint(("================\n"));
	KdPrint(("����*NULL=3;�Ƿ�ᷢ���쳣\n"));
	__try {	
		__try {		//__finally��__except�����ܷ���һ�𣬲���C��׼����
			*pi = 3;
		} __finally {
			KdPrint(("__finally����������\n"));
		}
	} __except(EXCEPTION_EXECUTE_HANDLER) {
		KdPrint(("�������쳣\n"));
		isOk = false;
	}
	if (isOk) {
		KdPrint(("û�����쳣\n"));
	}
	KdPrint(("================\n"));
}

void ASSERT_test() {  
	int *p=NULL;
	int i=1;
	ASSERT(p==NULL); //p==NULL�������������������ж�
}

void TestException() {
	Memaccess_Test();
	ProbeForRead_Test();
	ProbeForWrite_Test();
	finally_test();
	ASSERT_test();
}


#pragma INITCODE
VOID DriverUnload(PDRIVER_OBJECT pDriverObject) {
	DbgPrint("�����ɹ�ж��!");
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING reg_path) {
	DbgPrint("�����ɹ�����!\n");
	TestException();
	pDriverObject->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}