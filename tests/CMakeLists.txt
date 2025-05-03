# tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)
project(minisurge_tests)

include(FetchContent)

# 1) Fetch Catch2
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)

# 2) Now find the Catch2 CMake package
#    This sets up imported targets *and* makes the helper module available.
find_package(Catch2 3.5.2 CONFIG REQUIRED)

# 3) Enable CTest and load Catch’s helper
include(CTest)
include(Catch)   # <— defines catch_discover_tests()

# 4) Declare your test executable
add_executable(record_test
  record_test.cpp
)

# make sure it sees your Record headers
target_include_directories(record_test
  PRIVATE ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(record_test
  PRIVATE
    minisurge_core       # your library
    Catch2::Catch2WithMain
)

# 5) Discover every TEST_CASE in record_test
catch_discover_tests(record_test
  # optional: prefix the CTest test names
  TEST_PREFIX "record."
  # optional: pass extra flags to your tests
  # EXTRA_ARGS --log-level debug
)
# New: WALReader tests
add_executable(wal_test
  wal_test.cpp
)

target_include_directories(wal_test
  PRIVATE ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(wal_test
  PRIVATE
    minisurge_core
    Catch2::Catch2WithMain
)

catch_discover_tests(wal_test
  TEST_PREFIX "wal."
)
