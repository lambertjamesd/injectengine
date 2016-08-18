#include <string>
#include <unordered_map>

#include "item.h"

class Inventory {
public:
    bool hasItem(const std::string& name) const;
    int itemCount(const std::string& name) const;
    bool giveItem(const std::string& name, int count);
    bool takeItem(const std::string& name, int count);

    void defineItem(const Item& item) const;
private:
    struct Entry {
        Entry(const Item& item);
        Item item;
        int count;
    };

    std::unordered_map<std::string, Entry> items;
};