# CreateOS
### BootLoaderビルド
memmap作成
```
$ cd $HOME/edk2
$ source edksetup.sh
$ build -p MikanLoaderPkg/MikanLoaderPkg.dsc -a X64 -t CLANG38
$ $HOME/CreateOS/osbook/devenv/run_qemu.sh Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi

$ mkdir -p mnt
$ sudo mount -o loop disk.img mnt
$ cat mnt/memmap
$ sudo umount mnt
```
### カーネル起動までの流れ
電源ON
  ↓
UEFI firmware
  ↓
BOOTX64.EFI
  ↓
loader.efi
  ↓
UefiMain()
  ↓
メモリマップ取得
  ↓
kernel.elf 読み込み
  ↓
ELF解析
  ↓
メモリに展開
  ↓
ExitBootServices
  ↓
kernel entry
  ↓
OS kernel 起動

### build and run kernel
```
$ clang++ $CPPFLAGS -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++17 -c main.cpp
$ ld.lld $LDFLAGS --entry KernelMain -z norelro --image-base 0x100000 --static -o kernel.elf main.o
$ build -p MikanLoaderPkg/MikanLoaderPkg.dsc -a X64 -t CLANG38
$ $HOME/CreateOS/osbook/devenv/run_qemu.sh Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi $HOME/CreateOS/kernel/kernel.elf
```
