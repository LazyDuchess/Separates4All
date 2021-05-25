// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

//thanks stackoverflow
void WriteToMemory(DWORD addressToWrite, char* valueToWrite, int byteNum)
{
    //used to change our file access type, stores the old
    //access type and restores it after memory is written
    unsigned long OldProtection;
    //give that address read and write permissions and store the old permissions at oldProtection
    VirtualProtect((LPVOID)(addressToWrite), byteNum, PAGE_EXECUTE_READWRITE, &OldProtection);

    //write the memory into the program and overwrite previous value
    memcpy((LPVOID)addressToWrite, valueToWrite, byteNum);

    //reset the permissions of the address back to oldProtection after writting memory
    VirtualProtect((LPVOID)(addressToWrite), byteNum, OldProtection, NULL);
}

char newFunc[] = { 0xE9, 0x89, 0xFF, 0xD0, 0xFF, 0x90 };
char newBytes[] = { 0xA1, 0xE7, 0x1F, 0x40, 0x00, 0x39, 0x46, 0x10, 0x0F, 0x84, 0x69, 0x00, 0x2F, 0x00, 0xA1, 0xEB, 0x1F, 0x40, 0x00, 0x39, 0x46, 0x10, 0x0F, 0x84, 0x5B, 0x00, 0x2F, 0x00, 0xA1, 0xEF, 0x1F, 0x40, 0x00, 0x39, 0x46, 0x10, 0x0F, 0x84, 0x4D, 0x00, 0x2F, 0x00, 0x89, 0x96, 0xD8, 0x00, 0x00, 0x00, 0xE9, 0x42, 0x00, 0x2F, 0x00 };
char bottomId[] = { 0x01, 0x00, 0x00, 0x01 };
char topId[] = { 0x00, 0x00, 0x00, 0x01 };
char outfitId[] = { 0x02, 0x00, 0x00, 0x01 };

DWORD WINAPI MainThread(LPVOID param)
{
    Sleep(10000);
    char* modBase = (char*)GetModuleHandleA(NULL);
    WriteToMemory((DWORD)modBase + 0x2F2024, newFunc, sizeof(newFunc) / sizeof(*newFunc));
    WriteToMemory((DWORD)modBase + 0x1FB2, newBytes, sizeof(newBytes) / sizeof(*newBytes));
    WriteToMemory((DWORD)modBase + 0x1FEB, bottomId, sizeof(bottomId) / sizeof(*bottomId));
    WriteToMemory((DWORD)modBase + 0x1FE7, topId, sizeof(topId) / sizeof(*topId));
    WriteToMemory((DWORD)modBase + 0x1FEF, outfitId, sizeof(outfitId) / sizeof(*outfitId));
    FreeLibraryAndExitThread((HMODULE)param, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, MainThread, hModule, 0, 0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

