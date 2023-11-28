#define _ALERT_LEVEL_ 0

#include <p2c_lib.h>

#include <iostream>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[])
{
    p2c_liblist libList;
    argTable = new p2c_argtable;
    bool gui = false;
    argTable->initArgs(argc, argv);
    cout << '\n'
         << argTable->getJSON() << "\n\n";
    if (!argTable->countArg("init_args"))
        p2c_alerter::alerting(ERROR, "Usage: p2c [options]\n");
    queue<string> *init_args = argTable->getArg("init_args");
    while (!init_args->empty())
    {
        p2c_alerter::alerting(DEBUG, "'test_mod_normal':22: classify init_args ...");
        if (init_args->front() == "--gui")
            gui = true;
        else if (libList.mod_count(init_args->front()))
            argTable->addArg("mod_cmd", init_args->front());
        else if (libList.gen_count(init_args->front()))
            argTable->addArg("gen_cmd", init_args->front());
        else
        {
            p2c_alerter::alerting(INFO, "find other command", init_args->front());
            // p2c_alerter::alerting(ERROR, "Usage: p2c [options]\n");
        }
        init_args->pop();
    }
    cout << '\n'
         << argTable->getJSON() << "\n\n";
    queue<string> *mod_cmd = argTable->getArg("mod_cmd");
    if (mod_cmd)
        while (!mod_cmd->empty())
        {
            libList.callModFunc(mod_cmd->front());
            mod_cmd->pop();
        }
    cout << '\n'
         << argTable->getJSON() << "\n\n";
    p2c_alerter::alerting(DEBUG, "Test done");
    return 0;
}
