#include <Windows.h>
#include "../SampleServer/NameOfStopEvent.h"
int main()
{
	if (const auto stop_event = OpenEventA(MAXIMUM_ALLOWED, false, SampleServer_NameOfStopEvent)) {
		(void)SetEvent(stop_event);
		CloseHandle(stop_event);
	}
}
