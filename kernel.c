#include "kernel.h"
#include "common.h"

/**
 * boot した時に定義しておいた領域を C に展開する
 *
 * extern char __bbs と書いても良いが __bss と書くと
 * リンカスクリプトで定義した .bss領域の先頭アドレスではなく
 * .bss領域の0バイト目の値という意味になってしまうため [] をつけて
 * __bss がアドレスを返すようにすることでケアレスミスを防ぐ
 */
extern char __bss[], __bss_end[], __stack_top[];

struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long fid, long eid)
{
  /**
   * register と __asm__() は指定したレジスタに値を入れるようコンパイラに指示するためのもの
   */
  register long a0 __asm__("a0") = arg0;
  register long a1 __asm__("a1") = arg1;
  register long a2 __asm__("a2") = arg2;
  register long a3 __asm__("a3") = arg3;
  register long a4 __asm__("a4") = arg4;
  register long a5 __asm__("a5") = arg5;
  register long a6 __asm__("a6") = fid;
  register long a7 __asm__("a7") = eid;

  /**
   * ecall 命令を実行することで、OpenSBI に処理を委譲する
   * CPUの実行モードがカーネル用（S-Mode）からOpenSBI用（M-Mode）に切り替わる
   * a0 に格納された値は、OpenSBI が定義するエラーコード
   * a1 に格納された値は、OpenSBI が定義する返り値
   */
  __asm__ __volatile__("ecall"
                       : "=r"(a0), "=r"(a1)
                       : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5),
                         "r"(a6), "r"(a7)
                       : "memory");

  return (struct sbiret){.error = a0, .value = a1};
}

void putchar(char ch)
{
  sbi_call(ch, 0, 0, 0, 0, 0, 0, 1 /* Console Putchar */);
}

void kernel_main(void)
{
  /**
   * memset を使って.bss領域を 0 で初期化する
   * stack は 0 に向かって伸びるため領域の末尾アドレスを設定する
   */
  memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);

  printf("\n\nHello %s\n", "World!");
  printf("1 + 2 = %d, %x\n", 1 + 2, 0x1234abcd);

  for (;;)
  {
    /**
     * WFI 命令を実行することで、CPU を停止させる
     * WFI: Wait For Interrupt
     */
    __asm__ __volatile__("wfi");
  }
}

/**
 * OpenSBIからは実行イメージのベースアドレス (0x80200000) へジャンプするようになっているため
 * カーネルのエントリポイントは0x80200000に配置する必要がある
 *
 * __attribute__((section(".text.boot")))という属性をつけて専用のセクションに配置し
 * リンカスクリプトで先頭に来るようにしている
 * kernel.ld の 10行目 にある .text.boot は、このセクションの先頭アドレスを表している?
 */
__attribute__((section(".text.boot")))

/**
 * __attribute__((naked)) は関数のプロローグとエピローグを生成しない
 *
 * Prologue コード（プロローグコード）
 * 関数の先頭で、関数内で使用するスタックやレジスタの準備を行うコード
 * Epilogue コード（エピローグコード）
 * 関数の終わりで、スタックやレジスタを関数が呼ばれる前の状態に戻すコード
 *
 * ref: https://en.wikipedia.org/wiki/Function_prologue_and_epilogue
 * ref: https://u64178.blogspot.com/2014/10/attributenaked-prologue-epilogue.html
 */
__attribute__((naked))

void
boot(void)
{
  // 下記は inline assembly の記法
  // ref: https://ja.wikipedia.org/wiki/%E3%82%A4%E3%83%B3%E3%83%A9%E3%82%A4%E3%83%B3%E3%82%A2%E3%82%BB%E3%83%B3%E3%83%96%E3%83%A9
  __asm__ __volatile__(
      // リンカスクリプトで定義したシンボルを使って、スタックポインタを設定する
      "mv sp, %[stack_top]\n"
      // C で実装した kernel_main を呼び出す
      "j kernel_main\n"
      :
      : [stack_top] "r"(__stack_top));
}