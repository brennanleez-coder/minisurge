# src/CMakeLists.txt

add_library(minisurge_core STATIC
  record.cpp
  wal.cpp
  wal_reader.cpp 
)

target_include_directories(minisurge_core
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
