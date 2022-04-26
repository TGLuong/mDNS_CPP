#include <map>
#include <iostream>

int main() {
    std::map<std::string, std::string> haha = {{"name", "tran gia luong"}, {"age", "21"}};

    for (const auto& item : haha) {
        std::cout << item.first << "=" << item.second << std::endl;
    }
    return 0;
}
