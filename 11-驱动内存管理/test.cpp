#pragma once
#ifdef __cplusplus
	extern "C" {
#endif
	#include <ntddk.h>
#ifdef __cplusplus
	}
#endif
#define INITCODE code_seg("INIT")



void* _cdecl operator new(size_t size, POOL_TYPE pooltype) {
	KdPrint(("����new����\n"));
	KdPrint(("�����ڴ���С:%d\n",size));
	return ExAllocatePool(pooltype, size);
}

void _cdecl operator delete(void* pointer) {
	KdPrint(("����delete����\n"));
	ExFreePool(pointer);
}

void check(VOID UNALIGNED *p1, VOID UNALIGNED *p2, unsigned char UNALIGNED *p, int ALLOCATE_SIZE) {
	DbgPrint("p1=");
	p = (unsigned char*)p1;
	for (int i = 0; i < ALLOCATE_SIZE; i++)
		DbgPrint("%02x ", p[i]);
	DbgPrint("\n");
	DbgPrint("p2=");
	p = (unsigned char*)p2;
	for (int i = 0; i < ALLOCATE_SIZE; i++)
		DbgPrint("%02x ", p[i]);
	DbgPrint("\n");
}

void TestMemFunciton() {
	VOID UNALIGNED *p1;
	VOID UNALIGNED *p2;
	unsigned char UNALIGNED *p;
	SIZE_T ALLOCATE_SIZE=8;
	ULONG uRet;

	#if DBG
		_asm int 3
	#endif

	//�������غ��new��delete����
	char *buffer=new (PagedPool)  char[ALLOCATE_SIZE];
	delete buffer;

	//Ϊp1,p2ָ������СΪALLOCATE_SIZE�ֽڵ��ں� �ڴ�
	p1 = ExAllocatePool(PagedPool, ALLOCATE_SIZE);
	KdPrint(("p1�����ڴ��ַ%x\n", (void*)p1));
	p2 = ExAllocatePool(PagedPool, ALLOCATE_SIZE);
	KdPrint(("p2�����ڴ��ַ%x\n", (void*)p2));
	check(p1, p2, p, ALLOCATE_SIZE);

	//���p1ָ��
	RtlFillMemory(p1, ALLOCATE_SIZE, 'X');
	KdPrint(("p1���ָ���\n"));
	check(p1, p2, p, ALLOCATE_SIZE);

	//����p1ָ�뵽p2ָ�����ݣ�����ȷ���ڴ�û���ص�������ʹ��RtlMoveMemory
	RtlCopyMemory(p2, p1, ALLOCATE_SIZE);
	KdPrint(("����p1ָ�뵽p2ָ�����ݺ�\n"));
	check(p1, p2, p, ALLOCATE_SIZE);

	//�Ƚ��ڴ���Ƿ���ͬ
	uRet = RtlCompareMemory(p1, p2, ALLOCATE_SIZE);
	if (uRet == ALLOCATE_SIZE)
		KdPrint(("p1��p2�ڴ����ͬ\n"));
	else
		KdPrint(("p1��p2�ڴ�鲻��ͬ\n"));

	//���p1ָ���ַ����
	RtlZeroBytes(p1, ALLOCATE_SIZE);
	KdPrint(("���p1ָ���ַ���ݺ�\n"));
	check(p1, p2, p, ALLOCATE_SIZE);
	uRet = RtlCompareMemory(p1, p2, ALLOCATE_SIZE);
	if (uRet == ALLOCATE_SIZE)
		KdPrint(("p1��p2�ڴ����ͬ\n"));
	else
		KdPrint(("p1��p2�ڴ�鲻��ͬ\n"));

	ExFreePool(p1);
	ExFreePool(p2);
}



#pragma INITCODE
VOID DriverUnload(PDRIVER_OBJECT pDriverObject) {
	DbgPrint("�����ɹ�ж��!");
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING reg_path) {
	DbgPrint("�����ɹ�����!\n");
	TestMemFunciton();
	pDriverObject->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}