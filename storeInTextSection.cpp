#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

	PVOID allocatedMemory;
	BOOL virtualProtectReturnValue;
	HANDLE hThread;
	DWORD previousMemoryProtectionConstant = 0;

	WCHAR payload[] = { 0x90, 0x90, 0xcc, 0xc3 };
	size_t payload_len = sizeof(payload) / sizeof(payload[0]);

	allocatedMemory = ::VirtualAlloc(NULL, payload_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	printf("%-20s : 0x%-016p\n", "payload addr", (PVOID)payload);
	printf("%-20s : 0x%-016p\n", "allocatedMemory addr", (PVOID)allocatedMemory);


	::RtlMoveMemory(allocatedMemory, payload, payload_len);


	virtualProtectReturnValue = ::VirtualProtect(allocatedMemory, payload_len, PAGE_EXECUTE_READ, &previousMemoryProtectionConstant);

	printf("\nHit me!\n");
	getchar();


	if (virtualProtectReturnValue != 0) {
		// second argument is the start address for the thread
		hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)allocatedMemory, NULL, 0, NULL);
		::WaitForSingleObject(hThread, -1);
	}

	return 0;
}
