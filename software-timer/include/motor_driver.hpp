#pragma once

#include "hardware_sim.hpp"
#include <cstdint>

class MotorDriver {
private:
    FakeMotorRegisters* hw;

public:
    explicit MotorDriver(FakeMotorRegisters* hardware_addr): hw(hardware_addr) {}

    void enableMotor() {
        hw->CSR = hw->CSR | MotorBits::MASK_POWER;
        printRegisters(*hw);
    }

    void disableMotor() {
        hw->CSR = hw->CSR & ~MotorBits::MASK_POWER;
        printRegisters(*hw);
    }

    void engageBrake() {
        hw->CSR|=MotorBits::MASK_BRAKE;
        printRegisters(*hw);
    }

    void disengageBrake() {
        hw->CSR&=~MotorBits::MASK_BRAKE;
        printRegisters(*hw);
    }

    void setSpeed(uint8_t speed) {
        hw->ARR = speed;
        printRegisters(*hw);
    }
};
