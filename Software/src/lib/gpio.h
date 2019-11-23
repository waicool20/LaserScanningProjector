#pragma once

#include <climits>
#include <cstdint>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>
#include <array>

#include "helper.h"

class gpio {
  public:
    struct mmio {
      std::uint32_t mode;
      alignas(std::uint32_t) std::uint16_t otype;
      std::uint32_t ospeed;
      std::uint32_t pupd;
      alignas(std::uint32_t) std::uint16_t id;
      alignas(std::uint32_t) std::uint16_t od;
      std::uint32_t bsr;
      alignas(std::uint32_t) std::uint16_t lck;
      std::uint64_t af;
      alignas(std::uint32_t) std::uint16_t br;
    } __attribute__((packed));
    static_assert(sizeof(mmio) == mmio_size(0x28));

    gpio(std::uint32_t gpiox, std::uint16_t p);

    void setup(std::uint8_t mode, std::uint8_t pupd);
    void output_opts(std::uint8_t otype, std::uint8_t speed);
    void set_af(uint8_t af_num);
    void enable_ext_interrupt(exti_trigger_type type);

    void state(bool new_state);
    void toggle();

    bool get();
    uint8_t get_pin_num() const;
    exti_trigger_type get_exti_trigger_type() const;
    void set_exti_callback(void (*action)(bool));

    volatile mmio *handle();

    const std::uint32_t port;
    const std::uint16_t pin;

  private:
    friend void exti0_isr();
    friend void exti1_isr();
    friend void exti2_tsc_isr();
    friend void exti3_isr();
    friend void exti4_isr();
    friend void exti9_5_isr();
    friend void exti15_10_isr();
    std::uint8_t _pin_num = 0;
    exti_trigger_type _trigger_type = EXTI_TRIGGER_BOTH;
    static std::array<void (*)(bool state), 16> pin_callback;
    static std::array<gpio*, 16> exti_pins;
    static void callback_nop(bool);
};
