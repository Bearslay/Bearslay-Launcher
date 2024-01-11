#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> list;
    
    std::string in;
    std::cin >> in;

    for (unsigned char i = 0; i < in.length(); i++) {
        list.emplace_back(in.substr(i, 1));
    }

    for (unsigned char i = 0; i < list.size(); i++) {
        std::cout << list[i];
    }
    std::cout << "\n";

    return 0;
}
