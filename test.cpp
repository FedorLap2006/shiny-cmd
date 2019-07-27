#include "cmd.hpp"

int main() {
    std::string strs[] = {
        "dmd test\n",
        "cmd test 2"
    };

    shiny::Parser l;
    shiny::BaseCmd cmd;
    shiny::CmdDesc desc(l.parse(strs[0]));
    cmd.execCmd(desc);
}