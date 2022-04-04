#ifndef ELF_NAMES_H
#define ELF_NAMES_H

#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#endif

// translate value of e_type field to a string
const char *get_type_name(uint16_t value);

// translate value of e_machine field to a string
const char *get_machine_name(uint16_t value);

#ifdef __cplusplus
}
#endif

#endif // ELF_NAMES_H
