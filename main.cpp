//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <tchar.h>
//---------------------------------------------------------------------------

#pragma argsused
#include <windows.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <conio.h>
#include <stdio.h>
#include "launch.h"
#include <Psapi.h>
#include <string.>
#include <iostream.h>

#define WIN32_LEAN_AND_MEAN
#define CREATE_THREAD_ACCESS (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

int game::yukle()
{
   // Process ID ve Title alıyoruz
   HWND WHandle=FindWindow(NULL, Form1->Edit1->Text.c_str());
   DWORD pID=GetTargetThreadIDFromProcName("KnightOnLine.exe");

   // dll'nin adresini alıyoruz
   char buf[MAX_PATH]={0};
   GetFullPathName("deb.bpl", MAX_PATH, buf, NULL);

/*   printf(buf);
   printf("\n");    */

   // dll'mizi inject ediyoruz
   if(!Inject(WHandle, pID, buf))
	  MessageBoxA(NULL, "DLL yüklenemedi.", "Loader", NULL);

   return 0;
}
//---------------------------------------------------------------------------

bool game::Inject(HWND WHandle, DWORD pID, const char *DLL_NAME)
{
	HANDLE Proc;
	HMODULE hLib;
	char buf[50] = {0};
	LPVOID RemoteString, LoadLibAddy;

   if(!pID || Form1->Edit1->Text=="deb client" || Form1->Edit1->Text=="")
   {
		return false;
   }else{
//		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)bypass, 0, 0, &pID);
//		bypass();
		LoadLibAddy=(LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
		RemoteString=(LPVOID)VirtualAllocEx(Proc, NULL, strlen(DLL_NAME), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		WriteProcessMemory(Proc, (LPVOID)RemoteString, DLL_NAME, strlen(DLL_NAME), NULL);
		CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL);
   }

   if(!WHandle)
   {
		MessageBoxA(NULL, "Knight Online bulunamadı.", NULL, GetLastError());
		return false;
   }

   Proc=OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);

   if(!Proc)
   {
		MessageBoxA(NULL, "OpenProcess() failed:", NULL, GetLastError());
	  //MessageBox(NULL, buf, "Loader", MB_OK);
//      printf(buf);
		return false;
   }

   CloseHandle(Proc);
   return true;
}
//---------------------------------------------------------------------------

DWORD game::GetTargetThreadIDFromProcName(char *ProcName)
{
	PROCESSENTRY32 pe;
	HANDLE thSnapShot;
	BOOL retval, ProcFound = false;

   thSnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   if(thSnapShot==INVALID_HANDLE_VALUE)
   {
      //MessageBox(NULL, "Error: Unable <strong class="highlight">to</strong> create toolhelp snapshot!", "2MLoader", MB_OK);
	  ShowMessage("Error: Unable to create toolhelp snapshot!");
	  return false;
   }

   pe.dwSize=sizeof(PROCESSENTRY32);

   retval=Process32First(thSnapShot, &pe);
   while(retval)
   {
	  if(StrStrI(pe.szExeFile, ProcName))
      {
         return pe.th32ProcessID;
      }
	  retval=Process32Next(thSnapShot, &pe);
   }
   return 0;
}
//---------------------------------------------------------------------------

void game::WriteMemory(void *lpMem, void *lpSrc, DWORD len)
{
	DWORD OldProtection;
	VirtualProtect(lpMem, len, PAGE_EXECUTE_READWRITE, &OldProtection);
	memcpy(lpMem, lpSrc, len);
	//ErrorExit(TEXT("memcpy"));
	VirtualProtect(lpMem, len, OldProtection, &OldProtection);
}
//---------------------------------------------------------------------------

typedef unsigned char byte;
int game::ReadMemory(void *lpMem, bool wait, bool debug)
{
	byte Bytes[]={0x0, 0x0, 0x0, 0x0};
	DWORD OldProtection;
	HANDLE ProcessHandle;
	VirtualProtect(lpMem, 4L, PAGE_EXECUTE_READWRITE, &OldProtection);
	if(debug){
		printf("Changed memory permission access of 0x%x from 0x%x to 0x%x\n", lpMem, OldProtection, PAGE_EXECUTE_READWRITE);
		printf("Buffer status: \t0x%x, 0x%x, 0x%x, 0x%x\n", Bytes[3], Bytes[2], Bytes[1], Bytes[0]);
	}
	DWORD value=0;

	while(1)
	{
		bool again=false;
			ReadProcessMemory(ProcessHandle, lpMem, &Bytes, 4L, 0L);

			if(debug)
				printf("Readed bytes in address 0x%x: \t0x%x, 0x%x, 0x%x, 0x%x\n", lpMem, Bytes[3], Bytes[2], Bytes[1], Bytes[0]);
			value=0;
			value=(value<<8) + Bytes[3];
			value=(value<<8) + Bytes[2];
			value=(value<<8) + Bytes[1];
			value=(value<<8) + Bytes[0];

			if((value==0xFFFFFFFF) || (value==0x00000000)){
				value=-1;
				again=wait;
			}

		if(!again)break;
		Sleep(2000);
	}

	VirtualProtect(lpMem, 4L, OldProtection, &OldProtection);
	return value;
}
//---------------------------------------------------------------------------

