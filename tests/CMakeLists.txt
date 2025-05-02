include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  
  GIT_TAG v3.5.2
)

FetchContent_MakeAvailable(Catch2)

add_executable(record_test record_test.cpp)

target_include_directories(record_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(record_test
  PRIVATE
    minisurge_core
    Catch2::Catch2WithMain
)

add_test(NAME RecordTest COMMAND record_test)
