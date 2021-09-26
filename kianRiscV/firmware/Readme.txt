took and adjusted linker script and crt0_spiflash.S from bruno and mandel and raytrace
https://github.com/BrunoLevy/learn-fpga
and some c basic stdlib stuff from
https://github.com/cliffordwolf/picorv32

Raytracer Demo precompiled
--------------------------
you can flash on fun the firmware.bin  is precompiled raytracer for oled ssd1331,
cpu fetches instruction only from nor flash

iceFUNprog -o $((64*1024*4)) firmware.bin

