#include <p2c_lib.h>

#include <iostream>

std::vector<std::string> split(std::string str, std::string pattern)
{
    std::vector<std::string> result;
    while (str.find(pattern) != std::string::npos)
    {
        std::size_t pos = str.find(pattern);
        result.push_back(str.substr(0, pos));
        str = str.substr(pos + pattern.size());
    }
    result.push_back(str);
    return result;
}

class p2c_mod_normal : public p2c_mod
{
public:
    p2c_mod_normal(){};
    ~p2c_mod_normal(){};
    virtual int entry(std::string, std::string) override;
    virtual std::vector<std::string> getCommand() override;
};

extern "C" p2c_mod *p2c_create_mod()
{
    return new p2c_mod_normal;
}

int p2c_mod_normal::entry(std::string cmd, std::string token)
{
    if (cmd == "-a" || cmd == "--add")
    {
        std::vector<std::string> argv = split(token, ":");
        if (argv.size() == 2)
            argTable->addArg("ADD", argv);
        else
            p2c_alerter::alerting(ERROR, "Usage: p2c [-a|--add] SOURCE:DEST\n");
    }
    else if (cmd == "-c" || cmd == "--copy")
    {
        std::vector<std::string> argv = split(token, ":");
        if (argv.size() == 2)
            argTable->addArg("COPY", argv);
        else
            p2c_alerter::alerting(ERROR, "Usage: p2c [-c|--copy] SOURCE:DEST\n");
    }
    else if (cmd == "-e" || cmd == "--env")
    {
        std::vector<std::string> argv = split(token, "=");
        if (argv.size() == 2)
            argTable->addArg("ENV", argv);
        else
            p2c_alerter::alerting(ERROR, "Usage: p2c [-e|--env] \"KEY=VALUE\"\n");
    }
    else if (cmd == "-f" || cmd == "--from")
    {
        if (token != "")
            argTable->addArg("FROM", token);
        else
            p2c_alerter::alerting(ERROR, "Usage: p2c [-f|--from] IMAGE_NAME[:TAG]\n");
    }
    else if (cmd == "-h" || cmd == "--help")
    {
        std::cout << "help page...\n"; // TODO:show help
    }
    else if (cmd == "-i" || cmd == "--ignore")
    {
        if (token != "")
            argTable->addArg("IGNORE", token);
        else
            p2c_alerter::alerting(ERROR, "Usage: p2c [-i|--ignore] \"CONTEXT\"\n");
    }
    else if (cmd == "-m" || cmd == "--cmd")
    {
        if (token != "")
            argTable->addArg("CMD", token);
        else
            p2c_alerter::alerting(ERROR, "Usage: p2c [-m|--cmd] \"COMMAND\"\n");
    }
    else if (cmd == "-n" || cmd == "--entrypoint")
    {
        if (token != "")
            argTable->addArg("ENTRYPOINT", token);
        else
            p2c_alerter::alerting(ERROR, "Usage: p2c [-n|--entrypoint] \"COMMAND\"\n");
    }
    else if (cmd == "-o" || cmd == "--output")
    {
        if (token != "")
            argTable->addArg("OUTPUT", token);
        else
            p2c_alerter::alerting(ERROR, "Usage: p2c [-o|--output] \"DIR\"\n");
    }
    else if (cmd == "-p" || cmd == "--expose")
    {
        if (token != "")
            argTable->addArg("EXPOSE", token);
        else
            p2c_alerter::alerting(ERROR, "Usage: p2c [-p|--expose] PORT[/TYPE]\n");
    }
    else if (cmd == "-r" || cmd == "--run")
    {
        if (token != "")
            argTable->addArg("RUN", token);
        else
            p2c_alerter::alerting(ERROR, "Usage: p2c [-r|--run] \"COMMAND\"\n");
    }
    else
    {
        p2c_alerter::alerting(ERROR, "Usage: p2c [options]\n");
        return 1;
    }
    return 0;
}

std::vector<std::string> p2c_mod_normal::getCommand()
{
    std::vector<std::string> cmd{
        "-a", "--add",
        "-c", "--copy",
        "-e", "--env",
        "-f", "--from",
        "-h", "--help",
        "-i", "--ignore",
        "-m", "--cmd",
        "-n", "--entrypoint",
        "-o", "--output",
        "-p", "--expose",
        "-r", "--run"};
    return cmd;
}
