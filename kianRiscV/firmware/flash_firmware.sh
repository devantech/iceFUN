#!/bin/sh
FIRMWARE=firmware.bin

# ./flash_firmware.sh spi_nor_fun.ld main_raytrace.c
# ./flash_firmware.sh spi_nor2bram_fun.ld main_mandel.c
rm *.o; ./kianv_firmware_gcc.sh $1 $2
iceFUNprog -o $((64*1024*4)) $FIRMWARE

