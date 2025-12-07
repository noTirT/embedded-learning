#include "hardware_sim.hpp"
#include "timer.hpp"
#include "motor_controller.hpp"
#include "types.hpp"
#include <chrono>
#include <iostream>
#include <thread>


uint32_t getCurrentTime() {
    return static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}

void ledAction(void* context) {
    (void)context;
    std::cout << "Led flickered" << std::endl;
}

int main() {
    struct FakeMotorRegisters fakeRegister;
    UartBuffer buffer;

    fakeRegister.ARR = 0;
    fakeRegister.CSR = 0;

    auto ledTimer = Timer(500, &ledAction);
    MotorController motor = MotorController(&fakeRegister, &buffer);

    auto last = getCurrentTime();

    while(true) {
        auto current = getCurrentTime();
        while(current - last >= 1) {
            ledTimer.onTick();
            motor.tick();
            last++;
        }
        ledTimer.update();
        motor.update();
        if(!buffer.isEmpty()) {
            char c;
            int success = buffer.pop(&c);
            if(success)
                std::cout << c << std::endl;
            else
                std::cout << "Failed to read from buffer" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    return 0;
}
