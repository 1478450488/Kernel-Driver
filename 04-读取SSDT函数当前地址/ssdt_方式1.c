#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriverObject) {
	DbgPrint("�����ɹ�ж��!");
}

extern long KeServiceDescriptorTable;	//��ȡSSDT��[[KeServiceDescriptorTable]+0x7a*4]�е�NtOpenProcess��ַ

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING reg_path) {
	ULONG SSDT_NtOpenProcess;
	DbgPrint("�����ɹ�����!\n");
	pDriverObject->DriverUnload = DriverUnload;
	_asm {
		push ebx
		push eax
			mov ebx, KeServiceDescriptorTable
			mov ebx, [ebx]											//[KeServiceDescriptorTable]
			mov eax, 0x7a											
			shl eax, 2
			add ebx, eax												//[KeServiceDescriptorTable]+0x7a*4
			mov ebx, [ebx]											//NtOpenProcess��ַ
			mov SSDT_NtOpenProcess, ebx
		pop eax
		pop ebx
	}
	KdPrint(("NtOpenProcess�ĵ�ַ��%08x\n", SSDT_NtOpenProcess));
	return STATUS_SUCCESS;
}