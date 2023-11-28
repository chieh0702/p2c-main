#include <p2c_lib.h>

#include <iostream>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[])
{
    p2c_liblist libList;
    argTable = new p2c_argtable;
    argTable->initArgs(argc, argv);
    bool gui = false;
    if (!argTable->countArg("init_args"))
        p2c_alerter::alerting(ERROR, "Usage: p2c [options]\n");
    queue<string> *init_args = argTable->getArg("init_args");
    while (!init_args->empty())
    {
        if (init_args->front() == "--gui")
            gui = true;
        else if (libList.mod_count(init_args->front()))
            argTable->addArg("mod_cmd", init_args->front());
        else if (libList.gen_count(init_args->front()))
            argTable->addArg("gen_cmd", init_args->front());
        else
            p2c_alerter::alerting(ERROR, "Usage: p2c [options]\n");
        init_args->pop();
    }
    queue<string> *mod_cmd = argTable->getArg("mod_cmd");
    while (!mod_cmd->empty())
    {
        while (!mod_cmd->empty())
        {
            libList.callModFunc(mod_cmd->front());
            mod_cmd->pop();
        }
        if (gui)
            runGUI();
        mod_cmd = argTable->getArg("mod_cmd");
    }
    queue<string> *gen_cmd = argTable->getArg("gen_cmd");
    while (!gen_cmd->empty())
    {
        libList.callGenFunc(gen_cmd->front());
        gen_cmd->pop();
    }
    return 0;
}
