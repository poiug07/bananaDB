#include "./store.h"

#include <map>
#include <string>
#include <vector>

bool Store::Set(const std::string& key, const std::string& val) {
    std::lock_guard<std::shared_mutex> g(this->mutex_);
    const auto it= this->store.insert_or_assign(key, val);
    return it.first!=this->store.end();
}

std::pair<std::string, bool> Store::Get(const std::string& key) {
        std::shared_lock<std::shared_mutex> g(this->mutex_);
        const auto it = this->store.find(key);
        if(it == this->store.end()) {
            return std::make_pair("", false);
        }
        return std::make_pair(it->second, true);
}

bool Store::DeleteByKey(const std::string& key) {
    std::lock_guard<std::shared_mutex> g(this->mutex_);
    return this->store.erase(key);
}

std::vector<std::string> Store::GetAllKeys() {
    std::shared_lock<std::shared_mutex> g(this->mutex_);
    std::vector<std::string> keys;
    for(auto & it : this->store) {
        keys.push_back(it.first);
    }
    return keys;
}

std::string Store::GetSet(const std::string &key, const std::string &default_val) {
    std::lock_guard<std::shared_mutex> g(this->mutex_);
    auto it = this->store.find(key);
    if(it==this->store.end()) {
        this->Set(key, default_val);
    }
    // Using [] operator since value exists no matter what
    return this->store[key];
}