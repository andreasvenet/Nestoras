#pragma once

// Ths is required for translation table and disassembler. The table
// could be implemented straight up as an array, but I used a vector.
#include <vector>

// These are required for disassembler. If you dont require disassembly
// then just remove the function.
#include <string>
#include <map>

// Emulation Behaviour Logging ======================================
// Uncomment this to create a logfile entry for each clock tick of 
// the CPU. Beware: this slows down emulation considerably and
// generates extremely large files. I recommend "glogg" to view the
// data as it is designed to handle enormous files.
//
//#define LOGMODE // <- Uncomment me to enable logging!

#ifdef LOGMODE
#include <stdio.h>
#endif


// Forward declaration of generic communications bus class to
// prevent circular inclusions
class Bus;

// The 6502 Emulation Class.
class Cpu {
public:
  Cpu();
  ~Cpu();

public:

  // Registers
  uint8_t accumulator = 0x00;
  uint8_t x = 0x00;
  uint8_t y = 0x00;
  uint8_t stackpointer = 0x00; // Points to a location on the bus
  uint16_t pc = 0x0000; // Program counter
  uint8_t status = 0x00; // Status Regiester

  // Interrupt functions
  void reset();
  void irq();
  void nmi();

  void clock(); 

  // Returns true if current instruction has completed
  bool complete();

  // aporia
  // Produces a map of strings, with keys equivalent to instruction start locations
	// in memory, for the specified address range
	std::map<uint16_t, std::string> disassemble(uint16_t nStart, uint16_t nStop);

  // Status register bits
  enum FLAGS6502 {
    C = (1 << 0) // Carry bit
    Z = (1 << 1) // Zero (set usually when the result of an operation equals zero)
    I = (1 << 2) // Disable interrupts
    D = (1 << 3) // Decimal mode (unused as the NES did not implement such mode in hardware) 
    B = (1 << 4) // Break operation has been called
    U = (1 << 5) // Unused
    V = (1 << 6) // Overflow
    N = (1 << 7) // Negative
  }



  void connectBus(Bus *n) { bus = n; }

private:
  // Connect to the Bus
  Bus *bus = nullptr;
  void write(uint16_t address, uint8_t data);
  uint8_t read (uint16_t address);

  // Functions to access status register bits
  uint8_t getFlag(FLAGS6502 f);
  void setFlag(FLAGS6502 f, bool v);

  // The read location of data can come from two sources, a memory address, or
	// its immediately available as part of the instruction. This function decides
	// depending on address mode of instruction byte
	uint8_t fetch();



  struct INSTRUCTION
	{
		std::string name; // Textual representation of the instruction		
		uint8_t (Cpu::*operate )(void) = nullptr; // Function pointer to the implementation of the given opcode
		uint8_t (Cpu::*addrmode)(void) = nullptr; // Function pointer to the implementation of the addressing mode by the instruction
		uint8_t cycles = 0; // Number of cycles required for the cpu to execute the instruction
	};

	std::vector<INSTRUCTION> lookup;

    // Addressing modes
  uint8_t IMP();  uint8_t IMM();
  uint8_t ZP0();  uint8_t ZPX();
  uint8_t ZPY();  uint8_t REL();
  uint8_t ABS();  uint8_t ABX();
  uint8_t ABY();  uint8_t IND();
  uint8_t IZY();  uint8_t IZY();

  // Opcodes
  uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
	uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
	uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
	uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
	uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
	uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
	uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
	uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
	uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
	uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
	uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
	uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
	uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
	uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();

  uint8_t XXX(); // Used to catch illegal opcodes

#ifdef LOGMODE
private:
	FILE* logfile = nullptr;
#endif

};