#ifndef LASERSCANNINGPROJECT_SDCARD_H
#define LASERSCANNINGPROJECT_SDCARD_H

#include <climits>
#include <cstdint>
#include <libopencm3/stm32/spi.h>
#include <lib/gpio.h>

#include "../../middleware/ff14/source/ff.h"
#include "../../middleware/ff14/source/diskio.h"

class sdcard {
 public:

 private:
  const std::uint32_t _spi_ = SPI2;
  // Clock (SCK)
  gpio _sck_ = gpio(GPIOB, GPIO13);
  // Data Out (MOSI)
  gpio _mosi_ = gpio(GPIOB, GPIO15);
  // Data In (MISO)
  gpio _miso_ = gpio(GPIOB, GPIO14);
  // Chip Select
  gpio _cs_ = gpio(GPIOB, GPIO12);

  friend DSTATUS disk_initialize(BYTE pdrv);
  friend DSTATUS disk_status(BYTE pdrv);
  friend DRESULT disk_read(BYTE pdrv, BYTE* buff, LBA_t sector, UINT count);
  friend DRESULT disk_write(BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count);
  friend DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void* buff);
};

#endif //LASERSCANNINGPROJECT_SDCARD_H
