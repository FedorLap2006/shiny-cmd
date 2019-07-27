#pragma once

#include <algorithm>
#include <cctype>
#include <vector>
#include <string>
#include <iostream>
#include <list>

namespace shiny {

    struct Parser {
        struct Node {
            std::string value;
            enum type {
                CMD,
                ARG
            }type = CMD;

            Node operator=(Node other) {
                this->value = other.value;
                this->type = other.type;
                return *this;
            }
        };
        std::vector<Node> parseString(std::string str) {
            enum tstate {
                WAITCMD,
                WAITARG,
                INCMD,
                INARG,
                WAITSTR,
                INSTR,
            }state = WAITCMD,state2 = WAITSTR;
            std::vector<Node> nodes;
            Node cur_node;
            for (auto it = str.begin(); it != str.end(); it++) {
                switch(state) {
                    case WAITCMD:
                        if(std::isspace(*it) == 0) {
                            if((*it == '\"' || *it == '\'' || *it == '`')) state2 = INSTR;

                            //std::cout << "waitcmd, is = 0\n";
                            state = INCMD;
                            cur_node.value += *it;
                            cur_node.type = Node::type::CMD;
                        }
                        break;
                    case WAITARG:
                        if(std::isspace(*it) == 0) {
                            if((*it == '\"' || *it == '\'' || *it == '`')) state2 = INSTR;
                            //std::cout << "waitarg, is = 0\n";
                            state = INARG;
                            cur_node.value += *it;
                            cur_node.type = Node::type::ARG;
                        }
                        break;
                    case INCMD:
                        if(std::isspace(*it) == 0) {
                            if((*it == '\"' || *it == '\'' || *it == '`')) state2 = INSTR;

                            //std::cout << "incmd, is = 0\n";
                            state = INCMD;
                            cur_node.value += *it;
                            cur_node.type = Node::type::CMD;

                            if((*it == '\"' || *it == '\'' || *it == '`') && state2 == INSTR) {
                                state2 = WAITSTR;
                                cur_node.value.erase(0,1);

                                cur_node.value.erase(cur_node.value.size() - 1);
                            }

                        } else if(std::isspace(*it) != 0 && state2 != INSTR) {

                            //std::cout << "incmd, is = 1\n";
                            state = WAITARG;
                            nodes.push_back(cur_node);
                            cur_node = {"",Node::type::ARG};
                        } else if(std::isspace(*it) != 0 && state2 == INSTR) {
                            cur_node.value += *it;
                            cur_node.type = Node::type::CMD;
                        }
                        break;
                    case INARG:
                        if(std::isspace(*it) == 0) {
                            if((*it == '\"' || *it == '\'' || *it == '`')) state2 = INSTR;
                            //std::cout << "inarg, is = 0\n";
                            state = INCMD;
                            cur_node.value += *it;
                            cur_node.type = Node::type::CMD;
                            if((*it == '\"' || *it == '\'' || *it == '`') && state2 == INSTR) {
                                state2 = WAITSTR;
                                cur_node.value.erase(0,1);

                                cur_node.value.erase(cur_node.value.size() - 1);
                            }

                        } else if(std::isspace(*it) != 0 && state2 != INSTR) {

                            //std::cout << "incmd, is = 1\n";
                            state = WAITARG;
                            nodes.push_back(cur_node);
                            cur_node = {"",Node::type::ARG};
                        } else if(std::isspace(*it) != 0 && state2 == INSTR) {
                            cur_node.value += *it;
                            cur_node.type = Node::type::CMD;
                        }
                        break;
                }

                //std::cout << "log: " << cur_node.type << ":" << cur_node.value << ":" << std::endl;
            }
            if (cur_node.value.size() != 0) {
                nodes.push_back(cur_node);
            }
            return nodes;
        }
        using Node_List = std::vector<Node>;
    };

    struct CmdDesc {
        std::string cmd;
        std::vector<std::string> args;
        CmdDesc(typename Parser::Node_List nodes) {
            if(nodes.size() != 0) {
                cmd = nodes[0].value;
                if(nodes.size() > 1) {
                    for(auto it : nodes) {
                        args.push_back(it.value);
                    }
                    args.erase(args.begin());
                }
            }
        }
    };



    struct BaseCmd {
        enum cmd_res {
            OK,
            ERR
        };
        virtual cmd_res execCmd(CmdDesc cd) {
            std::cout << cd.cmd << std::endl;
            for(auto it : cd.args) {
                std::cout << it << std::endl;
            }
            return OK;
        }
    };
    CmdDesc makeCmdDesc(std::string input) {
        shiny::Parser p;
        shiny::CmdDesc desc(p.parseString(input));
        return desc;
    }
}
