#pragma once
#ifdef __cplusplus
	extern "C" {
#endif
	#include <ntddk.h>
#ifdef __cplusplus
	}
#endif
#define INITCODE code_seg("INIT")


#define BUFFER_SIZE 1024
VOID CharTest() {  
	KdPrint(("=========�ִ�����=========\n"));
	PCHAR s1="abc11";		//CHAR
	KdPrint(("%x,%s\n",s1,s1)); 
	WCHAR* s2=L"abc11";		//WCHAR
	KdPrint(("%x,%S\n",s2,s2));
	KdPrint(("==========================\n"));
	KdPrint(("\n"));
}

VOID StringInitTest() {
	KdPrint(("=========��ʼ���ִ�����=========\n"));
	ANSI_STRING AnsiString1={0};
	ANSI_STRING AnsiString2={0};
	UNICODE_STRING UnicodeString1={0};
	UNICODE_STRING UnicodeString2={0};
	
	//�û�������ʼ��
	AnsiString1.Buffer=(PCHAR)ExAllocatePool(PagedPool,BUFFER_SIZE);
	strcpy(AnsiString1.Buffer,"ANSI_STRING!!!!!!");
	AnsiString1.Length=strlen( AnsiString1.Buffer);
	AnsiString1.MaximumLength=BUFFER_SIZE;

	UnicodeString1.Buffer=(PWSTR)ExAllocatePool(PagedPool,BUFFER_SIZE);
	wcscpy(UnicodeString1.Buffer,L"UNICODE_STRING!!!!!!");
	UnicodeString1.Length=wcslen(UnicodeString1.Buffer)*2-1;
	UnicodeString1.MaximumLength = BUFFER_SIZE;

	//��RTL������ʼ��
	RtlInitAnsiString(&AnsiString2, "ANSI_STRING!!!!!!");	 
	RtlInitUnicodeString(&UnicodeString2, L"UNICODE_STRING!!!!!!");

	//��ӡASCII�ַ��� %Z
	KdPrint(("%x AnsiString1:   %Z\n",&AnsiString1,   &AnsiString1));
	KdPrint(("%x AnsiString2:   %Z\n",&AnsiString2,   &AnsiString2));
	
	//��ӡUNICODE�ַ��� %wZ �����ַ��ᱻ�ض�
	KdPrint(("%x UnicodeString1:%wZ\n",&UnicodeString1,&UnicodeString1));
	KdPrint(("%x UnicodeString2:%wZ\n",&UnicodeString2,&UnicodeString2));
	KdPrint(("===========================\n")); 

	//����UnicodeString2
	//ע��!���ù�RtlInitUnicodeString ��ʼ�����ִ�!UnicodeString2��AnsiString1��������,���������
	RtlFreeAnsiString(&AnsiString1);
	RtlFreeUnicodeString(&UnicodeString1);
	KdPrint(("\n"));
}

VOID StringCopyTest() { 
	KdPrint(("=========�ִ������жϲ��Կ�ʼ=========\n"));
	UNICODE_STRING UnicodeString1={0};
	RtlInitUnicodeString(&UnicodeString1,L"COPY_STRING_TEST");

	UNICODE_STRING UnicodeString2={0};
	UnicodeString2.Buffer = (PWSTR)ExAllocatePool(PagedPool,BUFFER_SIZE);
	UnicodeString2.MaximumLength = BUFFER_SIZE;
	//����ʼ��UnicodeString1������UnicodeString2
	RtlCopyUnicodeString(&UnicodeString2,&UnicodeString1);

	//�ֱ���ʾUnicodeString1��UnicodeString2
	KdPrint(("�ִ�1:%wZ\n",&UnicodeString1));
	KdPrint(("�ִ�2:%wZ\n",&UnicodeString2));
	if ( RtlEqualUnicodeString(&UnicodeString1,&UnicodeString2,true)) { 
		KdPrint(("�ִ����\n"));
	}
	
	//����UnicodeString2
	//ע��!���ù�RtlInitUnicodeString ��ʼ�����ִ�UnicodeString1��������, 
	//RtlFreeUnicodeString(&UnicodeString1); ���ϴ��л�����
	RtlFreeUnicodeString(&UnicodeString2);
	KdPrint(("==================================\n"));
		KdPrint(("\n"));
}

