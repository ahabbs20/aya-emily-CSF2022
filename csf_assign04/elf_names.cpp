#include "elf_names.h"

struct MagicValue {
  uint16_t value;
  uint16_t end_value;
  const char *name;
};

const struct MagicValue s_obj_types[] = {
  { 0x00 , 0x00 , "ET_NONE" },
  { 0x01 , 0x01 , "ET_REL" },
  { 0x02 , 0x02 , "ET_EXEC" },
  { 0x03 , 0x03 , "ET_DYN" },
  { 0x04 , 0x04 , "ET_CORE" },
  { 0xFE00 , 0xFE00 , "ET_LOOS" },
  { 0xFEFF , 0xFEFF , "ET_HIOS" },
  { 0xFF00 , 0xFF00 , "ET_LOPROC" },
  { 0xFFFF , 0xFFFF , "ET_HIPROC" },
};

const struct MagicValue s_machine_names[] = {
  { 0x00 , 0x00 , "No specific instruction set" },
  { 0x01 , 0x01 , "AT&T WE 32100" },
  { 0x02 , 0x02 , "SPARC" },
  { 0x03 , 0x03 , "x86" },
  { 0x04 , 0x04 , "Motorola 68000 (M68k)" },
  { 0x05 , 0x05 , "Motorola 88000 (M88k)" },
  { 0x06 , 0x06 , "Intel MCU" },
  { 0x07 , 0x07 , "Intel 80860" },
  { 0x08 , 0x08 , "MIPS" },
  { 0x09 , 0x09 , "IBM System/370" },
  { 0x0A , 0x0A , "MIPS RS3000 Little-endian" },
  { 0x0B , 0x0D , "Reserved for future use" },
  { 0x0E , 0x0E , "Hewlett-Packard PA-RISC" },
  { 0x0F , 0x0F , "Reserved for future use" },
  { 0x13 , 0x13 , "Intel 80960" },
  { 0x14 , 0x14 , "PowerPC" },
  { 0x15 , 0x15 , "PowerPC (64-bit)" },
  { 0x16 , 0x16 , "S390, including S390x" },
  { 0x17 , 0x17 , "IBM SPU/SPC" },
  { 0x18 , 0x23 , "Reserved for future use" },
  { 0x24 , 0x24 , "NEC V800" },
  { 0x25 , 0x25 , "Fujitsu FR20" },
  { 0x26 , 0x26 , "TRW RH-32" },
  { 0x27 , 0x27 , "Motorola RCE" },
  { 0x28 , 0x28 , "ARM (up to ARMv7/Aarch32)" },
  { 0x29 , 0x29 , "Digital Alpha" },
  { 0x2A , 0x2A , "SuperH" },
  { 0x2B , 0x2B , "SPARC Version 9" },
  { 0x2C , 0x2C , "Siemens TriCore embedded processor" },
  { 0x2D , 0x2D , "Argonaut RISC Core" },
  { 0x2E , 0x2E , "Hitachi H8/300" },
  { 0x2F , 0x2F , "Hitachi H8/300H" },
  { 0x30 , 0x30 , "Hitachi H8S" },
  { 0x31 , 0x31 , "Hitachi H8/500" },
  { 0x32 , 0x32 , "IA-64" },
  { 0x33 , 0x33 , "Stanford MIPS-X" },
  { 0x34 , 0x34 , "Motorola ColdFire" },
  { 0x35 , 0x35 , "Motorola M68HC12" },
  { 0x36 , 0x36 , "Fujitsu MMA Multimedia Accelerator" },
  { 0x37 , 0x37 , "Siemens PCP" },
  { 0x38 , 0x38 , "Sony nCPU embedded RISC processor" },
  { 0x39 , 0x39 , "Denso NDR1 microprocessor" },
  { 0x3A , 0x3A , "Motorola Star*Core processor" },
  { 0x3B , 0x3B , "Toyota ME16 processor" },
  { 0x3C , 0x3C , "STMicroelectronics ST100 processor" },
  { 0x3D , 0x3D , "Advanced Logic Corp. TinyJ embedded processor family" },
  { 0x3E , 0x3E , "AMD x86-64" },
  { 0x8C , 0x8C , "TMS320C6000 Family" },
  { 0xAF , 0xAF , "MCST Elbrus e2k" },
  { 0xB7 , 0xB7 , "ARM 64-bits (ARMv8/Aarch64)" },
  { 0xF3 , 0xF3 , "RISC-V" },
  { 0xF7 , 0xF7 , "Berkeley Packet Filter" },
  { 0x101 , 0x101 , "WDC 65C816" },
};

static const char *lookup_desc(
  uint16_t value, const struct MagicValue *table,
  unsigned table_len, const char *default_desc) {

  for (unsigned i = 0; i < table_len; i++) {
    if (value >= table[i].value && value <= table[i].end_value) {
      return table[i].name;
    }
  }
  return default_desc;
}

// translate value of e_type field to a string
const char *get_type_name(uint16_t value) {
  unsigned num_obj_types =
    sizeof(s_obj_types)/sizeof(struct MagicValue);
  return lookup_desc(value, s_obj_types, num_obj_types, "Unknown object type");
}

// translate value of e_machine field to a string
const char *get_machine_name(uint16_t value) {
  unsigned num_machine_names =
    sizeof(s_machine_names)/sizeof(struct MagicValue);
  return lookup_desc(value, s_machine_names, num_machine_names, "Unknown instruction set");
}
