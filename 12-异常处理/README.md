����ڴ������
ProbeForRead
VOID ProbeForRead(
  __in  PVOID Address,
  __in  SIZE_T Length,
  __in  ULONG Alignment
);

ProbeForWrite
VOID ProbeForWrite(
  __in  PVOID Address,
  __in  SIZE_T Length,
  __in  ULONG Alignment
);

NTSTATUS
typedef LONG NTSTATUS;

NT_SUCCESS
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)

#define STATUS_SUCCESS                          ((NTSTATUS)0x00000000L) // ntsubauth
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes

R��Reserved)����λ
C (Customer) �ͻ�λ
Sev(Severity) ��Ҫλ ��2��������λ 00��ʾ�ɹ� 01��ʾ��Ϣ 10��ʾ���� 11��ʾ����
