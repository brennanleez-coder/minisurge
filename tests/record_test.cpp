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

    SECTION("Handles non-empty key and value") {
        // key_len=3 (0x03000000), val_len=5 (0x05000000)
        const char buffer[] = {3,0,0,0, 5,0,0,0, 'k','e','y','v','a','l','u','e'};
        Record r;
        deserialize(buffer, sizeof(buffer), r);
        REQUIRE(r.key_len == 3);
        REQUIRE(r.val_len == 5);
        REQUIRE(r.key == "key");
        REQUIRE(r.value == "value");
    }

    SECTION("Handles partial data") {
        const char buffer[] = {3,0,0,0, 5,0,0,0, 'k','e','y'};
        Record r;
        deserialize(buffer, sizeof(buffer), r);
        REQUIRE(r.key_len == 3);
        REQUIRE(r.val_len == 5);
        REQUIRE(r.key == "key");
        REQUIRE(r.value == "");
    }

    SECTION("Handles large key and value") {
        std::vector<char> buffer(8 + 256 + 512, 'a');
        // Set key_len=256 (0x00000100 in little-endian: 0x00,0x01,0x00,0x00)
        buffer[0] = 0x00;
        buffer[1] = 0x01;
        buffer[2] = 0x00;
        buffer[3] = 0x00;
        
        // Set val_len=512 (0x00000200 in little-endian: 0x00,0x02,0x00,0x00)
        buffer[4] = 0x00;
        buffer[5] = 0x02;
        buffer[6] = 0x00;
        buffer[7] = 0x00;
        
        Record r;
        deserialize(buffer.data(), buffer.size(), r);
        REQUIRE(r.key_len == 256);
        REQUIRE(r.val_len == 512);
        REQUIRE(r.key.size() == 256);
        REQUIRE(r.value.size() == 512);
    }
}
