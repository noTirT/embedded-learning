#pragma once

#include <algorithm>
#include <thread>
#include <iostream>
#include "types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <chrono>

struct Task {
    uint32_t period;
    uint32_t last_run;
    void * context;
    void(*callback)(void*);
    size_t priority;
};

template<size_t TaskCount>
class Scheduler {
private:
    std::array<Task, TaskCount> tasks;
    size_t tasks_insert_index = 0;
    UartBuffer* buffer;

    uint32_t getCurrentTime() {
        return static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    }

public:
    Scheduler(UartBuffer* buf):buffer(buf) {}

    void registerTask(uint32_t period, void(*callback)(void*), void* context, size_t priority = 1) {
        if(tasks_insert_index >= TaskCount) return;

        tasks[tasks_insert_index].period = period;
        tasks[tasks_insert_index].callback = callback;
        tasks[tasks_insert_index].last_run = getCurrentTime();
        tasks[tasks_insert_index].context = context;
        tasks[tasks_insert_index].priority = priority;
        tasks_insert_index ++;
    }

    void run() {
        std::sort(tasks.begin(), tasks.begin()+tasks_insert_index, [](const Task &lhs, const Task &rhs) {
            return lhs.priority > rhs.priority;
        });

        while(true) {
            auto current_time = getCurrentTime();
            for(size_t i = 0; i<tasks_insert_index; ++i) {
                while(current_time - tasks[i].last_run >= tasks[i].period) {
                    auto start = getCurrentTime();
                    tasks[i].callback(tasks[i].context);
                    auto end = getCurrentTime();

                    if(end - start > 1) {
                        std::cout << "[Warning] Task Priority " << tasks[i].priority << " took " << (end-start) << "ms!" << std::endl;
                    }
                    tasks[i].last_run += tasks[i].period;
                }
            }
            if(!buffer->isEmpty()) {
                char c;
                if(buffer->pop(&c))
                    std::cout << c << std::endl;
                else
                    std::cout << "Failed to read from buffer" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    }
};

