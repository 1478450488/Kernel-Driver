#pragma once
#ifdef __cplusplus
extern "C"
	{
#endif
#include <NTDDK.h> //���������Ҫ��C��ʽ�����ͷ�ļ�
#ifdef __cplusplus
	}
#endif

BOOL ssdthook_flag = FALSE; //��ǰ�Ƿ������hook�ı�־
ULONG  RealNtOpenAddress; 	//hookǰ��NtOpenAddress��ַ
HANDLE hookPID; 			//Ҫhook�Ľ���id

typedef struct _ServiceDescriptorTable {
	PVOID ServiceTableBase;					//System Service Dispatch Table �Ļ���ַ  
	PVOID ServiceCounterTable;				//������ SSDT ��ÿ�����񱻵��ô����ļ����������������һ����sysenter ���¡� 
	unsigned int NumberOfServices;			//�� ServiceTableBase �����ķ������Ŀ��  
	PVOID ParamTableBase;					//����ÿ��ϵͳ��������ֽ�����Ļ���ַ-ϵͳ��������� 
} *PServiceDescriptorTable;  
extern "C" extern PServiceDescriptorTable KeServiceDescriptorTable;

// �����Զ���NtOpenProcess����ָ��
extern "C" typedef NTSTATUS __stdcall NTOPENPROCESS ( OUT PHANDLE ProcessHandle, 
													IN ACCESS_MASK AccessMask, 
													IN POBJECT_ATTRIBUTES ObjectAttributes, 
													IN PCLIENT_ID ClientId 
													);
NTOPENPROCESS *RealNtOpenProcess; 
PEPROCESS EP;

// �Զ����NtOpenProcess����MyNtOpenProcess
extern "C" NTSTATUS __stdcall MyNtOpenProcess(OUT PHANDLE ProcessHandle, IN  ACCESS_MASK DesiredAccess, IN  POBJECT_ATTRIBUTES ObjectAttributes, IN PCLIENT_ID ClientId ) { 
	NTSTATUS rc; 
	HANDLE PID; 
	KdPrint(("������MyNtOpenProcess"));  	
	if(ClientId != NULL) { 
		PID = ClientId->UniqueProcess; 	 
		KdPrint(( "����PID=%d\n",(int*)PID ));
		if(PID == hookPID) {			//�ж��Ƿ�Ϊ��Ҫ�����Ľ���pid
			KdPrint(("%d�Ǳ��������� hookPID=%d \n",(int)PID, (int)hookPID));
			KdPrint(("�������ڱ���,NtOpenProcessʧ��!!!!!\n"));
			ProcessHandle = NULL;
			rc = STATUS_ACCESS_DENIED;	//�ܾ�����
			EP = PsGetCurrentProcess();	//Ҳ��ʹ��PsLookupProcessByProcessId((ULONG)PID,&EP);		 
			KdPrint(("������:%s\n",(PTSTR)((ULONG)EP+0x174)));
		} else {
			KdPrint(("%d���Ǳ��������� hookPID=%d \n",(int)PID, (int)hookPID));
			rc = (NTSTATUS)RealNtOpenProcess(ProcessHandle, DesiredAccess, ObjectAttributes, ClientId); 
		}
	} 
	return rc; 
} 

// HOOK ��������
VOID HookNtOpenProcess() { 
	ssdthook_flag=true;
 	LONG PSSDT_Addr, *PNtOpenProcessr_Addr, NtOpenProcessr_Addr; 

	PSSDT_Addr=(LONG)KeServiceDescriptorTable->ServiceTableBase;
	PNtOpenProcessr_Addr=(PLONG)(PSSDT_Addr+0x7A*4);
	NtOpenProcessr_Addr=*PNtOpenProcessr_Addr;	 
	RealNtOpenAddress = *PNtOpenProcessr_Addr; 
	RealNtOpenProcess = (NTOPENPROCESS *)RealNtOpenAddress; 

	KdPrint(("��ʵ��NtOpenProcess��ַ: %x\n",(int)RealNtOpenAddress)); 
	KdPrint(("α��NTOpenProcess��ַ: %x\n", (int)MyNtOpenProcess));  
	__asm
	{
		 cli
			mov eax,cr0
			and eax,not 10000h
			mov cr0,eax
	}

	// �޸�SSDT 
	*PNtOpenProcessr_Addr= (ULONG)MyNtOpenProcess; 
	KdPrint(("UnHook�޸�NTOpenProcess OK \n")); 
		
	__asm 
	{ 
		  mov  eax, cr0 
		  or  eax, 10000h 
		  mov  cr0, eax 
		  sti 
	}   
	return;
} 

//UnHook��������
VOID UnHookNtOpenProcess() { 
	ULONG PNtOpenProcessr_Addr; 
	PNtOpenProcessr_Addr = (ULONG)KeServiceDescriptorTable->ServiceTableBase + 0x7A * 4; 
	if (ssdthook_flag) {
		ssdthook_flag=false;
		__asm 
		{ 
			cli 
			mov  eax, cr0 
			and  eax, not 10000h 
			mov  cr0, eax 
		} 

		// ��ԭSSDT 
		*((ULONG*)PNtOpenProcessr_Addr) = (ULONG)RealNtOpenAddress; 
		KdPrint(("UnHook��ԭNTOpenProcess OK \n")); 
		
		__asm 
		{ 
			mov  eax, cr0 
			or  eax, 10000h 
			mov  cr0, eax 
			sti 
		} 
	}
	return;
}
