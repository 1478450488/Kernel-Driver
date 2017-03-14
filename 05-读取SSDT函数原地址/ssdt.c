#include <ntddk.h>

typedef struct _ServiceDescriptorTable {
	PVOID ServiceTableBase;					//System Service Dispatch Table �Ļ���ַ  
	PVOID ServiceCounterTable;				//������ SSDT ��ÿ�����񱻵��ô����ļ����������������һ����sysenter ���¡� 
	unsigned int NumberOfServices;		//�� ServiceTableBase �����ķ������Ŀ��  
	PVOID ParamTableBase;						//����ÿ��ϵͳ��������ֽ�����Ļ���ַ-ϵͳ��������� 
} *PServiceDescriptorTable;  
extern PServiceDescriptorTable KeServiceDescriptorTable;

ULONG GetNtOpenProcessCurrentAddr() {
	ULONG PSSDT_Addr, *SSDT_Addr, SSDT_NtOpenProcess_Cur_Addr;
	PSSDT_Addr=(LONG)KeServiceDescriptorTable->ServiceTableBase;
	SSDT_Addr=(PLONG)(PSSDT_Addr+0x7A*4);
	SSDT_NtOpenProcess_Cur_Addr=*SSDT_Addr;	
	KdPrint(("��ǰNtOpenProcess��ַΪ%08x\n", SSDT_NtOpenProcess_Cur_Addr));
	return SSDT_NtOpenProcess_Cur_Addr;
}

ULONG GetNtOpenProcessOldAddr() {
	ULONG  SSDT_NtOpenProcess_Old_Addr; 
	UNICODE_STRING SSDT_NtOpenProcess_Name;
	RtlInitUnicodeString(&SSDT_NtOpenProcess_Name, L"NtOpenProcess");
	SSDT_NtOpenProcess_Old_Addr = (ULONG)MmGetSystemRoutineAddress(&SSDT_NtOpenProcess_Name);
	KdPrint(("ԭ����NtOpenProcess�ĵ�ַΪ%08x\n", SSDT_NtOpenProcess_Old_Addr));
	return SSDT_NtOpenProcess_Old_Addr;
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObject) {
	DbgPrint("�����ɹ�ж��!");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING reg_path) {
	ULONG cur, old;
	DbgPrint("�����ɹ�����!\n");
	cur = GetNtOpenProcessCurrentAddr();
	old = GetNtOpenProcessOldAddr();
	if (old != cur) {
		DbgPrint("NtOpenProcess��ַ��Hook��");
	} else {
		DbgPrint("NtOpenProcess��ַ����");
	}
	pDriverObject->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}