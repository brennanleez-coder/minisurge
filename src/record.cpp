#include "record.hpp"
#include <cstring>

void serialize(const Record& r, char* buffer) {
    // Copy key_len and val_len
    std::memcpy(buffer, &r.key_len, sizeof(r.key_len));
    std::memcpy(buffer + sizeof(r.key_len), &r.val_len, sizeof(r.val_len));

    // Copy key and value data
    std::memcpy(buffer + sizeof(r.key_len) + sizeof(r.val_len), r.key.data(), r.key_len);
    std::memcpy(buffer + sizeof(r.key_len) + sizeof(r.val_len) + r.key_len, r.value.data(), r.val_len);
}

bool deserialize(const char* data, size_t len, Record& r) {
    if (len < 8) return false;  // not enough for header

    std::memcpy(&r.key_len, data, 4);
    std::memcpy(&r.val_len, data + 4, 4);

    if (len < 8 + r.key_len) return false;  // incomplete key

    r.key.assign(data + 8, r.key_len);

    if (len < 8 + r.key_len + r.val_len) {
        r.value = "";  // incomplete value, OK in your test
        return true;
    }

    r.value.assign(data + 8 + r.key_len, r.val_len);
    return true;
}
