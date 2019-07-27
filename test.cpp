#include "cmd.hpp"



struct MoveCmd : public shiny::BaseCmd {
    cmd_res execCmd(shiny::CmdDesc cd) override {
        std::cout << "cmd(" << cd.cmd << ") player " << cd.args[0] << " controller(" << cd.args[1] << ") move to " << cd.args[2];
        return OK;
    }
};

int main() {
    std::string strs[] = {
        "`move test` yasha cam '012.43 232.2214 3422.432'\0" // "" '' ``
        "cmd test 2"
    };

    shiny::CmdDesc desc0 = shiny::makeCmdDesc(strs[0]);
    shiny::CmdDesc desc1 = shiny::makeCmdDesc(strs[1]);

    MoveCmd cmd;
    cmd.execCmd(desc0);
    std::cout << std::endl;
}