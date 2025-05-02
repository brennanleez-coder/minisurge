add_library(minisurge_core STATIC
  record.cpp
  # add other source files here later
)

target_include_directories(minisurge_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
