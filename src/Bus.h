#pragma once
#include <cstdint>
#include "Cpu.h"
#include <array>

//Bus which every device associated with the NES is connected to 
class Bus {

public: //devices
  Cpu cpu;

  //fake RAM
  std::array<uint8_t, 64*1024> ram;

public: // read and write
  void write(uint16_t address, uint8_t data);
  uint8_t read (uint16_t address, bool b = false);
}
