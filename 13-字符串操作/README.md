A��ASNI�ַ�����UNICODE�ַ���
    ��C�����У���ANSI��UNICODE ������ͬ�ִ����͡�
    char* s1="abc";          //CHAR * s1="abc"; 61
    KdPrint(("%x,%s\n",s1,s1));
    wchar_t * s2=L"abc";     //WCHAR * s2=L"abc";00 61
    KdPrint(("%x,%S\n",s2,s2));
    
B��ANSI_STRING�ַ�����UNICODE_STRING�ַ���
    ��C�����У���ͳ��char��wchar_t���͵��ַ�����ʹ�ù����У������׳��ֻ�����������󣬴Ӷ���������ϵͳ�ı�����һ�����ڳ���Աû�ж��ִ����Ƚ���У����ɵġ�
    ������DDK�� �ṩ��2�����ִ����ȵ����� ANSI_STRING��UNICODE_STRING
   ANSI_STRING s1 ;        //CHAR Length MaxLength PCHAR
   KdPrint(("%x,%Z\n",s1,s1));
   UNICODE_STRING s2   //WCHAR * =PWSTR
   KdPrint(("%x,%wZ\n",s2,s2));
    
C���ַ����ĳ�ʼ��������
��ʹ��ANSI_STRING��UNICODE_STRING��2�����͵��ִ�ʱ����Ҫ������г�ʼ�� ������
RtlInitAnsiString  RtlInitString(&s1,"abc"); //%Z
RtlInitUnicodeString(&s2,L"abc");// %wZ


D���ַ������ƣ��Ƚϣ�����Сд���������ִ����໥ת��
RtlFreeAnsiString
RtlFreeUnicodeString
RtlCopyUnicodeString
RtlEqualUnicodeString
RtlUpcaseUnicodeString
RtlUnicodeStringToInteger
RtlIntegerToUnicodeString

E��ANSI_STRING�ַ�����UNICODE_STRING�ַ����໥ת��
RtlUnicodeStringToAnsiString
RtlAnsiStringToUnicodeString
