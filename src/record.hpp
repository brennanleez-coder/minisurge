#pragma once
#include <string>
#include <cstdint>
#include <vector>

struct Record {
    uint8_t type;
    uint32_t key_len{};
    uint32_t val_len{};
    std::string key;
    std::string value;
    std::vector<Record> children;

    size_t size() const;
};

void serialize(const Record& r, char* buffer);

bool deserialize(const char* data, size_t len, Record&);
