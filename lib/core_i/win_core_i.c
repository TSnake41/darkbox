#include <windows.h>

int core_peek_stdin(void)
{
    /* Threat stdin as a pipe, curiously, this works,
       however this do not follow Windows's philosophy ^^
    */
    HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD available = 0;
    PeekNamedPipe(hin, NULL, 0, NULL, &available, NULL);
    return available;
}
