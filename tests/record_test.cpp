#include "catch2/catch_test_macros.hpp"
#include "record.hpp"

TEST_CASE("Record deserialization", "[record]") {
    SECTION("Handles empty key and value") {
        const char buffer[] = {0,0,0,0, 0,0,0,0}; // Little-endian 0s
        Record r;
        deserialize(buffer, sizeof(buffer), r);
        REQUIRE(r.key_len == 0);
        REQUIRE(r.val_len == 0);
        REQUIRE(r.key == "");
        REQUIRE(r.value == "");
    }

    // Update deserialization test buffers to include type field
    SECTION("Handles non-empty key and value") {
        const char buffer[] = {0,3,0,0,0,5,0,0,0,'k','e','y','v','a','l','u','e'};
        Record r;
        deserialize(buffer, sizeof(buffer), r);
        REQUIRE(r.key_len == 3);
        REQUIRE(r.val_len == 5);
        REQUIRE(r.key == "key");
        REQUIRE(r.value == "value");
    }

    SECTION("Handles partial data") {
        const char buffer[] = {0,3,0,0,0,5,0,0,0,'k','e','y'};
        Record r;
        deserialize(buffer, sizeof(buffer), r);
        REQUIRE(r.key_len == 3);
        REQUIRE(r.val_len == 5);
        REQUIRE(r.key == "key");
        REQUIRE(r.value == "");
    }

    SECTION("Handles large key and value") {
        std::vector<char> buffer(9 + 256 + 512, 'a');  // 1 byte type + 8 byte lengths
        // Set type to 0 (leaf)
        buffer[0] = 0x00;
        // Set key_len=256 (0x00000100 in little-endian)
        buffer[1] = 0x00;
        buffer[2] = 0x01;
        buffer[3] = 0x00;
        buffer[4] = 0x00;
        // Set val_len=512 (0x00000200 in little-endian)
        buffer[5] = 0x00;
        buffer[6] = 0x02;
        buffer[7] = 0x00;
        buffer[8] = 0x00;
        
        Record r;
        deserialize(buffer.data(), buffer.size(), r);
        REQUIRE(r.key_len == 256);
        REQUIRE(r.val_len == 512);
        REQUIRE(r.key.size() == 256);
        REQUIRE(r.value.size() == 512);
    }
}
TEST_CASE("Record::serialize buffer API", "[record]") {
    SECTION("Empty key/value") {
        Record r;
        r.type = 0;
        r.key_len = 0; r.val_len = 0;
        r.key.clear(); r.value.clear();

        size_t n = 9;  // 1 byte type + 8 byte lengths
        std::vector<char> buf(n);
        serialize(r, buf.data());
        REQUIRE(std::all_of(buf.begin(), buf.end(), [](char c){ return c == 0; }));
    }

    // Update serialization test expectations
    SECTION("Non-empty key/value") {
        Record r;
        r.type = 0;
        r.key   = "key";
        r.value = "value";
        r.key_len = 3; r.val_len = 5;
    
        size_t n = 9 + 3 + 5;  // Updated size calculation (header + key + value)
        std::vector<char> buf(n);
        serialize(r, buf.data());
    
        // Single expected declaration
        std::vector<char> expected = {
            0,          // type field (0 = leaf)
            3,0,0,0,    // key_len (little-endian)
            5,0,0,0,    // val_len (little-endian)
            'k','e','y',
            'v','a','l','u','e'
        };
        
        REQUIRE(std::memcmp(buf.data(), expected.data(), n) == 0);
    }


}
