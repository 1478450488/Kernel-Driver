#include <ntddk.h>
#include <windef.h>
typedef struct _ServiceDescriptorTable {
	PVOID ServiceTableBase;					//System Service Dispatch Table �Ļ���ַ  
	PVOID ServiceCounterTable;				//������ SSDT ��ÿ�����񱻵��ô����ļ����������������һ����sysenter ���¡� 
	unsigned int NumberOfServices;			//�� ServiceTableBase �����ķ������Ŀ��  
	PVOID ParamTableBase;					//����ÿ��ϵͳ��������ֽ�����Ļ���ַ-ϵͳ��������� 
} *PServiceDescriptorTable;  
extern PServiceDescriptorTable KeServiceDescriptorTable;

#pragma pack(1)	//��1�ֽڶ���
typedef struct MYJMPCODE {
	BYTE E9;
	ULONG JMPADDR;
}JMPCODE, *PJMPCODE;
#pragma pack()	//�ָ��ֽڶ���

PJMPCODE pjmpcode;
JMPCODE oldjmpcode;
ULONG cur, old;
BOOL ishook;

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

VOID UnhookProcess() {
	_asm {									//ȥ��ҳ�汣��
		cli
		mov eax, cr0
		and eax, not 10000h
		mov cr0, eax
	}
	pjmpcode = (PJMPCODE)cur;				//ָ��ǰĿ���ַ
	oldjmpcode.E9 = pjmpcode->E9;			//���浱ǰĿ���ַǰ5�ֽ�
	oldjmpcode.JMPADDR = pjmpcode->JMPADDR;
	pjmpcode->E9 = 0xE9;					//�޸ĵ�ǰĿ���ַ
	pjmpcode->JMPADDR = old-cur-5;
	_asm {									//�ָ�ҳ�汣��
		mov eax, cr0
		or eax, 10000h
		mov cr0,eax
		sti
	}
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObject) {
	if (ishook) {
		_asm {									//ȥ��ҳ�汣��
			cli
			mov eax, cr0
			and eax, not 10000h
			mov cr0, eax
		}
		pjmpcode = (PJMPCODE)cur;				//ָ��ǰĿ���ַ
		pjmpcode->E9 = oldjmpcode.E9;			//�ָ�Ŀ���ַǰ5�ֽ�	
		pjmpcode->JMPADDR = oldjmpcode.JMPADDR;
		_asm {									//�ָ�ҳ�汣��
			mov eax, cr0
			or eax, 10000h
			mov cr0,eax
			sti
		}
	}
	DbgPrint("�����ɹ�ж��!");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING reg_path) {
	DbgPrint("�����ɹ�����!\n");
	cur = GetNtOpenProcessCurrentAddr();
	old = GetNtOpenProcessOldAddr();
	if (old != cur) {
		ishook=TRUE;
		DbgPrint("NtOpenProcess��ַ��Hook��");
		UnhookProcess();
	} else {
		ishook=FALSE;
		DbgPrint("NtOpenProcess��ַ����");
	}
	pDriverObject->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}