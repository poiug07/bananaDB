#define BOOST_TEST_MODULE StoreTest
#include <boost/test/included/unit_test.hpp>

#include <string>
#include <bananadb/store.h>

BOOST_AUTO_TEST_CASE(set_get_test) {
    auto s = Store();
    std::string key="key1";
    std::string val="value1";
    bool ok = s.Set(key, val);
    BOOST_REQUIRE(ok);
    std::pair<std::string, bool> get = s.Get(key);
    BOOST_REQUIRE(get.second);
    BOOST_TEST(get.first==val);
}

BOOST_AUTO_TEST_CASE(set_existing_key_test) {
    auto s = Store();
    std::string key="key1";
    std::string val1 = "value1";
    std::string val2 = "value2";
    bool ok = s.Set(key, val1);
    BOOST_REQUIRE(ok);
    std::pair<std::string, bool> get1 = s.Get(key);
    BOOST_REQUIRE(get1.second);
    BOOST_TEST(get1.first == val1);

    ok = s.Set(key, val2);
    BOOST_REQUIRE(ok);
    std::pair<std::string, bool> get2 = s.Get(key);
    BOOST_REQUIRE(get2.second);
    BOOST_TEST(get2.first == val2);
}

BOOST_AUTO_TEST_CASE(get_non_existing_key) {
    auto s = Store();
    std::string key="key1";
    std::string val="value1";
    
    std::pair<std::string, bool> get = s.Get(key);
    BOOST_REQUIRE(!get.second);
}

BOOST_AUTO_TEST_CASE(delete_by_key) {
    auto s = Store();
    std::string key1="key1";
    std::string val1="value1";
    std::string key2="key2";
    std::string val2="value2";
    s.Set(key1, val1);
    s.Set(key2, val2);

    // First key must be deleted
    bool deleted = s.DeleteByKey(key1);
    BOOST_REQUIRE(deleted);
    std::pair<std::string, bool> get = s.Get(key1);
    BOOST_REQUIRE(!get.second);

    // Second key must still be present in store
    get = s.Get(key2);
    BOOST_REQUIRE(get.second);
}

BOOST_AUTO_TEST_CASE(delete_by_non_existing_key) {
    auto s = Store();
    std::string key1="key1";
    std::string val1="value1";
    std::string key2="key2";
    std::string val2="value2";

    bool deleted = s.DeleteByKey(key1);
    BOOST_REQUIRE(!deleted);
    std::pair<std::string, bool> get = s.Get(key1);
    BOOST_REQUIRE(!get.second);
}

BOOST_AUTO_TEST_CASE(get_all_keys) {
    auto s = Store();
    std::string[] keys = { "key1" };
    std::string value = "value" ;
}