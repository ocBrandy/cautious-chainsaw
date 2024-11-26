#include  "LogDebugSwitch.h"
#include  "SEGGER_RTT.h"

void LogDebugInit(void)
{
	#if (USE_INSERT_DEBUG != 0)/*�ܵ��Կ��� 1�� 0�ر�*/
	SEGGER_RTT_Init();
	#endif
}


void SeggerRTT_LogDebug(const char * sFormat, ...)
{
	va_list ParamList;
  va_start(ParamList, sFormat);
  SEGGER_RTT_vprintf(0,sFormat, &ParamList);
  va_end(ParamList);
}
