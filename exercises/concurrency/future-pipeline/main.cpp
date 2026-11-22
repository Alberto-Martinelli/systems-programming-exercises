#include "joining_thread.hpp"
#include <charconv>
#include <exception>
#include <future>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <thread>

static bool is_prime(int number) {
    if (number < 2) return false;
    if (number % 2 == 0) return number == 2;
    // Division avoids overflow from i*i near INT_MAX.
    for (int i = 3; i <= number / i; i += 2) {
        if (number % i == 0) return false;
    }
    return true;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " INTEGER\n";
        return 1;
    }
    try {
        std::promise<int> input_promise;
        auto input = input_promise.get_future();
        std::promise<bool> result_promise;
        auto result = result_promise.get_future();
        std::exception_ptr failure;
        {
            // Producer and worker propagate exceptions along the same pipeline.
            JoiningThread producer([&] {
                try {
                    std::string_view text(argv[1]);
                    int value = 0;
                    auto [end, error] = std::from_chars(text.data(),
                                                       text.data() + text.size(), value);
                    if (error != std::errc{} || end != text.data() + text.size())
                        throw std::invalid_argument("expected a signed 32-bit integer");
                    input_promise.set_value(value);
                } catch (...) {
                    input_promise.set_exception(std::current_exception());
                }
            });
            JoiningThread worker([&] {
                try {
                    result_promise.set_value(is_prime(input.get()));
                } catch (...) {
                    result_promise.set_exception(std::current_exception());
                }
            });
            JoiningThread consumer([&] {
                try {
                    const bool prime = result.get();
                    std::cout << (prime ? "prime" : "not prime") << '\n';
                } catch (...) {
                    failure = std::current_exception();
                }
            });
        }
        // Joining the consumer makes its write to failure visible here.
        if (failure) std::rethrow_exception(failure);
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
