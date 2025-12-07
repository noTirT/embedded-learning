#pragma once

#include "hardware_sim.hpp"
#include "motor_driver.hpp"
#include "timer.hpp"
#include "types.hpp"

enum class MotorState {
    IDLE,
    ACCELERATING,
    CONSTANT_SPEED,
    BRAKING
};

class MotorController {
private:
    MotorState state;
    Timer timer;
    MotorDriver driver;
    UartBuffer *buffer;

    void runStateMachine() {
        switch(state) {
        case MotorState::IDLE:
            buffer->push('A');
            state = MotorState::ACCELERATING;
            driver.enableMotor();
            timer.setDuration(500);
            break;
        case MotorState::ACCELERATING:
            buffer->push('T');
            state = MotorState::CONSTANT_SPEED;
            driver.setSpeed(200);
            timer.setDuration(1000);
            break;
        case MotorState::CONSTANT_SPEED:
            buffer->push('B');
            state = MotorState::BRAKING;
            driver.disableMotor();
            driver.engageBrake();
            timer.setDuration(1500);
            break;
        case MotorState::BRAKING:
            buffer->push('I');
            state = MotorState::IDLE;
            driver.disengageBrake();
            driver.setSpeed(0);
            timer.setDuration(1000);
            break;
        }
    }
    static void staticCallback(void* obj) {
        MotorController* self = static_cast<MotorController*>(obj);

        self->runStateMachine();
    }
public:
    MotorController(FakeMotorRegisters* hw, UartBuffer *buff):
        state(MotorState::IDLE),
        timer(1000,&MotorController::staticCallback, this),
        driver(MotorDriver(hw)),
        buffer(buff)
    {}

    void update() {
        timer.update();
    }

    void tick() {
        timer.onTick();
    }
};
