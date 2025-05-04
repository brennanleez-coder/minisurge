#include "wal_reader.hpp"
#include "record.hpp"
#include <fcntl.h>      // For O_RDONLY
#include <unistd.h>     // For read/lseek/close
#include <cstring>       // For memcpy
#include <vector>       // For std::vector

WALReader::WALReader(const std::filesystem::path& path)
  : path_(path) {}

std::vector<Record> WALReader::replay() {
    std::vector<Record> out;
    int fd = ::open(path_.c_str(), O_RDONLY);
    if (fd < 0) return out;

    while(true) {
        // Read header first to determine record size
        char header[9];
        ssize_t header_read = ::read(fd, header, 9);
        if(header_read != 9) break;

        // Parse lengths without full deserialization
        uint32_t key_len, val_len;
        std::memcpy(&key_len, header + 1, sizeof(key_len));
        std::memcpy(&val_len, header + 5, sizeof(val_len));
        
        // Calculate full record size including header
        size_t full_size = 9 + key_len + val_len;
        std::vector<char> full_record(full_size);
        std::memcpy(full_record.data(), header, 9);
        
        // Read remaining data directly into buffer
        ssize_t data_read = ::read(fd, full_record.data() + 9, full_size - 9);
        if(data_read != static_cast<ssize_t>(full_size - 9)) break;

        // Deserialize the full record in one pass
        Record r;
        if(deserialize(full_record.data(), full_size, r)) {
            out.push_back(std::move(r));
        } else {
            break;
        }
    }
    
    ::close(fd);
    return out;
}
