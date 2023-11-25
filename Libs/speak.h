#ifdef _WIN32

#ifndef SPEAK_H_
#define SPEAK_H_

#include <wchar.h>
#include <stdarg.h>

void speak(const wchar_t* format, ...);

#endif // SPEAK_H_

#endif // _WIN32
