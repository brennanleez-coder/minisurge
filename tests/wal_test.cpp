#include <catch2/catch_test_macros.hpp>
#include "wal.hpp"
#include <filesystem>
#include <cstdio>
#include "wal_reader.hpp"
TEST_CASE("WAL append & flush", "[wal]") {
  auto path = std::filesystem::temp_directory_path() / "minisurge_wal_test.log";
  std::remove(path.c_str());   // ensure clean
  WALWriter w(path, /*append=*/false);
  
  Record r1{0,3,5,"key","value"};
  Record r2{0,4,6,"test","foobar"};
  w.append(r1);
  w.append(r2);
  w.flush();

  // Read binary data instead of text
  std::ifstream in(path, std::ios::binary | std::ios::ate);
  REQUIRE(in.is_open());
  size_t size = in.tellg();
  in.seekg(0);
  
  // Verify total size: 2 records × (9 header + key_len + val_len)
  REQUIRE(size == (9 + 3 + 5) + (9 + 4 + 6));

  // Read entire file content
  std::vector<char> buffer(size);
  in.read(buffer.data(), size);
  
  // Verify first record
  REQUIRE(buffer[0] == 0);  // type
  REQUIRE(*reinterpret_cast<uint32_t*>(buffer.data() + 1) == 3);  // key_len
  REQUIRE(*reinterpret_cast<uint32_t*>(buffer.data() + 5) == 5);  // val_len
  REQUIRE(memcmp(buffer.data() + 9, "key", 3) == 0);
  REQUIRE(memcmp(buffer.data() + 12, "value", 5) == 0);

  // Verify second record
  size_t second_record = 9 + 3 + 5;
  REQUIRE(buffer[second_record] == 0);  // type
  REQUIRE(*reinterpret_cast<uint32_t*>(buffer.data() + second_record + 1) == 4);
  REQUIRE(*reinterpret_cast<uint32_t*>(buffer.data() + second_record + 5) == 6);
  REQUIRE(memcmp(buffer.data() + second_record + 9, "test", 4) == 0);
  REQUIRE(memcmp(buffer.data() + second_record + 13, "foobar", 6) == 0);
  
  in.close();
}
TEST_CASE("WAL append & replay", "[wal]") {
  auto path = std::filesystem::temp_directory_path() / "minisurge_wal_test.log";
  std::remove(path.c_str());   // ensure clean

  {
    WALWriter w(path, /*append=*/false);
    Record r1{0,3,5,"key","value"};
    Record r2{0,4,6,"test","foobar"};
    w.append(r1);
    w.append(r2);
    w.flush();
  }

  WALReader reader(path);
  auto recs = reader.replay();

  REQUIRE(recs.size() == 2);
  
  REQUIRE(recs[0].type == 0);
  REQUIRE(recs[0].key_len == 3);
  REQUIRE(recs[0].val_len == 5);
  REQUIRE(recs[0].key == "key");
  REQUIRE(recs[0].value == "value");
  
  REQUIRE(recs[1].type == 0);
  REQUIRE(recs[1].key_len == 4);
  REQUIRE(recs[1].val_len == 6);
  REQUIRE(recs[1].key == "test");
  REQUIRE(recs[1].value == "foobar");
}
