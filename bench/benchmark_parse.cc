//
// Created by fl on 1/24/19.
//
#include <benchmark/benchmark.h>

#include <src/Document.h>
#include <src/FileReadStream.h>
#include <src/JsonWriter.h>
#include <src/StringWriteStream.h>


template <class ...Args>
void bench_read_parse(benchmark::State &s, Args &&... args) {
    for (auto _: s) {
        FILE *input = fopen(args..., "r");
        if (input == nullptr) {
            exit(1);
        }
        jfson::Document doc;
        jfson::FileReadStream is(input);
        benchmark::DoNotOptimize(is);
        fclose(input);
        if (doc.parseStream(is) != jfson::PARSE_OK) {
            exit(1);
        }
    }
}

template <class ...Args>
void bench_strlen(benchmark::State &s, Args &&... args) {
    for (auto _:s) {
        FILE *input = fopen(args..., "r");
        if (input == nullptr)
            exit(1);
        jfson::FileReadStream is(input);
        benchmark::DoNotOptimize(is);
        fclose(input);
        int length = 0;
        while (is.hasNext()) {
            char ch = is.next();
            if (ch != '\0') {
                length += 1;
            }
        }
    }
}

//BENCHMARK_CAPTURE(bench_read_parse, many_double, "/home/fl/CLionProjects/JFSON/canada.json")->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(bench_read_parse, simple, "/home/fl/CLionProjects/JFSON/citm_catalog.json")->Unit(benchmark::kMillisecond);

//BENCHMARK_CAPTURE(bench_strlen, many_double, "/home/fl/CLionProjects/JFSON/canada.json")->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(bench_strlen, simple, "/home/fl/CLionProjects/JFSON/citm_catalog.json")->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();

/**
 *
Running /home/fl/CLionProjects/JFSON/cmake-build-debug/bin/benchmark_parse
Run on (8 X 4200 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 256K (x4)
  L3 Unified 8192K (x1)
Load Average: 0.69, 0.98, 0.93
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations
------------------------------------------------------------------
bench_read_parse/simple        118 ms          118 ms            6
bench_strlen/simple           53.4 ms         53.4 ms           13

Process finished with exit code 0

 **/
