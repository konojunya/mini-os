#include "common.h"

/**
 * メモリの内容を特定の値で埋める
 * @param buf 埋めるメモリの先頭アドレス
 * @param c 埋める値
 * @param n 埋めるバイト数
 * @return buf
 */
void *memset(void *buf, char c, size_t n)
{
  // buf を uint8_t のポインタにキャストする
  uint8_t *p = (uint8_t *)buf;

  // 領域アドレスから n 分のメモリを c で埋める
  while (n--)

    /**
     * C言語ではよくやる手法として
     * *p = c;
     * p = p + 1;
     * を省略して書くというものがある
     */
    *p++ = c;
  return buf;
}

/**
 * メモリの内容をコピーする
 * @param dst コピー先のメモリの先頭アドレス
 * @param src コピー元のメモリの先頭アドレス
 * @param n コピーするバイト数
 * @return dst
 */
void *memcpy(void *dst, const void *src, size_t n)
{
  uint8_t *d = (uint8_t *)dst;
  const uint8_t *s = (const uint8_t *)src;
  while (n--)
    *d++ = *s++;
  return dst;
}

/**
 * 文字列をコピーする
 * @param dst コピー先の文字列の先頭アドレス
 * @param src コピー元の文字列の先頭アドレス
 * @return dst
 */
char *strcpy(char *dst, const char *src)
{
  char *d = dst;
  while (*src)
  {
    *d++ = *src++;
  }
  *d = '\0';

  return dst;
}

/**
 * 文字列を比較する
 * @param s1 比較する文字列の先頭アドレス
 * @param s2 比較する文字列の先頭アドレス
 * @return s1 と s2 が等しい場合は 0、s1 が s2 より小さい場合は負の数、s1 が s2 より大きい場合は正の数
 */
int strcmp(const char *s1, const char *s2)
{
  while (*s1 && *s2)
  {
    if (*s1 != *s2)
    {
      break;
    }

    s1++;
    s2++;
  }

  return *s1 - *s2;
}

void putchar(char ch);

/**
 * C の pringf 相当を実装する
 * 今回は %d(10進数) と %x(16進数) と %s(文字列) のみ対応
 * @param fmt 出力するフォーマット
 * @param ... 可変長引数
 */
void printf(const char *fmt, ...)
{
  va_list vargs;
  va_start(vargs, fmt);

  while (*fmt)
  {
    if (*fmt == '%')
    {
      fmt++;
      switch (*fmt)
      {
        // % が続いている場合は % を出力する
      case '\0':
      case '%':
        putchar('%');
        break;

        // %s の対応
      case 's':
      {
        // 引数にとったものを文字列として出力する
        const char *s = va_arg(vargs, const char *);
        while (*s)
        {
          putchar(*s);
          s++;
        }
        break;
      }

      // %d の対応
      case 'd':
      {
        // 引数にとったものを int として取り出す
        int value = va_arg(vargs, int);
        if (value < 0)
        {
          // 負の数の場合は - を出力して正の数に変換する
          putchar('-');
          value = -value;
        }

        int divisor = 1;

        // value の最上位の桁を求めるために、10 で割り続ける
        while (value / divisor > 9)
        {
          divisor *= 10;
        }

        // 最上位の桁から順に出力する
        while (divisor > 0)
        {
          putchar('0' + value / divisor);
          value %= divisor;
          divisor /= 10;
        }

        break;
      }

      // %x の対応
      case 'x':
      {
        int value = va_arg(vargs, int);

        // 32 ビットの値を 4 ビットずつ 16 進数で出力する
        for (int i = 7; i >= 0; i--)
        {
          /**
           * 4 ビット（16進数の 1 桁が 4 ビットのため）ずつ変換する
           * 0xf は 0b1111 なので、0xf と論理積を取ることで下位 4 ビットを取り出す
           * i が 0 の時は 0 * 4 = 0 なので、下位 4 ビットが取り出せる
           * i が 1 の時は 1 * 4 = 4 なので、4 ビット右シフトして下位 4 ビットを取り出せる
           * i が 2 の時は 2 * 4 = 8 なので、8 ビット右シフトして下位 4 ビットを取り出せる
           * i が 3 の時は 3 * 4 = 12 なので、12 ビット右シフトして下位 4 ビットを取り出せる
           * i が 4 の時は 4 * 4 = 16 なので、16 ビット右シフトして下位 4 ビットを取り出せる
           * i が 5 の時は 5 * 4 = 20 なので、20 ビット右シフトして下位 4 ビットを取り出せる
           * i が 6 の時は 6 * 4 = 24 なので、24 ビット右シフトして下位 4 ビットを取り出せる
           * i が 7 の時は 7 * 4 = 28 なので、28 ビット右シフトして下位 4 ビットを取り出せる
           */
          int nibble = (value >> (i * 4)) & 0xf;

          // nibble は 0 から 15 までの値をとるので、16進数でどこに対応するかを出力する
          putchar("0123456789abcdef"[nibble]);
        }
      }
      }
    }
    else
    {
      // % 以外はそのまま出力する
      putchar(*fmt);
    }

    fmt++;
  }

  va_end(vargs);
}
