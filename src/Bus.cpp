#include "Bus.h"

Bus::Bus() {
  //clear ram
  for (auto &i : ram) i = 0x00;

  cpu.connectBus(this);
}

Bus::~Bus() {

}

void Bus:: write(uint16_t address, uint8_t data) {
  if (address >= 0x0000 && address <= 0xFFFF)
    return ram[address] = data;
}

uint8_t read (uint16_t address, bool flag) {
  if (address >= 0x0000 && address <= 0xFFFF)
    return ram[address];
  
  return 0x00;
}