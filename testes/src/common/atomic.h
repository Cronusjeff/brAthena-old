/****************************************************************************!
*                _           _   _   _                                       *    
*               | |__  _ __ / \ | |_| |__   ___ _ __   __ _                  *  
*               | '_ \| '__/ _ \| __| '_ \ / _ \ '_ \ / _` |                 *   
*               | |_) | | / ___ \ |_| | | |  __/ | | | (_| |                 *    
*               |_.__/|_|/_/   \_\__|_| |_|\___|_| |_|\__,_|                 *    
*                                                                            *
*                                                                            *
* \file src/common/atomic.h                                                  *
* Descri��o Prim�ria.                                                        *
* Descri��o mais elaborada sobre o arquivo.                                  *
* \author brAthena, rAthena                                                  *
* \date ?                                                                    *
* \todo ?                                                                    *  
*****************************************************************************/ 

#ifndef _rA_ATOMIC_H_
#define _rA_ATOMIC_H_

// Atomic Operations
// (Interlocked CompareExchange, Add .. and so on ..)
//
// Implementation varies / depends on:
//	- Architecture
//	- Compiler
//	- Operating System
//
// our Abstraction is fully API-Compatible to Microsofts implementation @ NT5.0+
//
#include "../common/cbasetypes.h"

#if defined(_MSC_VER)
#include "../common/winapi.h"

#if !defined(_M_X64)
// When compiling for windows 32bit, the 8byte interlocked operations are not provided by microsoft
// (because they need at least i586 so its not generic enough.. ... )
forceinline int64 InterlockedCompareExchange64(volatile int64 *dest, int64 exch, int64 _cmp)
{
	_asm {
		lea esi,_cmp;
		lea edi,exch;

		mov eax,[esi];
		mov edx,4[esi];
		mov ebx,[edi];
		mov ecx,4[edi];
		mov esi,dest;

		lock CMPXCHG8B [esi];
	}
}


forceinline volatile int64 InterlockedIncrement64(volatile int64 *addend)
{
	__int64 old;
	do {
		old = *addend;
	} while(InterlockedCompareExchange64(addend, (old+1), old) != old);

	return (old + 1);
}



forceinline volatile int64 InterlockedDecrement64(volatile int64 *addend)
{
	__int64 old;

	do {
		old = *addend;
	} while(InterlockedCompareExchange64(addend, (old-1), old) != old);

	return (old - 1);
}

forceinline volatile int64 InterlockedExchangeAdd64(volatile int64 *addend, int64 increment)
{
	__int64 old;

	do {
		old = *addend;
	} while(InterlockedCompareExchange64(addend, (old + increment), old) != old);

	return old;
}

forceinline volatile int64 InterlockedExchange64(volatile int64 *target, int64 val)
{
	__int64 old;
	do {
		old = *target;
	} while(InterlockedCompareExchange64(target, val, old) != old);

	return old;
}

#endif //endif 32bit windows

#elif defined(__GNUC__)

#if !defined(__x86_64__) && !defined(__i386__)
#error Your Target Platfrom is not supported
#endif

static forceinline int64 InterlockedExchangeAdd64(volatile int64 *addend, int64 increment)
{
	return __sync_fetch_and_add(addend, increment);
}//end: InterlockedExchangeAdd64()


static forceinline int32 InterlockedExchangeAdd(volatile int32 *addend, int32 increment)
{
	return __sync_fetch_and_add(addend, increment);
}//end: InterlockedExchangeAdd()


static forceinline int64 InterlockedIncrement64(volatile int64 *addend)
{
	return __sync_add_and_fetch(addend, 1);
}//end: InterlockedIncrement64()


static forceinline int32 InterlockedIncrement(volatile int32 *addend)
{
	return __sync_add_and_fetch(addend, 1);
}//end: InterlockedIncrement()


static forceinline int64 InterlockedDecrement64(volatile int64 *addend)
{
	return __sync_sub_and_fetch(addend, 1);
}//end: InterlockedDecrement64()


static forceinline int32 InterlockedDecrement(volatile int32 *addend)
{
	return __sync_sub_and_fetch(addend, 1);
}//end: InterlockedDecrement()


static forceinline int64 InterlockedCompareExchange64(volatile int64 *dest, int64 exch, int64 cmp)
{
	return __sync_val_compare_and_swap(dest, cmp, exch);
}//end: InterlockedCompareExchange64()


static forceinline int32 InterlockedCompareExchange(volatile int32 *dest, int32 exch, int32 cmp)
{
	return __sync_val_compare_and_swap(dest, cmp, exch);
}//end: InterlockedCompareExchnage()


static forceinline int64 InterlockedExchange64(volatile int64 *target, int64 val)
{
	return __sync_lock_test_and_set(target, val);
}//end: InterlockedExchange64()


static forceinline int32 InterlockedExchange(volatile int32 *target, int32 val)
{
	return __sync_lock_test_and_set(target, val);
}//end: InterlockedExchange()


#endif //endif compiler decission


#endif
