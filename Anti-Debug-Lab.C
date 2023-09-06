#include<Windows.h>
#include<stdio.h>
#include<winternl.h>

void BEEING_DEBUGGED();  
void BEEING_DEBUGGED_2();
void NT_QUERY_DETECT();  
void CREATE_FILE_HANDLE_DETECT();
int DETECT_BY_EXCEPTION();


int main(int argc, char* argv[]) {
	//FUNCTIONS DEFINED DOWN.
	printf("[+] PRESS ENTER TO BEGIN ");
	int PAUSE = getchar();
	
	BEEING_DEBUGGED();
	BEEING_DEBUGGED_2();
	NT_QUERY_DETECT();
	CREATE_FILE_HANDLE_DETECT();
    DETECT_BY_EXCEPTION();


	printf("[+] YOU WIN.. PRESS ENTER TO TERMINATE\n");
	PAUSE=getchar();
	exit(0);
	
}



int MAX_CALL = 0; 

void MAXIMUM() {
    //AVOID TOO MANY RECURSION, IGNORE THIS.
    if (MAX_CALL > 3) {
        printf("[-] TOO MANY TRIES. CLOSE THE PROCESS AND START AGAIN\n");
        int PAUSE = getchar();
        exit(1);
    }
    else { MAX_CALL += 1; }
}
char BUFFER[5][60] = {
    "y	nGTGNmd`ggklefg`weegfrcqqgf", //   LEVEL 1 key=34
    "G7A<Pyjyp<.<SZ<Q]RI]P<^YYUR[<XY^I[[YX<ZP][<L]OOYX", //     LEVEL 2 key= 28
    "io~WDW^}t|FcGW@K{\\T]@_SF[]\\bsaawv", // LEVEL 3 key= 50
    "C3E8T}n}t8,8W^8[j}yl}^qt}Pyv|t}8HYKK]\\", // level 4 key  81
    "]-[&Jcpcj&3&C^ECVROIH&NGHBJCT&VGUUCB",  // level 5  key=6
};

void PASSED(char* level, int key) {
    char buffer[60];
    unsigned int counter;
    for (counter = 0; counter < strlen(level); counter++) {

        buffer[counter] = level[counter] ^ key;

    }
    buffer[counter] = '\x00';
    printf("%s\n", buffer);

}


void (*SUCCESS)(char* level, int key) = PASSED;


PPEB GET_PEB() {
    PPEB pPeb;
#if defined(_WIN64)
    __asm {
        mov rax, qword ptr gs : [0x60]
        mov pPeb, rax
    }
#elif defined(_WIN32)
    __asm {
        mov eax, dword ptr fs : [0x30]
        mov pPeb, eax
    }
#endif
    return pPeb;
}

void BEEING_DEBUGGED() {
    if (IsDebuggerPresent()) {

        printf("[-] DEBUG DETECTED BY Beeing FLAG..\n[+] PRESS ENTER TO TRY AGAIN..\n");
        int PAUSE = getchar();
        MAXIMUM();
        return BEEING_DEBUGGED();

    }
    else {
        SUCCESS(BUFFER[0], 34);
        MAX_CALL = 0;


    }
}
//________________________________________________________________________________________

void BEEING_DEBUGGED_2() {
    PPEB PEB = GET_PEB();
    if (PEB->BeingDebugged) {
        printf("[-] DEBUG DETECTED BY Beeing FLAG..\n[+] PRESS ENTER TO TRY AGAIN..\n");
        MAXIMUM();
        int PAUSE = getchar();
        BEEING_DEBUGGED_2();

    }
    else {
        SUCCESS(BUFFER[1], 14 * 2);
        MAX_CALL = 0;
    }

}
//__________________________________________________________________________________________________________________

void NT_QUERY_DETECT() {
    typedef NTSTATUS(NTAPI* NtQuery_ptr) (IN HANDLE ProcessHandle, IN PROCESSINFOCLASS ProcessInformationClass, OUT PVOID ProcessInformation, IN ULONG ProcessInformationLength, OUT PULONG ReturnLength);

    HMODULE hNtdll = LoadLibraryA("ntdll.dll");
    if (hNtdll) {

        // auto type could be used!
        NtQuery_ptr pfnNtQueryInformationProcess = (NtQuery_ptr)GetProcAddress(hNtdll, "NtQueryInformationProcess");

        if (pfnNtQueryInformationProcess) {
            DWORD dwProcessDebugPort, dwReturned;

            NTSTATUS status = pfnNtQueryInformationProcess(GetCurrentProcess(), ProcessDebugPort, &dwProcessDebugPort, sizeof(DWORD), &dwReturned);

            if (NT_SUCCESS(status) && (-1 == dwProcessDebugPort)) {
                MAXIMUM();
                printf("[-] DEBUG DETECTED BY NTQueryInformationProcess..\n[+] PRESS ENTER TO TRY AGAIN..\n");
                int PAUSE = getchar(); return NT_QUERY_DETECT();

            }
            else {
                SUCCESS(BUFFER[2], 10 * 5);
                MAX_CALL = 0;
            }
        }
    }
}

//_____________________________________________________________________________________________________________________________
void CREATE_FILE_HANDLE_DETECT() {

    CHAR fileName[255];
    if (!GetModuleFileNameA(NULL, fileName, sizeof(fileName))) {
        printf("[-] DEBUG DETECTED USING FILE HANDLE..\n\t[+] PRESS ENTER TO TRY AGAIN..\n");

        int PAUSE = getchar();
        MAXIMUM();
        return CREATE_FILE_HANDLE_DETECT();
    }
    else {
        SUCCESS(BUFFER[3], 24);
        MAX_CALL = 0;


    }

}
//___________________________________________________________________________________________


int DETECT_BY_EXCEPTION() {
    BOOL debugger = TRUE;
    __try
    {
        RaiseException(EXCEPTION_BREAKPOINT, 0, 0, NULL);
    }
    __except (GetExceptionCode() == EXCEPTION_BREAKPOINT ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
        debugger = FALSE;
    }


    if (!debugger) {
        SUCCESS(BUFFER[4], int(2.0 * 3.0));
        MAX_CALL = 0;
    }
    else {
        printf("[-] DEBUGGER DETECTED USING EXCEPTION HANDLER..\n[+] PRESS ENTER TO RETRY\n");
        MAXIMUM();
        int PAUSE = getchar();

        return DETECT_BY_EXCEPTION();
    }

}
