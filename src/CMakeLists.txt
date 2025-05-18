# src/CMakeLists.txt

add_library(minisurge_core STATIC
  record.cpp
  wal.cpp
  wal_reader.cpp
  sstable.cpp
  memtable.cpp
  bloom_filter.cpp
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(minisurge_core
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
