#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void __stdcall EasyMode(void)
{
	
	int func_StepSquare = 0x01003512;	//StepSquare(x, x) function's address
	int func_MakeGuess  = 0x0100374F;	//MakeGuess(x, x) function's address
	//int array_base      = 0x01005340;	//Pointer to the base of the array containing all the square values

	__asm 
	{
		mov 	ecx		, 	[ebp + 0xC]
		push	ecx
		mov 	esi		, 	[ebp + 0x8]
		push 	esi
		shl 	ecx		, 	5
		lea 	edx		, 	[ecx + esi + 0x01005340]
		cmp		[edx]	,	80h
		jnz		step_square
		
		call	func_MakeGuess

		add 	ebp		,	8
		mov		eax		,	[ebp - 0x8]
		mov		[ebp]	,	eax
		mov		eax		, 	[ebp - 0x4]
		mov		[ebp+4]	,	eax
		
		add 	esp		,	8
		pop		esi
		mov		esp		,	ebp
		pop		ebp
		ret

		step_square: 
		call func_StepSquare

		add 	ebp		,	8
		mov		eax		,	[ebp - 0x8]
		mov		[ebp]	,	eax
		mov		eax		, 	[ebp - 0x4]
		mov		[ebp+4]	,	eax

		add 	esp		,	8
	}
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	int hook_address = 0x010038B1;		//Address for redirecting call to my own function

	char address_patch[] = "\xe8XXXX";		//E8 = call opcode "XXXX" will be replaced later with the relative address to EasyMode()

	*((int*)(address_patch + 1)) = ((int) &EasyMode) - (hook_address + 5);	//Patch 4 "X" bytes in address_patch to make a relative call to EasyMode()


	HANDLE hProcess = GetCurrentProcess();

	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		WriteProcessMemory(hProcess, (void*) hook_address, address_patch, 5, NULL);
		break;
	}
	return TRUE;
}