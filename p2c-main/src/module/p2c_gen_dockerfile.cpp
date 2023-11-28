#include <p2c_lib.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

class p2c_gen_dockerfile : public p2c_mod
{
public:
    p2c_gen_dockerfile(){};
    ~p2c_gen_dockerfile(){};
    virtual int entry(std::string, std::string) override;
    virtual std::vector<std::string> getCommand() override
    {
        std::vector<std::string> cmd{"-df", "--dockerfile"};
        return cmd;
    };
};

int p2c_gen_dockerfile::entry(std::string cmd, std::string token)
{
    std::ofstream fout("Dockerfile");
    if (fout.fail())
        p2c_alerter::alerting(ERROR, "Cannot open Dockerfile for output");
    std::queue<std::string> FROM(*argTable->getArg("FROM"));
    if (FROM.size() == 1)
    {
        fout << "FROM " << FROM.front() << '\n';
        FROM.pop();
    }
    else
        p2c_alerter::alerting(ERROR, "FROM must be specified"); // TODO: add default FROM
    std::queue<std::string> RUN(*argTable->getArg("RUN"));
    while (!RUN.empty())
    {
        fout << "RUN " << RUN.front() << '\n';
        RUN.pop();
    }
    std::queue<std::string> ADD(*argTable->getArg("ADD"));
    while (!ADD.empty())
    {
        std::string src = ADD.front();
        ADD.pop();
        if (ADD.empty())
        {
            p2c_alerter::alerting(ERROR, "ADD number error");
            return 1;
        }
        std::string dest = ADD.front();
        ADD.pop();
        fout << "ADD [\"" << src << "\", \"" << dest << "\"]\n";
    }
    std::queue<std::string> COPY(*argTable->getArg("COPY"));
    while (!COPY.empty())
    {
        std::string src = COPY.front();
        COPY.pop();
        if (COPY.empty())
            p2c_alerter::alerting(ERROR, "COPY number error");
        std::string dest = COPY.front();
        COPY.pop();
        fout << "COPY [\"" << src << "\", \"" << dest << "\"]\n";
    }
    std::queue<std::string> ENV(*argTable->getArg("ENV"));
    while (!ENV.empty())
    {
        std::string key = ENV.front();
        ENV.pop();
        if (ENV.empty())
            p2c_alerter::alerting(ERROR, "ENV number error");
        std::string value = ENV.front();
        ENV.pop();
        fout << "ENV " << key << "=" << value << '\n';
    }
    std::queue<std::string> EXPOSE(*argTable->getArg("EXPOSE"));
    while (!EXPOSE.empty())
    {
        fout << "EXPOSE " << EXPOSE.front() << '\n';
        EXPOSE.pop();
    }
    std::queue<std::string> CMD(*argTable->getArg("CMD"));
    while (!CMD.empty())
    {
        fout << "CMD " << CMD.front() << '\n';
        CMD.pop();
    }
    std::queue<std::string> ENTRYPOINT(*argTable->getArg("ENTRYPOINT"));
    while (!ENTRYPOINT.empty())
    {
        fout << "ENTRYPOINT " << ENTRYPOINT.front() << '\n';
        ENTRYPOINT.pop();
    }
    fout.close();
    return 0;
}

extern "C" p2c_mod *p2c_create_mod()
{
    return new p2c_gen_dockerfile;
}