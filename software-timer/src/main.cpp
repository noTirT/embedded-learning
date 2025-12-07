#include "hardware_sim.hpp"
#include "scheduler.hpp"
#include "timer.hpp"
#include "motor_controller.hpp"
#include "types.hpp"

void timer_onTick_wrapper(void* ctx) {
    static_cast<Timer*>(ctx)->onTick();
}

void timer_update_wrapper(void* ctx) {
    static_cast<Timer*>(ctx)->update();
}

void motor_tick_wrapper(void* ctx) {
    static_cast<MotorController*>(ctx)->tick();
}

void motor_update_wrapper(void* ctx) {
    static_cast<MotorController*>(ctx)->update();
}

void ledAction(void* context) {
    (void)context;
    std::cout << "Led blinked" << std::endl;
}

int main() {
    struct FakeMotorRegisters fakeRegister = {0,0};
    UartBuffer buffer;

    auto ledTimer = Timer(500, &ledAction);
    MotorController motor = MotorController(&fakeRegister, &buffer);

    Scheduler<4> scheduler = Scheduler<4>(&buffer);

    scheduler.registerTask(1, timer_onTick_wrapper, &ledTimer, 10);
    scheduler.registerTask(10, timer_update_wrapper, &ledTimer);
    scheduler.registerTask(1, motor_tick_wrapper, &motor, 10);
    scheduler.registerTask(10, motor_update_wrapper, &motor, 5);

    scheduler.run();

    return 0;
}
