#include "joining_thread.hpp"
#include <charconv>
#include <exception>
#include <iostream>
#include <semaphore>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>

static int bounded_number(const char* argument, int maximum) {
    std::string_view text(argument);
    int value = 0;
    auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);
    if (error != std::errc{} || end != text.data() + text.size() ||
        value < 1 || value > maximum)
        throw std::invalid_argument("arguments must be positive integers within limits");
    return value;
}

int main(int argc, char** argv) {
    if (argc != 1 && argc != 3) {
        std::cerr << "Usage: " << argv[0] << " [CYCLES WIDTH]; limits 1000 and 1000\n";
        return 1;
    }
    try {
        const int cycles = argc == 3 ? bounded_number(argv[1], 1000) : 3;
        const int width = argc == 3 ? bounded_number(argv[2], 1000) : 5;
        std::binary_semaphore plus{1}, minus{0}, star{0}, newline{0}, acknowledged{0};
        // Do not let workers enter the protocol until all four were created.
        std::binary_semaphore ready[4]{
            std::binary_semaphore{0}, std::binary_semaphore{0},
            std::binary_semaphore{0}, std::binary_semaphore{0}};
        bool cancelled = false;
        auto symbols = [&](char symbol, std::binary_semaphore& turn,
                           std::binary_semaphore& next, int slot) {
            ready[slot].acquire();
            if (cancelled) return;
            for (int cycle = 0; cycle < cycles; ++cycle) {
                turn.acquire();
                std::cout << std::string(static_cast<std::size_t>(width), symbol);
                newline.release();
                acknowledged.acquire();
                next.release();
            }
        };
        JoiningThread workers[4];
        try {
            workers[0] = JoiningThread(symbols, '+', std::ref(plus), std::ref(minus), 0);
            workers[1] = JoiningThread(symbols, '-', std::ref(minus), std::ref(star), 1);
            workers[2] = JoiningThread(symbols, '*', std::ref(star), std::ref(plus), 2);
            workers[3] = JoiningThread([&] {
                ready[3].acquire();
                if (cancelled) return;
                for (int line = 0; line < cycles * 3; ++line) {
                    newline.acquire();
                    std::cout << '\n';
                    acknowledged.release();
                }
            });
        } catch (...) {
            // Each started worker observes cancellation after its own acquire.
            cancelled = true;
            for (auto& gate : ready) gate.release();
            throw;
        }
        for (auto& gate : ready) gate.release();
        // Worker destructors join before the semaphores leave scope.
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
