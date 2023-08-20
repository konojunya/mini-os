# 出てきたコマンド

## QEMU

Ctrl+A を押下した直後に、C を入力すると、QEMU のデバッグコンソール (QEMU モニター) に移行します。モニター上で q コマンドを実行すると、QEMU を終了できます。
Ctrl+A には、QEMU モニターへの移行 (C キー) の他にもいくつかの機能があります。例えば、X キーを押下すると、QEMU を即座に終了します。

```shell
C-a h print this help
C-a x exit emulator
C-a s save disk data back to file (if -snapshot)
C-a t toggle console timestamps
C-a b send break (magic sysrq)
C-a c switch between console and monitor
C-a C-a sends C-a
```

- `info registers`: 現在のレジスタの値を表示する

## llvm

- `llvm-objdump -d kernel.elf`: 逆アセンブラ（llvm-objdump）を使って CPU からみた kernel.c の内容を確認
- `llvm-nm kernel.elf`: 各関数や変数のアドレスを確認する
