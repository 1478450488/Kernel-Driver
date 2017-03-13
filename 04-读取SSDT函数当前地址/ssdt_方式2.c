#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriverObject) {
	DbgPrint("�����ɹ�ж��!");
}

typedef struct _ServiceDescriptorTable {
	PVOID ServiceTableBase;					//System Service Dispatch Table �Ļ���ַ  
	PVOID ServiceCounterTable;				//������ SSDT ��ÿ�����񱻵��ô����ļ����������������һ����sysenter ���¡� 
	unsigned int NumberOfServices;		//�� ServiceTableBase �����ķ������Ŀ��  
	PVOID ParamTableBase;						//����ÿ��ϵͳ��������ֽ�����Ļ���ַ-ϵͳ��������� 
} *PServiceDescriptorTable;  
extern PServiceDescriptorTable KeServiceDescriptorTable;

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING reg_path) {
	LONG PSSDT_Addr, *SSDT_Addr, SSDT_NtOpenProcess_Addr; 
	DbgPrint("�����ɹ�����!\n");
	pDriverObject->DriverUnload = DriverUnload;

	PSSDT_Addr=(LONG)KeServiceDescriptorTable->ServiceTableBase;
	KdPrint(("��ǰServiceTableBase��ַΪ%08x\n", PSSDT_Addr));
	SSDT_Addr=(PLONG)(PSSDT_Addr+0x7A*4);
	KdPrint(("��ǰPSSDT_Addr+0x7A*4=%08x\n", SSDT_Addr)); 
	SSDT_NtOpenProcess_Addr=*SSDT_Addr;	
	KdPrint(("��ǰSSDT_NtOpenProcess_Addr��ַΪ%08x\n", SSDT_NtOpenProcess_Addr));
	return STATUS_SUCCESS;
}