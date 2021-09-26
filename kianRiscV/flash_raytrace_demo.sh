# flash riscv raytracer binary
iceFUNprog -o $((64*1024*4)) firmware.bin 
# flash riscv rv32im cpu
iceFUNprog -v kianv_soc.bit

