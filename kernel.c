typedef unsigned char uint8_t;
typedef unsigned short uint32_t;
typedef uint32_t size_t;

/**
 * boot した時に定義しておいた領域を C に展開する
 *
 * extern char __bbs と書いても良いが __bss と書くと
 * リンカスクリプトで定義した .bss領域の先頭アドレスではなく
 * .bss領域の0バイト目の値という意味になってしまうため [] をつけて
 * __bss がアドレスを返すようにすることでケアレスミスを防ぐ
 */
extern char __bss[], __bss_end[], __stack_top[];

void *memset(void *buf, char c, size_t n)
{
  // buf を uint8_t のポインタにキャストする
  uint8_t *p = (uint8_t *)buf;

  // 領域アドレスから n 分のメモリを c で埋める
  while (n--)
    *p++ = c;

  // ポインタで値が差し込まれた buf を返す
  return buf;
}

void kernel_main(void)
{
  /**
   * memset を使って.bss領域を 0 で初期化する
   * stack は 0 に向かって伸びるため領域の末尾アドレスを設定する
   */
  memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);

  for (;;)
    ;
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