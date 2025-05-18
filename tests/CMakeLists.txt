# tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)
project(minisurge_tests)

include(FetchContent)


FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)


#    This sets up imported targets *and* makes the helper module available.
find_package(Catch2 3.5.2 CONFIG REQUIRED)

include(CTest)
include(Catch)   # <— defines catch_discover_tests()

function(add_minisurge_catch_test TEST_NAME SOURCE_FILE TEST_PREFIX)
    add_executable(${TEST_NAME} ${SOURCE_FILE})

    # Ensure test executables can find headers from the main project source
    target_include_directories(${TEST_NAME}
        PRIVATE ${CMAKE_SOURCE_DIR}/src
    )

    target_link_libraries(${TEST_NAME}
        PRIVATE
        minisurge_core       # your library
        Catch2::Catch2WithMain
    )

    catch_discover_tests(${TEST_NAME}
        TEST_PREFIX "${TEST_PREFIX}."
    )
endfunction()

add_minisurge_catch_test(record_test record_test.cpp record)
add_minisurge_catch_test(wal_test wal_test.cpp wal)
add_minisurge_catch_test(memtable_test memtable_test.cpp memtable)
add_minisurge_catch_test(bloom_filter_test bloom_filter_test.cpp bloom_filter)

find_package(benchmark REQUIRED)
add_executable(record_benchmark benchmark.cpp)
target_link_libraries(record_benchmark PRIVATE minisurge_core benchmark::benchmark)