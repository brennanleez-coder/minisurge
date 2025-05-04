#include "wal.hpp"

#include <system_error>
#include <unistd.h>     // ::write, ::fsync, ::close
#include <fcntl.h>      // open flags
#include <vector>

WALWriter::WALWriter(const std::filesystem::path& path, bool append) {
  int flags = O_CREAT | O_WRONLY | (append ? O_APPEND : O_TRUNC);
  fd_ = ::open(path.c_str(), flags, 0644);
  if (fd_ < 0) {
    throw std::system_error(errno, std::generic_category(), "opening WAL");
  }
}

WALWriter::~WALWriter() {
  if (fd_ >= 0) {
    ::close(fd_);
  }
}

void WALWriter::append(const Record& r) {
  // Compute needed buffer size (9 byte header + payload)
  size_t bufSize = 9 + r.key_len + r.val_len;  // 1(type) + 4+4(len) + key + val

  std::vector<char> buf(bufSize);
  serialize(r, buf.data());

  std::lock_guard lock(mu_);
  ssize_t written = ::write(fd_, buf.data(), bufSize);
  if (written < 0 || size_t(written) != bufSize) {
    throw std::system_error(errno, std::generic_category(), "writing WAL");
  }
}

void WALWriter::flush() {
  std::lock_guard lock(mu_);
  if (::fsync(fd_) < 0) {
    throw std::system_error(errno, std::generic_category(), "fsync WAL");
  }
}
