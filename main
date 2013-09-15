#ifndef mainH
#define mainH

#include "launch.h"

class game
{
	public:

		int yukle();
		bool Inject(HWND WHandle, DWORD pID, const char *DLL_NAME);
		DWORD GetTargetThreadIDFromProcName(char *ProcName);
		void WriteMemory(void *lpMem, void *lpSrc, DWORD len);
		int ReadMemory(void *lpMem, bool wait, bool debug);
		void bypass(void);
		int WriteINI(char *app, char *key, char *str, char *file);
		char* ReadINI(char *app, char *key, char *file, char *defval);
		bool CreateProc(char *name, char *loc);
		bool Inject2(DWORD pID, const char *DLL_NAME);
		void SND_FIX();
		void AttachProcess(LPTSTR WindowsName);
};
#endif
