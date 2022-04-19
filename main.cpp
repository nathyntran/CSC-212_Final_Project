#include "boyer.h"
#include "rabin.h"

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cout << "\nPlease enter a file, a file to compare, and the search size in command line\nIf you want to check entire string, please input 0 as search size!\n\n";
        return 0;
    }

    if (std::stoi(argv[3]) < 6 && std::stoi(argv[3]) != 0)
    {   
        std::cout << "Search size is too small, as it may cause faulty positives for checks!!!\n\n";
        return 0;
    }

    Boyer boyer_test = Boyer(argv[1], argv[2], std::stoi(argv[3]));
    Rabin rabin_test = Rabin(argv[1], argv[2], std::stoi(argv[3]));
    
    boyer_test.test();
    rabin_test.test();


}