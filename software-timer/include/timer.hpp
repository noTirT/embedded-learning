#pragma once

#include <utility>
#include <cstdint>

class Timer {
public:
    using TimerCallback = void(*)(void*);
private:
    uint32_t duration;
    uint32_t remaining_ms;
    TimerCallback action;
    void* context;
    bool active;
    bool period_elapsed;

public:
    explicit Timer(uint32_t duration_ms, TimerCallback func, void* ctx = nullptr): duration(duration_ms), remaining_ms(duration_ms), action(std::move(func)), active(true), period_elapsed(false), context(ctx) {
    }

    void onTick() {
        if(active && remaining_ms > 0 ) {
            remaining_ms--;
            if(remaining_ms == 0) {
                period_elapsed = true;
            }
        }
    }

    void update() {
        if(period_elapsed) {
            if(action) {
                action(context);
            }
            resetTimer();
        }
    }

    void resetTimer() {
        remaining_ms = duration;
        period_elapsed = false;
        active = true;
    }

    void setDuration(uint32_t new_duration) {
        duration = new_duration;
    }
};


