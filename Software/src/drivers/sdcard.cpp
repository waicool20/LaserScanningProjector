#include "sdcard.h"



extern "C" DSTATUS disk_initialize(BYTE pdrv) {
}

extern "C" DSTATUS disk_status(BYTE pdrv) {

}

extern "C" DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) {

}

extern "C" DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count) {

}

extern "C" DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff) {

}
