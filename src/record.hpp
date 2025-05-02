#pragma once
#include <string>
#include <cstdint>

struct Record {
    uint32_t key_len{};
    uint32_t val_len{};
    std::string key;
    std::string value;
};

void serialize(const Record& r, char* buffer);

bool deserialize(const char* data, size_t len, Record&);
