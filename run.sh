#!/bin/bash
set -xue

# QEMU file path
QEMU=qemu-system-riscv32

# QEMU boot
# -machine virt: virtマシンとして起動する。ちなみに -machine '?'オプションで対応している環境を確認できる。
# -bios default: デフォルトのBIOS (ここではOpenSBI) を使用する。
# -nographic: QEMUをウィンドウなしで起動する。
# -serial mon:stdio: QEMUの標準入出力を仮想マシンのシリアルポートに接続する。mon:を指定することで、QEMUモニターへの切り替えも可能になる。
# --no-reboot: 仮想マシンがクラッシュしたら、再起動せずに停止させる (デバッグに便利)。
$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot