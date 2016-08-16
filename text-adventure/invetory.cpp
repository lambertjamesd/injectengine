#include "invetory.h"

Invetory::Entry::Entry(const Item& item) : item(item), count(0) {

}

bool Invetory::hasItem(const std::string& name) const {
    return itemCount(name) > 0;
}

int Invetory::itemCount(const std::string& name) const {
    auto entry = items.find(name);

    if (entry == items.end()) {
        return 0;
    } else {
        return entry->second.count;
    }
}

bool Invetory::giveItem(const std::string& name, int count) {
    return false;
}

bool Invetory::takeItem(const std::string& name, int count) {
    return false;
}