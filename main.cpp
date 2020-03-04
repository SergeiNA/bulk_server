#include <algorithm>
#include "server.h"

int main(int argc, char** argv) {
    if (argc <= 2 || argc > 3) {
        std::cerr << "Wrong number of arguments. Aborting" << std::endl;
        return 1;
    }

    std::string port(argv[1]);
    std::string bulkSize(argv[2]);

    if (!(std::all_of(port.begin(), port.end(), ::isdigit)
        && std::all_of(bulkSize.begin(), bulkSize.end(), ::isdigit))) {

        std::cerr << "Port or bulk size are incorrect" << std::endl;
        return 1;
    }
    try
    {
        server(static_cast<unsigned short>(std::stoul(port)),std::stoul(bulkSize));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}