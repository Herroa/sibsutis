#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
// 0xAF
//024
int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Usage: lexer FILE\n";
        std::exit(1);
    }
    std::ifstream ifs;
    ifs.open(argv[1], std::ifstream::in);

    std::regex regexp("^int");
    std::string str;
    while(std::getline(ifs, str)) {
        auto lexeme_begin = std::sregex_iterator(str.begin(), str.end(), regexp);
        auto lexeme_end = std::sregex_iterator();
        int nlexemes = std::distance(lexeme_begin, lexeme_end);
        if (nlexemes > 0) {
            std::cout << "Found " << std::distance(lexeme_begin, lexeme_end) << " lexemes\n";
            for (std::sregex_iterator i = lexeme_begin; i != lexeme_end; ++i) {
                std::smatch match = *i;
                std::cout << "-- lexeme: " << match.str() << "\n";
            }
        }
    }
    ifs.close();

    return 0;
}
