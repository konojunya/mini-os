#!/bin/bash
set -xue

# QEMU file path
QEMU=qemu-system-riscv32

# clang path
CC=/opt/homebrew/opt/llvm/bin/clang

# -std=c11: C11を使用する
# -O2: 最適化レベル2を指定する。https://godbolt.org/ で試す場合は RISC-V rb32gc clang(trunk) を選択し、Compiler options に -O2 を指定する
# -g3: デバッグ情報を最大レベルで出力する
# -Wall: 主要な警告を有効にする
# -Wextra: 追加の警告を有効にする
# --target=riscv32: RISC-V 32bitをターゲットにする
# -ffreestanding: フリースタンディング環境を有効にする。フリースタンディング環境とは、OSが存在しない環境のこと。これによりホスト環境（macOS）の標準ライブラリを使用しないようになる。
# -nostdlib: 標準ライブラリを使用しないようにする。これにより、C言語のエントリポイントである main 関数を定義できるようになる。
# -WL,-Tkernel.ld: リンカスクリプトを指定する。リンカスクリプトとは、リンカがオブジェクトファイルをどのように結合するかを指定するファイルのこと。
# -Wl,-Map=kernel.map: マップファイルを出力する。マップファイルとは、リンカがどのようにオブジェクトファイルを結合したかを記録したファイルのこと。
CFLAGS="-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib"

# C のコンパイル
$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
    kernel.c common.c

# QEMU boot
# -machine virt: virtマシンとして起動する。ちなみに -machine '?'オプションで対応している環境を確認できる。
# -bios default: デフォルトのBIOS (ここではOpenSBI) を使用する。
## OpenSBIは SBI のOSS実装。SBI(Supervisor Binary Interface)については以下の記事を見るのが理解しやすい？
## https://zenn.dev/tetsu_koba/articles/d0a25ea5035b3e
# -nographic: QEMUをウィンドウなしで起動する。
# -serial mon:stdio: QEMUの標準入出力を仮想マシンのシリアルポートに接続する。mon:を指定することで、QEMUモニターへの切り替えも可能になる。
# --no-reboot: 仮想マシンがクラッシュしたら、再起動せずに停止させる (デバッグに便利)。
# -kernel kernel.elf: カーネルを指定する
$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot -kernel kernel.elf