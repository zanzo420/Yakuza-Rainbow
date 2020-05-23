_TEXT SEGMENT

; Void FreeDriver(std::uintptr_t PoolBase, ULONG PoolTag, void* retAddr, void* exFreePoolWithTag)
; rcx rdx r8 r9

FreeDriver PROC

	push r8 ; pushing return address
	jmp r9 ; calling exFreePoolWithTag

FreeDriver ENDP

_TEXT ENDS

END