#include "stdafx.h"
#include "Console.h"

BOOL CConsole::IsExistent = FALSE;

CConsole::CConsole()
{
    if (IsExistent)
        return;

    AllocConsole();
    Attach(300, 80);

    IsExistent = TRUE;
}


CConsole::CConsole(LPCTSTR lpszTitle, SHORT ConsoleHeight, SHORT ConsoleWidth)
{
    if (IsExistent)
        return;

    AllocConsole();
    SetConsoleTitle(lpszTitle);
    Attach(ConsoleHeight, ConsoleWidth);

    IsExistent = TRUE;
}


CConsole::~CConsole()
{
    if (IsExistent)
    {
        FreeConsole();
        IsExistent = FALSE;
    }
}


void CConsole::Attach(SHORT ConsoleHeight, SHORT ConsoleWidth)
{
    HANDLE  hStd;
    int     fd;
    FILE    *file;

    // 重定向标准输入流句柄到新的控制台窗口

    hStd = GetStdHandle(STD_INPUT_HANDLE);
    fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); // 文本模式
    file = _fdopen(fd, "r");
    setvbuf(file, NULL, _IONBF, 0); // 无缓冲
    *stdin = *file;

    // 重定向标准输出流句柄到新的控制台窗口

    hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD size;
    size.X = ConsoleWidth;
    size.Y = ConsoleHeight;
    SetConsoleScreenBufferSize(hStd, size);
    fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); //文本模式
    file = _fdopen(fd, "w");
    setvbuf(file, NULL, _IONBF, 0); // 无缓冲
    *stdout = *file;

    // 重定向标准错误流句柄到新的控制台窗口

    hStd = GetStdHandle(STD_ERROR_HANDLE);
    fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); // 文本模式
    file = _fdopen(fd, "w");
    setvbuf(file, NULL, _IONBF, 0); // 无缓冲
    *stderr = *file;
}
