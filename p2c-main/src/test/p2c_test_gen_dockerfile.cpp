#define _ALERT_LEVEL_ 0

#include <p2c_lib.h>

#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    p2c_liblist libList;
    argTable = new p2c_argtable;
    bool gui = false;

    argTable->addArg("FROM", "debian");
    argTable->addArg("EXPOSE", "443/tcp");
    argTable->addArg("COPY", "/etc/apt/sources.list");
    argTable->addArg("COPY", "/etc/apt/sources.list");
    argTable->addArg("ENV", "key1");
    argTable->addArg("ENV", "value1");
    argTable->addArg("RUN", "apt-get update;apt-get install -y curl");
    argTable->addArg("CMD", "p2c -h");
    argTable->addArg("gen_cmd", "--dockerfile");

    queue<string> *gen_cmd = argTable->getArg("gen_cmd");
    while (!gen_cmd->empty())
    {
        libList.callGenFunc(gen_cmd->front());
        gen_cmd->pop();
    }
    return 0;
}
