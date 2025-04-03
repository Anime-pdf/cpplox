#include <utils.h>

#include <sstream>

std::string trim(const char *pStr) {
    std::string result(pStr);
    while (result.starts_with(' ')) {
        result.erase(result.begin());
    }

    while (result.ends_with(' ')) {
        result.erase(result.end() - 1);
    }

    return result;
}

std::vector<std::string> tokenize(const char *pStr) {
    std::vector<std::string> result;

    auto Iss = std::istringstream{pStr};
    auto Temp = std::string{};

    while (Iss >> Temp) {
        result.emplace_back(Temp);
    }

    return result;
}
