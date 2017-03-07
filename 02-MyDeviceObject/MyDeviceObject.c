#include <ntddk.h>
#define INITCODE code_seg("INIT") 
#define PAGECODE code_seg("PAGE")	//��ʾ�ڴ治��ʱ�����Ա��û���Ӳ��
#pragma INITCODE										//ָ�Ĵ������к� �ʹ��ڴ��ͷŵ�

NTSTATUS CreateMyDevice(IN PDRIVER_OBJECT pDriverObject) {
	NTSTATUS status;
	UNICODE_STRING devName;				//�豸����
	UNICODE_STRING sysLinkName;			//ϵͳ����������
	PDEVICE_OBJECT pDevObject;				//���ڷ��ش����豸

	RtlInitUnicodeString(&devName, L"\\Device\\MyDevObj");
	status = IoCreateDevice(pDriverObject, 0, &devName, FILE_DEVICE_UNKNOWN, 0, TRUE, &pDevObject);
	if (!NT_SUCCESS(status)) {						//�жϴ����豸�Ƿ�ɹ�
		if (status == STATUS_INSUFFICIENT_RESOURCES)
			KdPrint(("��Դ����\n"));
		if (status == STATUS_OBJECT_NAME_EXISTS)
			KdPrint(("ָ������������\n"));
		if (status == STATUS_OBJECT_NAME_COLLISION)
			KdPrint(("�������г�ͻ\n"));
		return status;
	}
	KdPrint(("�豸�����ɹ�\n"));

	pDevObject->Flags |= DO_BUFFERED_IO;	//��������ʽ��д
	RtlInitUnicodeString(&sysLinkName, L"\\??\\MySysLinkName_20170307");
	IoDeleteSymbolicLink(&sysLinkName);		//��ֹ������ͬ���������ظ�
	status = IoCreateSymbolicLink(&sysLinkName, &devName);		//�ж����ɷ��������Ƿ�ɹ�

	if (!NT_SUCCESS(status)) {
		KdPrint(("���ɷ�������ʧ��\n"));
		IoDeleteDevice(pDevObject);
		return status;
	}
	KdPrint(("���ɷ������ӳɹ�\n"));
	return STATUS_SUCCESS;
}


VOID DriverUnload(PDRIVER_OBJECT pDriverObject) {
	PDEVICE_OBJECT pDevObject;
	UNICODE_STRING sysLinkName;

	pDevObject = pDriverObject->DeviceObject;
	IoDeleteDevice(pDevObject);	//ȡ���豸��ɾ��
	KdPrint(("�ɹ�ɾ���豸\n"));

	RtlInitUnicodeString(&sysLinkName, L"\\??\\MySysLinkName_20170307");
	IoDeleteSymbolicLink(&sysLinkName);	//ȡ�÷������Ӳ�ɾ��
	KdPrint(("�ɹ�ɾ����������\n"));

	KdPrint(("�����ɹ�ж��\n"));
}


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING reg_path) {
	KdPrint(("�����ɹ�����\n"));
	CreateMyDevice(pDriverObject);
	pDriverObject->DriverUnload=DriverUnload;
	return STATUS_SUCCESS;
}