VOID StringToUpperTest() {  
	KdPrint(("===============�ִ�ת��д����===============\n"));
	UNICODE_STRING UnicodeString1;
	RtlInitUnicodeString(&UnicodeString1,L"UnicodeString1��Aabc");
	//�仯ǰ
	KdPrint(("UnicodeString1 ��ֵ:%wZ\n",&UnicodeString1));
	//���д
	RtlUpcaseUnicodeString(&UnicodeString1,&UnicodeString1,FALSE);
	//�仯��
	KdPrint(("UnicodeString1 ת����д��:%wZ\n",&UnicodeString1));
	KdPrint(("=============================================\n"));
		KdPrint(("\n"));
}

VOID StringToIntegerTest() {   
	KdPrint(("===============�ִ�ת��������===============\n"));
	//�ַ���ת��������
	UNICODE_STRING UnicodeString1;
	RtlInitUnicodeString(&UnicodeString1,L"101");
	ULONG lNumber;
	KdPrint(("��ת���ִ�%wZ \n",&UnicodeString1));
	NTSTATUS nStatus = RtlUnicodeStringToInteger(&UnicodeString1,2,&lNumber);
	if ( NT_SUCCESS(nStatus)) {
		KdPrint(("�ִ�ת��2���������ɹ� ���=%d !\n",lNumber));
	} else {
		KdPrint(("ת������ ʧ��\n"));
	}

	//����ת�����ַ���
	UNICODE_STRING UnicodeString2={0};
	UnicodeString2.Buffer = (PWSTR)ExAllocatePool(PagedPool,BUFFER_SIZE);
	UnicodeString2.MaximumLength = BUFFER_SIZE;
	nStatus = RtlIntegerToUnicodeString(200,2,&UnicodeString2);
	if ( NT_SUCCESS(nStatus)) {
		KdPrint(("2��������ת���ִ��ɹ�! ���:%wZ\n",&UnicodeString2));
	} else {
		KdPrint(("ת���ִ� ʧ��!\n"));
	}

	//����UnicodeString2
	//ע��!���ù�RtlInitUnicodeString ��ʼ�����ִ�!UnicodeString1��������, 
	//RtlFreeUnicodeString(&UnicodeString1); ���ϴ��л�����
	RtlFreeUnicodeString(&UnicodeString2);
	KdPrint(("=============================================\n"));
		KdPrint(("\n"));
}

//ANSI_STRING�ַ�����UNICODE_STRING�ַ����໥ת������
VOID StringConverTest() {  
	KdPrint(("===============ANSI_STRING�ַ�����UNICODE_STRING�ַ����໥ת������===============\n"));
	//��UNICODE_STRING�ַ���ת����ANSI_STRING�ַ���
	UNICODE_STRING UnicodeString1;
	RtlInitUnicodeString(&UnicodeString1,L"UnicodeString");
	ANSI_STRING AnsiString1;
	NTSTATUS nStatus = RtlUnicodeStringToAnsiString(&AnsiString1,&UnicodeString1,true);
	if ( NT_SUCCESS(nStatus)) {
		KdPrint(("RtlUnicodeStringToAnsiString ת���ɹ� ���=%Z\n",&AnsiString1));
	} else {
		KdPrint(("RtlAnsiStringToUnicodeString ת��ʧ�� !\n"));
	}
	RtlFreeAnsiString(&AnsiString1);

	//��ANSI_STRING�ַ���ת����UNICODE_STRING�ַ���
	ANSI_STRING AnsiString2;
	RtlInitString(&AnsiString2,"AnsiString");
	UNICODE_STRING UnicodeString2;
	nStatus = RtlAnsiStringToUnicodeString(&UnicodeString2,&AnsiString2,true);
	if ( NT_SUCCESS(nStatus)) {
		KdPrint(("RtlAnsiStringToUnicodeStringת���ɹ� ���=%wZ\n",&UnicodeString2));
	} else {
		KdPrint(("RtlAnsiStringToUnicodeString�ִ�ת��ʧ��!\n"));
	}
	RtlFreeUnicodeString(&UnicodeString2);
	KdPrint(("=====================================================================================\n")); 
		KdPrint(("\n"));
}

void TestString() {
	CharTest();
	StringInitTest();
	StringCopyTest();
	StringToUpperTest();
	StringToIntegerTest();
	StringConverTest();
}


#pragma INITCODE
VOID DriverUnload(PDRIVER_OBJECT pDriverObject) {
	DbgPrint("�����ɹ�ж��!");
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING reg_path) {
	DbgPrint("�����ɹ�����!\n");
	TestString();
	pDriverObject->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}