#include <ntddk.h>
#define INITCODE code_seg("INIT")	//ָ�Ĵ������к� �ʹ��ڴ��ͷŵ�
#define PAGECODE code_seg("PAGE")	//��ʾ�ڴ治��ʱ�����Ա��û���Ӳ��							

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



NTSTATUS MyDispatchRoutine(IN PDEVICE_OBJECT pDevobj,IN PIRP pIrp) {		//Ҳ�ɷֿ��ò�ͬ������������
	PIO_STACK_LOCATION irpsp=IoGetCurrentIrpStackLocation(pIrp);
	switch (irpsp->MajorFunction) {
	   case IRP_MJ_CREATE:
		   break;
	   case IRP_MJ_CLOSE:
		   break;
	   case IRP_MJ_READ:
		   break;
	   case IRP_MJ_WRITE:
		   break;
	   case  IRP_MJ_DEVICE_CONTROL:
		   break;
	   default:
	 KdPrint(("��������")); 
	}
	return STATUS_SUCCESS;
}





#pragma INITCODE
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING reg_path) {
	KdPrint(("�����ɹ�����\n"));
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = MyDispatchRoutine;	//������ǲ���̣�Ҳ�ɷֿ��ò�ͬ����
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = MyDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_READ] = MyDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = MyDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = MyDispatchRoutine;
	CreateMyDevice(pDriverObject);
	pDriverObject->DriverUnload=DriverUnload;
	return STATUS_SUCCESS;
}