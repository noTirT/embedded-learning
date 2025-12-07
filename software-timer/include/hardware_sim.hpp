#pragma once

#include <bitset>
#include <cstdint>
#include <iostream>

struct FakeMotorRegisters {
    volatile uint8_t CSR;
    volatile uint8_t ARR;
};
namespace MotorBits {
constexpr uint8_t MASK_POWER = (1 << 0);
constexpr uint8_t MASK_DIRECTION = (1 << 1);
constexpr uint8_t MASK_BRAKE = (1 << 2);
}

void printRegisters(const FakeMotorRegisters& regs) {
    std::cout << "  [HW Monitor] CSR: " << std::bitset<8>(regs.CSR) << " | Speed (ARR): "<< (int)regs.ARR <<std::endl;
}
