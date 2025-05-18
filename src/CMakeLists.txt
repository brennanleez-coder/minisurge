# src/CMakeLists.txt

add_library(minisurge_core STATIC
  sources/record.cpp
  sources/wal.cpp
  sources/wal_reader.cpp
  sources/sstable.cpp
  sources/memtable.cpp
  sources/bloom_filter.cpp
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(minisurge_core
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)