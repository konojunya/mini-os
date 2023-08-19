/* entrypoint は boot */
ENTRY(boot)

SECTIONS {
  /* ベースアドレス */
  . = 0x80200000;  

  /* コード領域 */
  .text :{
    KEEP(*(.text.boot));
    *(.text .text.*);
  }

  /* 定数データ領域 読み取り only */
  .rodata : ALIGN(4) {
    *(.rodata .rodata.*);
  }

  /* 読み書き可能データ領域 */
  .data : ALIGN(4) {
    *(.data .data.*);
  }

  /* 読み書き可能データ領域。初期値が 0 の変数を配置する */
  .bss : ALIGN(4) {
    __bss = .;
    *(.bss .bss.* .sbss .sbss.*);
    __bss_end = .;
  }

  /* 4byte境界になるようにアドレスを調整する（= 32bitで区切る?） */
  . = ALIGN(4);
  . += 128 * 1024;
  __stack_top = .;
}