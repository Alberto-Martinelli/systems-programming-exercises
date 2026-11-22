#include "joining_thread.hpp"
#include <algorithm>
#include <bit>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

namespace {
constexpr std::uint32_t max_values_per_file = 1000000;

std::uint32_t read_u32(std::istream& input) {
    unsigned char bytes[4];
    if (!input.read(reinterpret_cast<char*>(bytes), sizeof bytes))
        throw std::runtime_error("truncated binary integer");
    return static_cast<std::uint32_t>(bytes[0]) |
           (static_cast<std::uint32_t>(bytes[1]) << 8) |
           (static_cast<std::uint32_t>(bytes[2]) << 16) |
           (static_cast<std::uint32_t>(bytes[3]) << 24);
}

void write_u32(std::ostream& output, std::uint32_t value) {
    unsigned char bytes[4];
    for (unsigned int i = 0; i < 4; ++i)
        bytes[i] = static_cast<unsigned char>(value >> (8 * i));
    if (!output.write(reinterpret_cast<const char*>(bytes), sizeof bytes))
        throw std::runtime_error("binary output write failed");
}

std::vector<std::int32_t> read_and_sort(const std::string& path) {
    const auto bytes = std::filesystem::file_size(path);
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("cannot open input: " + path);
    const auto count = read_u32(input);
    if (count > max_values_per_file || bytes != 4ULL + 4ULL * count)
        throw std::runtime_error("invalid count or file length: " + path);
    std::vector<std::int32_t> values(count);
    for (auto& value : values) value = std::bit_cast<std::int32_t>(read_u32(input));
    std::sort(values.begin(), values.end());
    return values;
}

std::vector<std::int32_t> merge_runs(
    const std::vector<std::vector<std::int32_t>>& runs) {
    using Entry = std::tuple<std::int32_t, std::size_t, std::size_t>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> heap;
    std::size_t total = 0;
    for (std::size_t i = 0; i < runs.size(); ++i) {
        total += runs[i].size();
        if (!runs[i].empty()) heap.emplace(runs[i][0], i, 0);
    }
    if (total > std::numeric_limits<std::uint32_t>::max())
        throw std::runtime_error("output count exceeds binary format");
    std::vector<std::int32_t> merged;
    merged.reserve(total);
    while (!heap.empty()) {
        auto [value, run, position] = heap.top();
        heap.pop();
        merged.push_back(value);
        if (++position < runs[run].size())
            heap.emplace(runs[run][position], run, position);
    }
    return merged;
}
} // namespace

int main(int argc, char** argv) {
    if (argc < 3 || argc > 34) {
        std::cerr << "Usage: " << argv[0] << " INPUT... OUTPUT; at most 32 inputs\n";
        return 1;
    }
    try {
        const auto count = static_cast<std::size_t>(argc - 2);
        std::vector<std::vector<std::int32_t>> runs(count);
        std::vector<std::exception_ptr> errors(count);
        {
            std::vector<JoiningThread> workers;
            workers.reserve(count);
            for (std::size_t i = 0; i < count; ++i) {
                workers.emplace_back([&, i] {
                    try {
                        runs[i] = read_and_sort(argv[i + 1]);
                    } catch (...) {
                        errors[i] = std::current_exception();
                    }
                });
            }
            // Scope-bound joins also run if constructing a later worker throws.
        }
        for (const auto& error : errors) {
            if (error) std::rethrow_exception(error);
        }
        const auto merged = merge_runs(runs);
        std::ofstream output(argv[argc - 1], std::ios::binary | std::ios::trunc);
        if (!output) throw std::runtime_error("cannot open output");
        write_u32(output, static_cast<std::uint32_t>(merged.size()));
        for (auto value : merged)
            write_u32(output, std::bit_cast<std::uint32_t>(value));
        output.close();
        if (!output) throw std::runtime_error("cannot close output");
        std::cout << "Sorted " << merged.size() << " values from " << count << " files\n";
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
