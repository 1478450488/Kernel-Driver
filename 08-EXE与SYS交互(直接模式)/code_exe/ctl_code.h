#ifndef CTL_CODE
	#pragma message("---------\nEXEģʽ . Include winioctl.h\n--------- ")
	#include <WinIoCtl.h>
#else
	#pragma message("---------\nSYSģʽ . No Include winioctl.h\n---------  ")
#endif

#define add_code CTL_CODE(FILE_DEVICE_UNKNOWN, 	0x800, 	METHOD_IN_DIRECT,FILE_ANY_ACCESS)
#define sub_code CTL_CODE(FILE_DEVICE_UNKNOWN, 	0x801, 	METHOD_IN_DIRECT,FILE_ANY_ACCESS)