void game::bypass(void)
{
	DWORD w3pro = 0;

	while(1)
	{
		DWORD ehsvc=(DWORD)GetModuleHandle("ehsvc.dll");
		//w3pro = (DWORD)GetModuleHandleA("v3pro32s.dll");
		if(ehsvc)
		{
			WriteMemory((void *)(ehsvc + 0x09FB0), (void *)"\xC2\x04\x00", 3);
			WriteMemory((void *)(ehsvc + 0x2BF41), (void *)"\x90\x90", 2);
			WriteMemory((void *)(ehsvc + 0x3CBA4), (void *)"\x90\x90", 2);
			WriteMemory((void *)(ehsvc + 0x7B4EC), (void *)"\xC2\x04\x00",3);
			WriteMemory((void *)(ehsvc + 0x3CB2E), (void *)"\xB8\x01\x00\x00\x00\xC3",6);
			WriteMemory((void *)(ehsvc + 0x39F6F), (void *)"\xB8\x01\x00\x00\x00\xC3",6);
			WriteMemory((void *)(ehsvc + 0x3BFE6), (void *)"\xB8\x01\x00\x00\x00\xC3",6);
			Sleep(1000);
//			AttachProcess(Form1->Edit2->Text.c_str());
			break;
		 }
	}
}
//---------------------------------------------------------------------------

int game::WriteINI(char *app, char *key, char *str, char *file)
{
	return WritePrivateProfileString(app, key, str, file);
}
//---------------------------------------------------------------------------

char* game::ReadINI(char *app, char *key, char *file, char *defval)
{
	char *Result=new char[255];
//	file[255];

	memset(Result, 0x00, 255);
	GetPrivateProfileString(app, key, defval, Result, 255, file);

	return Result;
}
//---------------------------------------------------------------------------

bool game::CreateProc(char *name, char *loc)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset (&si, 0, sizeof(si));
	si.cb=sizeof(si);
	char buf[MAX_PATH] = {0};
	GetFullPathName("deb.bpl", MAX_PATH, buf, NULL);

	int process=CreateProcess(name, loc, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);
	DWORD pID=GetTargetThreadIDFromProcName("KnightOnLine.exe");

	if (!process)
	{
		MessageBoxA(0, "CreateProcess Failed", "Loader", 0);
		}else{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	return true;
}
//----------------------------------------------------------------------------

bool game::Inject2(DWORD pID, const char *DLL_NAME)
{
	HANDLE Proc;
	HMODULE hLib;
	char buf[50] = {0};
	LPVOID RemoteString, LoadLibAddy;

   if(!pID) {
		return false;
   }else{
//	   bypass();

   LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
   // Allocate space in the process for our DLL
   RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(DLL_NAME), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
   // Write the string name of our DLL in the memory allocated
   WriteProcessMemory(Proc, (LPVOID)RemoteString, DLL_NAME, strlen(DLL_NAME), NULL);
   // Load our DLL
   CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL);
   }

   Proc=OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);

   if(!Proc)
   {
		MessageBoxA(NULL, "OpenProcess() failed:", NULL, GetLastError());
		return false;
   }

   CloseHandle(Proc);
   return true;
}
//---------------------------------------------------------------------------

void game::SND_FIX()
{
	DWORD currentSend1;

    switch ((*(BYTE*)(0xCEED20))) {
          case 0x16:
            __asm mov currentSend1, 0
             break;
          case 0x17:
            __asm mov currentSend1, 1
             break;
          case 0x14:
            __asm mov currentSend1, 2
             break;
          case 0x15:
            __asm mov currentSend1, 3
            break;
          case 0x12:
            __asm mov currentSend1, 4
            break;
          case 0x13:
            __asm mov currentSend1, 5
            break;
          case 0x10:
            __asm mov currentSend1, 6
            break;
          case 0x11:
            __asm mov currentSend1, 7
            break;
          case 0x1E:
            __asm mov currentSend1, 8
            break;
          case 0x1F:
            __asm mov currentSend1, 9
           break;
    }
}
//---------------------------------------------------------------------------

void game::AttachProcess(LPTSTR WindowsName)
{
	DWORD ProcessID;
	HANDLE ProcessHandle;
	HWND WindowsHandle;

	ShowMessage("Waiting for FindWindowA");
	while(1)
	{
		WindowsHandle=FindWindowA(NULL, WindowsName);
		if(WindowsHandle)
		{
			ShowMessage("FindWindowA: " + (DWORD)WindowsHandle);
			ShowMessage("Waiting for GetWindowThreadProcessId");

			int addresses[]={0xD74C9C, 0xD74D54, 0xD74D20, 0x476762, 0xCEED20, 0x835DD0, 0xD74C98};
			while(1)
			{
				GetWindowThreadProcessId(WindowsHandle, &ProcessID);
				if(ProcessID)
				{
					ShowMessage("GetWindowThreadProcssId: " + ProcessID);
					ProcessHandle=OpenProcess(0x1F0FFF, false, (DWORD)ProcessID);
					ShowMessage("OpenProcess: " + (DWORD)ProcessHandle);
					DWORD oldProtection;
					VirtualProtectEx(ProcessHandle, (LPVOID)0xD74C9C, 4L, PAGE_EXECUTE_READWRITE, &oldProtection);
					ShowMessage("Changed memory permission access of 0xD7C9C from 0x to 0x" + oldProtection + PAGE_EXECUTE_READWRITE);
					SND_FIX();
					break;
				}
			}
			break;
		}
	}
}
