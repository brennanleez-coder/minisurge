# Makefile for minisurge

# Variables
BUILD_DIR := build
# Path to your benchmark executable, as per your README
BENCHMARK_EXECUTABLE := $(BUILD_DIR)/tests/record_benchmark

# Phony targets: these are not files
.PHONY: all compile test benchmark clean help

# Default target
all: compile

# Help target to display available commands
help:
	@echo "Available commands for minisurge:"
	@echo "  make all        - Configure and compile the project (default)"
	@echo "  make compile    - Configure and compile the project"
	@echo "  make test       - Compile and run tests"
	@echo "  make benchmark  - Compile and run benchmarks"
	@echo "  make clean      - Remove build artifacts"
	@echo "  make help       - Show this help message"

# Configure and compile the project
compile:
	@echo "=== Configuring and Building minisurge ==="
	@mkdir -p $(BUILD_DIR)
	@echo "--- Configuring project ---"
	@cmake -S . -B $(BUILD_DIR)
	@echo "\n--- Building all targets ---"
	@cmake --build $(BUILD_DIR) --parallel
	@echo "\nminisurge compiled successfully in $(BUILD_DIR)/"

# Compile and run tests
test: compile
	@echo "\n=== Running minisurge Tests ==="
	@ctest --test-dir $(BUILD_DIR) --output-on-failure

# Compile and run benchmarks
benchmark: compile
	@echo "\n=== Running minisurge Benchmarks ==="
	@if [ -f "$(BENCHMARK_EXECUTABLE)" ]; then \
		$(BENCHMARK_EXECUTABLE); \
	else \
		echo "Benchmark executable not found at $(BENCHMARK_EXECUTABLE)."; \
		echo "Please ensure 'record_benchmark' is built by CMake and located in '$(BUILD_DIR)/tests/'."; \
		exit 1; \
	fi

# Remove build artifacts
clean:
	@echo "=== Cleaning minisurge ==="
	@rm -rf $(BUILD_DIR)
	@echo "Build directory '$(BUILD_DIR)' removed."