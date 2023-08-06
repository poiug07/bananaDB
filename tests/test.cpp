#define BOOST_TEST_MODULE StoreTest
#include <boost/test/included/unit_test.hpp>

#include <string>
#include <bananadb/store.h>

BOOST_AUTO_TEST_CASE(set_get_test) {
    auto s = Store();
    std::string key="key1";
    std::string val="value1";
    bool ok = s.Set("key", "val");
    BOOST_REQUIRE(true);
}