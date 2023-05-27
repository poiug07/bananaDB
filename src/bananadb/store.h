#ifndef _STORE_H_
#define _STORE_H_

#include <map>
#include <string>
#include <vector>

class Store {
private:
    std::map<std::string, std::string> store;
public:
    // Set method returns true if insertion is successful, otherwise it return false.
    bool Set(const std::string& key, const std::string& val);
    // This method return std::pair<value, true>. If key does not exist in map it returns std::pair<"", false>.
    std::pair<std::string, bool> Get(const std::string& key);
    bool DeleteByKey(const std::string& key);
    std::vector<std::string> GetAllKeys();
    std::string GetSet(const std::string& key, const std::string& default_val);
};

#endif