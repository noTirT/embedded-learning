#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
struct Task {
    uint32_t period;
    uint32_t last_run;
    void(*callback)(void);
};

template<size_t TaskCount>
class Scheduler {
private:
    std::array<Task, TaskCount> tasks;
    size_t tasks_insert_index = 0;

public:
    void registerTask(uint32_t period, void(*callback)(void)) {
        if(tasks_insert_index >= TaskCount) return;

        tasks[tasks_insert_index].period = period;
        tasks[tasks_insert_index].callback = &callback;
        tasks[tasks_insert_index].last_run = 0;
        tasks_insert_index ++;
    }

    void run() {

    }
};

