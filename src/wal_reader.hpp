#pragma once

#include "record.hpp"
#include <filesystem>
#include <vector>

/// Reads back all valid Records from a WAL file.
/// Stops on I/O error or incomplete tail.
class WALReader {
public:
    explicit WALReader(const std::filesystem::path& path);
    std::vector<Record> replay();

private:
    std::filesystem::path path_;
};
