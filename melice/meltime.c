//
//  meltime.c
//  shmup
//
//  Created by Raphaël Calabro on 02/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "meltime.h"

#ifndef _WIN32
int64_t MELNanoTime(void) {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec % 1000) * 1000000000L + time.tv_usec * 1000;
}
#else
int64_t MELNanoTime(void) {
	FILETIME    file_time;
	SYSTEMTIME  system_time;
	ULARGE_INTEGER ularge;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	ularge.LowPart = file_time.dwLowDateTime;
	ularge.HighPart = file_time.dwHighDateTime;

	int64_t tv_sec = ((ularge.QuadPart - 116444736000000000) / 10000000L);
	int64_t tv_usec = system_time.wMilliseconds * 1000;

	return (tv_sec % 1000) * 1000000000L + tv_usec * 1000;
}

NTSTATUS(__stdcall *NtDelayExecution)(BOOL Alertable, PLARGE_INTEGER DelayInterval) = NULL;
NTSTATUS(__stdcall *ZwSetTimerResolution)(IN ULONG RequestedResolution, IN BOOLEAN Set, OUT PULONG ActualResolution) = NULL;

int nanosleep(const struct timespec *rqtp, struct timespec *rmtp) {
	if (NtDelayExecution == NULL) {
		NtDelayExecution = (NTSTATUS(__stdcall*)(BOOL, PLARGE_INTEGER)) GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "NtDelayExecution");
		ZwSetTimerResolution = (NTSTATUS(__stdcall*)(ULONG, BOOLEAN, PULONG)) GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "ZwSetTimerResolution");

		ULONG actualResolution;
		ZwSetTimerResolution(1, TRUE, &actualResolution);
	}
	LARGE_INTEGER interval;
	interval.QuadPart = -1 * rqtp->tv_nsec / 100;
	NtDelayExecution(FALSE, &interval);
	return 0;
}
#endif
