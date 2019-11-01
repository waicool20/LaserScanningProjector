#pragma once

#include <cstdint>

/**
 * \brief Computes the expected size of an MMIO struct.
 *
 * \param mmio_last_reg_offset offset for the last register of the peripheral.
 * \return Expected size of the MMIO struct in bytes. Use this like `static_assert(sizeof(mmio) == mmio_size(...));`
 */
constexpr std::size_t mmio_size(std::size_t mmio_last_reg_offset) {
  return (mmio_last_reg_offset + 0x04);
}
