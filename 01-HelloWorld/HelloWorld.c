#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT driver) {
	DbgPrint("�����ɹ�ж��!");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING reg_path) {
	DbgPrint("�����ɹ�����!\n");
	driver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}
