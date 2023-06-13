
#ifdef _NEW_DELETE_OPERATORS_
#ifdef __cplusplus
extern "C" {
#include <wdm.h>
}
#else
#include <wdm.h>
#endif

#include "newDelete.h"
#include "definitions.h"

#pragma code_seg()

PVOID operator new
(
    size_t      iSize,
    POOL_FLAGS  poolFlags,
    ULONG       tag
)
{
    PVOID result = ExAllocatePool2(poolFlags, iSize, tag);

    return result;
}

PVOID operator new
(
    size_t      iSize,
    POOL_FLAGS  poolFlags
)
{
    PVOID result = ExAllocatePool2(poolFlags, iSize, SIMPLEAUDIOSAMPLE_POOLTAG);

    return result;
}

void __cdecl operator delete
(
    PVOID pVoid,
    ULONG tag
)
{
    if (pVoid)
    {
        ExFreePoolWithTag(pVoid, tag);
    }
}

void __cdecl operator delete
(
    _Pre_maybenull_ __drv_freesMem(Mem) PVOID pVoid,
    _In_ size_t cbSize
)
{
    UNREFERENCED_PARAMETER(cbSize);

    if (pVoid)
    {
        ExFreePoolWithTag(pVoid, SIMPLEAUDIOSAMPLE_POOLTAG);
    }
}


void __cdecl operator delete[]
(
    _Pre_maybenull_ __drv_freesMem(Mem) PVOID pVoid,
    _In_ size_t cbSize
)
{
    UNREFERENCED_PARAMETER(cbSize);

    if (pVoid)
    {
        ExFreePoolWithTag(pVoid, SIMPLEAUDIOSAMPLE_POOLTAG);
    }
}

void __cdecl operator delete[]
(
    _Pre_maybenull_ __drv_freesMem(Mem) PVOID pVoid
)
{
    if (pVoid)
    {
        ExFreePoolWithTag(pVoid, SIMPLEAUDIOSAMPLE_POOLTAG);
    }
}
#endif//_NEW_DELETE_OPERATORS_
