#ifdef _WIN32

#define   TX_USE_SPEAK
#define   TX_COMPILED
#include "TXLib.h"

#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>

#include "speak.h"

void speak(const wchar_t* format, ...)
{
    const size_t BUFFER_SIZE = 64;
    wchar_t buffer[BUFFER_SIZE] = L"";

    va_list args = {};
    va_start(args, format);

    vswprintf(buffer, BUFFER_SIZE, format, args);

    va_end(args);

    txSpeak("<speak version='1.0' xmlns='http://www.w3.org/2001/10/synthesis' xml:lang='RU'>"
                "<prosody rate='fast' pitch='+1600Hz'>"
                    "%ls"
                "</prosody>"
            "</speak>", buffer);
}
#endif
