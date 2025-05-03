#include "record.hpp"
#include <cstring>

void serialize(const Record& r, char* buffer) {
    // Serialize header
    std::memcpy(buffer, &r.type, sizeof(r.type));
    std::memcpy(buffer + 1, &r.key_len, sizeof(r.key_len));
    std::memcpy(buffer + 5, &r.val_len, sizeof(r.val_len));
    
    // Serialize key and value
    char* ptr = buffer + 9;
    if(r.key_len > 0) {
        std::memcpy(ptr, r.key.data(), r.key_len);
        ptr += r.key_len;
    }
    if(r.val_len > 0) {
        std::memcpy(ptr, r.value.data(), r.val_len);
        ptr += r.val_len;
    }
    
    // Serialize children recursively
    for(const auto& child : r.children) {
        serialize(child, ptr);
        ptr += child.size();  // Need to implement size() method
    }
}

// Fix deserialization header offsets
bool deserialize(const char* data, size_t len, Record& r) {
    if (len < 9) return false;

    std::memcpy(&r.type, data, 1);
    std::memcpy(&r.key_len, data + 1, 4);
    std::memcpy(&r.val_len, data + 5, 4);

    if (len < 9 + r.key_len) return false;  // Require at least key data
    
    r.key.assign(data + 9, r.key_len);
    
    // Handle potentially incomplete value
    if (len >= 9 + r.key_len + r.val_len) {
        r.value.assign(data + 9 + r.key_len, r.val_len);
    } else {
        r.value = "";
    }
    
    return true;
}

size_t Record::size() const {
    size_t total = 9 + key_len + val_len; // Header + key + value
    for(const auto& child : children) {
        total += child.size(); // Recursive calculation
    }
    return total;
}
