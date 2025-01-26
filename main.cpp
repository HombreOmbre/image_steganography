#include <iostream>
#include <fstream>

#include "flag_functions/flag_funcs.hpp"
#include "validation_functions/validation_funcs.hpp"

enum Flags {
    INFO,
    ENCRYPT,
    DECRYPT,
    CHECK,
    HELP,
    WRONG,
};

auto checkFlagFromUser(std::string const& str) -> Flags;
auto trim(std::string& str) -> void;

auto main(int argc, char *argv[]) -> int {
    using namespace std;
    auto flag = HELP;

    if (argc > 1) {
        flag = checkFlagFromUser(argv[1]);
    }

    switch (flag) {
        case INFO:
            if (validateArguments(argc, 3, argv[2])) {
                return 0;
            }
            showInfoAboutFile(argv[2]);
        break;
        case ENCRYPT: {
            if (validateArguments(argc, 4, argv[2])) {
                return 0;
            }

            auto msg = std::string(argv[3]);
            trim(msg);
            encryptFile(argv[2], msg);
            break;
        }
        case DECRYPT:
            if (validateArguments(argc, 3, argv[2])) {
                return 0;
            }
            decryptFile(argv[2]);
        break;
        case CHECK: {
            if (validateArguments(argc, 4, argv[2])) {
                return 0;
                }

            auto msg = std::string(argv[3]);
            trim(msg);
            checkIfMessageCanBeInsertedIntoFile(argv[2], msg);
            break;
        }
        case HELP:
            if (argc > 2) {
                cout << "Too many arguments for this flag/alias\n";
                cout << "For getting more information about application options, try type in the console:"
                          <<" path/to/the/app -h or --help\n";
                return 0;
            }
            showHelpInfoAboutApp();
        break;
        case WRONG: {
            if (argc > 1) {
                cout << "Wrong call of the function\n";
                cout << "For getting more information about application options, try type in the console:"
                          <<" path/to/the/app -h or --help\n";
                return 0;
            }
            showHelpInfoAboutApp();
        }
    }
}

auto checkFlagFromUser(std::string const& str) -> Flags {
    if (str == "-i" || str == "--info") {
        return INFO;
    }

    if (str == "-e" || str == "--encrypt") {
        return ENCRYPT;
    }

    if (str == "-d" || str == "--decrypt") {
        return DECRYPT;
    }

    if (str == "-c" || str == "--check") {
        return CHECK;
    }

    if (str == "-h" || str == "--help") {
        return HELP;
    }

    return WRONG;
}

auto trim(std::string& str) -> void {
    auto start = str.begin();
    while (*start == 32 && start != str.end()) {
        ++start;
    }
    str.erase(str.begin(), start);

    start = str.end() - 1;
    while (*start == 32 && start != str.end()) {
        --start;
    }
    str.erase(start + 1, str.end());
}