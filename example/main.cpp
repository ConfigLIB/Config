#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "include/JSONParser.h"

using namespace std::literals::string_literals;
using namespace Config::JSON;

int main() {
    setlocale(LC_ALL, "");
    std::wcout.imbue(std::locale(""));
    auto source = LR"(
{
    "name": "HXZ",
    "university": "shmtu",
    "age": 114.514,
    "gender": "男",
    "isProgrammer": true,
    "skills": ["C#", "Python", "HTML5/CSS3"]
}
    )"s;
    std::wcout << source << L"\n";
    auto res = Parser(Scanner(std::move(source))).Parse();
    // auto obj = res->AsObject();
    // obj->insert({ "hometown", JSONElement("SomeWhere") });
    std::wcout << res->Dumps() << L"\n";
}
