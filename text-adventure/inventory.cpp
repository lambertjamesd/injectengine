#include "inventory.h"

Inventory::Entry::Entry(const Item& item) : item(item), count(0) {

}

bool Inventory::hasItem(const std::string& name) const {
    return itemCount(name) > 0;
}

int Inventory::itemCount(const std::string& name) const {
    auto entry = items.find(name);

    if (entry == items.end()) {
        return 0;
    } else {
        return entry->second.count;
    }
}

bool Inventory::giveItem(const std::string& name, int count) {
    return false;
}

bool Inventory::takeItem(const std::string& name, int count) {
    return false;
}