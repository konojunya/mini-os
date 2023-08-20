#pragma once

// va_list などは C 標準ライブラリの <stdarg.h> に定義されているマクロだが、標準ライブラリに頼らずコンパイラが用意しているものを直接使うため __builtin_ がついている
#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_end __builtin_va_end
#define va_arg __builtin_va_arg

void printf(const char *fmt, ...);