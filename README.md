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

```
$ clang++ $CPPFLAGS -O2 --target=x86_64-elf -fno-exceptions -ffreestanding -c $HOME/CreateOS/kernel/main.cpp
$ ld.lld $LDFLAGS --entry KernelMain -z norelro --image-base 0x100000 --static -o kernel.elf main.o
$ build -p MikanLoaderPkg/MikanLoaderPkg.dsc -a X64 -t CLANG38
$ $HOME/CreateOS/osbook/devenv/run_qemu.sh Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi kernel.elf
```
