# MiniSurge Storage Engine

## Prerequisites
- CMake 3.20+
- C++17 compiler
- Redis server (install via `brew install redis`)

## Build Commands
```bash
# Configure project
cmake -S . -B build

# Build all targets
cmake --build build --parallel

# Build specific target (e.g., tests)
cmake --build build --target record_benchmark


## Test Commands
# Run all tests
ctest --test-dir build --output-on-failure

# Run specific test suites
ctest --test-dir build -R "record.*|wal.*|memtable.*"

# Run MemTable tests only
ctest --test-dir build -R memtable.*

## Redis
# Start Redis server

redis-server --daemonize yes

# Stop Redis server

redis-cli shutdown

# Check Redis status

redis-cli ping

## Benchmarking
# Run all benchmarks
./build/tests/record_benchmark

# WAL performance test
./build/tests/record_benchmark --benchmark_filter=WALAppendThroughput

# MemTable vs Redis comparison
./build/tests/record_benchmark --benchmark_filter="MemTablePut|RedisSet"

# Long-running benchmark (2s per test)
./build/tests/record_benchmark --benchmark_min_time=2