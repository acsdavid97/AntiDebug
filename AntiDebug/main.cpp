#include <Windows.h>
#include <iostream>

https://www.codeproject.com/articles/1090943/anti-debug-protection-techniques-implementation-an#SEH

BOOL g_isDebuggerPresent = TRUE;
EXCEPTION_DISPOSITION ExceptionRoutine(
    PEXCEPTION_RECORD ExceptionRecord,
    PVOID             EstablisherFrame,
    PCONTEXT          ContextRecord,
    PVOID             DispatcherContext)
{
    g_isDebuggerPresent = FALSE;
    ContextRecord->Eip += 1;
    return ExceptionContinueExecution;
}

int main()
{
    __asm
    {
        // set SEH handler
        push ExceptionRoutine
        push dword ptr fs : [0]
        mov  dword ptr fs : [0] , esp
        // generate interrupt
        int  3h
        // return original SEH handler
        mov  eax, [esp]
        mov  dword ptr fs : [0] , eax
        add  esp, 8
    }
    if (g_isDebuggerPresent)
    {
        std::cout << "Stop debugging program!" << std::endl;
        exit(-1);
    }

    if (!g_isDebuggerPresent)
    {
        std::cout << "no debugger detected" << std::endl;
        exit(-2);
    }

    return 0;
}
