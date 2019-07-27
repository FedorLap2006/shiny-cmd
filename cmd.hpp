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
        };
        std::vector<Node> parse(std::string str) {
            enum tstate {
                WAITCMD,
                WAITARG,
                INCMD,
                INARG,
            }state = WAITCMD;
            std::vector<Node> nodes;
            Node cur_node;
            for (auto it = str.begin(); it != str.end(); it++) {
                switch(state) {
                    case WAITCMD:
                        if(std::isspace(*it) == 0) {
                            //std::cout << "waitcmd, is = 0\n";
                            state = INCMD;
                            cur_node.value += *it;
                            cur_node.type = Node::type::CMD;
                        }
                        break;
                    case WAITARG:
                        if(std::isspace(*it) == 0) {
                            //std::cout << "waitarg, is = 0\n";
                            state = INARG;
                            cur_node.value += *it;
                            cur_node.type = Node::type::ARG;
                        }
                        break;
                    case INCMD:
                        if(std::isspace(*it) == 0) {
                            //std::cout << "incmd, is = 0\n";
                            state = INCMD;
                            cur_node.value += *it;
                            cur_node.type = Node::type::CMD;
                        } else {
                            //std::cout << "incmd, is = 1\n";
                            state = WAITARG;
                            nodes.push_back(cur_node);
                            cur_node = {"",Node::type::ARG};
                        }
                        break;
                    case INARG:
                        if(std::isspace(*it) == 0) {
                            //std::cout << "inarg, is = 0\n";
                            state = INCMD;
                            cur_node.value += *it;
                            cur_node.type = Node::type::CMD;
                        } else {
                            //std::cout << "inarg, is = 1\n";
                            state = WAITARG;
                            nodes.push_back(cur_node);
                            cur_node = {"",Node::type::ARG};
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
}