#pragma once
#include "record.hpp"
#include <filesystem>
#include <fstream>
#include <mutex>

class WALWriter {
public:
  // Open (or create) the WAL file at path. 
  // If append=true, seek to EOF, else truncate.
  WALWriter(const std::filesystem::path& path, bool append = true);
  ~WALWriter();

  // Append one record; throws on I/O error.
  void append(const Record& r);

  // Flush to disk (fsync)
  void flush();

private:
int           fd_;   // POSIX file descriptor

  std::ofstream out_;
  std::mutex     mu_;           // protect multi-threaded calls
};
