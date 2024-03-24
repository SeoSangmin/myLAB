#include <iostream>
#include <future>
#include <chrono>
#include <thread>
#include <unistd.h>

int asyncTask(int value) {
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return value * 2;
}

int main() {
    std::cout << "Main thread ID: " << std::this_thread::get_id() << std::endl;

    std::future<int> result1 = std::async(std::launch::async, asyncTask, 10);
    std::future<int> result2 = std::async(std::launch::async, asyncTask, 20);

    std::cout << "Waiting for results..." << std::endl;

    int value1 = result1.get();
    int value2 = result2.get();

    std::cout << "Result 1: " << value1 << std::endl;
    std::cout << "Result 2: " << value2 << std::endl;

	sleep(20);
    return 0;
}
