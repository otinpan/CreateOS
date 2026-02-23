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
