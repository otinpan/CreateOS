#pragma once
#include<cstdint>
#include<array>
#include"error.hpp"

namespace pci{
  // CONFIG_ADDRESSレジスタのIOポートアドレス
  const uint16_t kConfigAddress=0x0cf8;
  // CONFIG_DATAレジスタのIOポートアドレス
  const uint16_t kConfigData=0x0cfc;

  // CONFIG_ADDRESSに指定されてた整数を書き込む
  void WriteAddress(uint32_t address);
  // CONFIG_DATAに指定された整数を書き込む
  void WriteData(uint32_t value);
  // CONFIG_DATAから32ビット整数を読み込む
  uint32_t ReadData();

  uint16_t ReadVendorId(uint8_t bus,uint8_t device,uint8_t function);
  uint16_t ReadDeviceId(uint8_t bus,uint8_t device,uint8_t function);
  uint8_t ReadHeaderType(uint8_t bus,uint8_t device,uint8_t function);

   uint32_t ReadClassCode(uint8_t bus, uint8_t device, uint8_t function);

  /** @brief バス番号レジスタを読み取る（ヘッダタイプ 1 用）
   *
   * 返される 32 ビット整数の構造は次の通り．
   *   - 23:16 : サブオーディネイトバス番号
   *   - 15:8  : セカンダリバス番号
   *   - 7:0   : リビジョン番号
   */
  uint32_t ReadBusNumbers(uint8_t bus, uint8_t device, uint8_t function);

  /** @brief 単一ファンクションの場合に真を返す． */
  bool IsSingleFunctionDevice(uint8_t header_type);

  /** @brief PCI デバイスを操作するための基礎データを格納する
   *
   * バス番号，デバイス番号，ファンクション番号はデバイスを特定するのに必須．
   * その他の情報は単に利便性のために加えてある．
   * */
  struct Device {
    uint8_t bus, device, function, header_type;
  };

  // #@@range_begin(var_devices)
  /** @brief ScanAllBus() により発見された PCI デバイスの一覧 */
  inline std::array<Device, 32> devices;
  /** @brief devices の有効な要素の数 */
  inline int num_device;
  /** @brief PCI デバイスをすべて探索し devices に格納する
   *
   * バス 0 から再帰的に PCI デバイスを探索し，devices の先頭から詰めて書き込む．
   * 発見したデバイスの数を num_devices に設定する．
   */
  Error ScanAllBus();
  // #@@range_end(var_devices)
}
