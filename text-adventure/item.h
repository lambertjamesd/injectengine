#include <string>

class Item {
public:
    Item(const std::string& name, bool multiple);
private:
    std::string name;
    int max;
    // std::unordered_map<std::string, std::unique_ptr<Action>> actions;
};