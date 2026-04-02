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
```
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
```


### build and run kernel
```
$ clang++ $CPPFLAGS -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++17 -c main.cpp
$ ld.lld $LDFLAGS --entry KernelMain -z norelro --image-base 0x100000 --static -o kernel.elf main.o
$ source edksetup.sh
$ build -p MikanLoaderPkg/MikanLoaderPkg.dsc -a X64 -t CLANG38
$ $HOME/CreateOS/osbook/devenv/run_qemu.sh Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi $HOME/CreateOS/kernel/kernel.elf
```

### ScanBus
```
ScanAllBus
  ↓
ScanBus(bus)
  ↓
device = 0..31 を調べる
  ↓
ScanDevice(bus, device)
  ↓
function 0 を調べる
  ↓
多機能なら function 1..7 も調べる
  ↓
ScanFunction(bus, device, function)
  ↓
devices[] に登録
  ↓
PCI-PCI bridge なら secondary bus を再帰的に ScanBus
```

### メモリ管理
UEFIから渡されたメモリマップを参照して空いているメモリ領域を探し、メモリ割り当て要求に応じて必用なメモリ領域を払い渡す。その前にいくつかので0他構造をOS側に移動する。でないと勝手にメモリが他のデータで上書きされてしまう。
* スタック領域
    - 新しいスタック領域の末尾に移動する
* GDT
    - GDTを書き、`LoadGDT`でCPUに渡す
* ページテーブル
    - CR3にページテーブルのアドレスをセットする


フレームが使われているか管理
```
alloc_map_[0] → フレーム 0〜63
alloc_map_[1] → フレーム 64〜127
alloc_map_[2] → フレーム 128〜191
```

### 重ね合わせ処理
```
layer_manager
|
[layer]
|
window (shared_ptr)
```
という関係になっている。
```
for(layer:layer_manager->GetLayer()){
    layer->draw();
}
```
みたいにすると全てのlayerに対して毎フレーム描画することになり、これは遅延の原因になる。そこで、windowの`transeparent_color_`に値が入っている場合は、再描画する必要があるとみなし、再描画する。それ以外のwindowについては、memcopyでshadow_bufferを描画領域にコピーする。これは背景レイヤーとマウスレイヤーが別々のwindowを所有しているからできる。
```
layer_manager
|
[layer]
|
window
|
shadow_buffer
|
memcpy
```
スクロールのとき、consoleを全て一度塗りなすような処理は、処理が重い。そこで、2行目から最終行までを画像として1行分上にずらし、最終行だけ背景色で塗りつぶすことにする。具体的には
```
console
|
window -> Move({0,0},width,height) 
```
を行う。Moveではshadow_bufferを上下の方向にずらし、memcpyする。

### window
window(`main_window`)を作成し、whileのカウントを表示する。シンプルに実装するなら、毎フレームごとに全windowを再表示すればよい。しかし、そうすると背景のwindowも再表示されるため、ちらちらして見える。そこで、`layer_manager`の`draw`を指定した部分だけ描画するように改造する。
```cpp
void LayerManager::Draw(const Rectangle<int>& area) const {
  for (auto layer : layer_stack_) {
    layer->DrawTo(*screen_, area);
  }
}

void LayerManager::Draw(unsigned int id) const {
  bool draw = false;
  Rectangle<int> window_area;
  for (auto layer : layer_stack_) {
    if (layer->ID() == id) {
      window_area.size = layer->GetWindow()->Size();
      window_area.pos = layer->GetPosition();
      draw = true;
    }
    if (draw) {
      layer->DrawTo(*screen_, window_area);
    }
  }
}
```
`main_window->LayerID()`に引数を設定することでmain_windowより上のレイヤを描画できる。
