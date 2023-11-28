#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>
void showHelp();
std::vector<std::string> split(std::string, std::string);
class Generator;
class Generator_State
{
public:
    virtual std::string getline(Generator *) = 0;
    Generator_State(){};
    ~Generator_State(){};
};
class NORMAL_STATE : public Generator_State
{
    virtual std::string getline(Generator *) override;
};
class ADDCOPY_STATE : public Generator_State
{
    virtual std::string getline(Generator *) override;
};
class ENV_STATE : public Generator_State
{
    virtual std::string getline(Generator *) override;
};
class FROM_STATE : public Generator_State
{
    virtual std::string getline(Generator *) override;
};
class CMD_STATE : public Generator_State
{
    virtual std::string getline(Generator *) override;
};
class ENTRY_STATE : public Generator_State
{
    virtual std::string getline(Generator *) override;
};
class EXPOSE_STATE : public Generator_State
{
    virtual std::string getline(Generator *) override;
};
class RUN_STATE : public Generator_State
{
    virtual std::string getline(Generator *) override;
};
class ERROR_STATE : public Generator_State
{
    virtual std::string getline(Generator *) override;
};
class Generator
{
private:
    bool _eof;
    Generator_State *_state;
    int _index;
    std::vector<std::string> _tokens;
    std::string _output;
    std::string _ignore;

public:
    Generator(std::vector<std::string> tokens)
    {
        this->_eof = false;
        this->_state = new NORMAL_STATE();
        this->_index = 0;
        this->_tokens = tokens;
        this->_output = "";
        this->_ignore = "";
    };
    ~Generator(){};
    std::string getToken()
    {
        if (!this->is_EOF())
        {
            if (this->_index + 1 >= this->_tokens.size())
                this->_eof = true;
            return this->_tokens[(this->_index)++];
        }
        return "";
    };
    std::string peekToken(int offset)
    {
        int index = (this->_index) + offset;
        if ((index > 0) || (index < this->_tokens.size()))
            return this->_tokens[(this->_index) + offset];
        return "";
    };
    std::string getline() { return _state->getline(this); };
    void setState(Generator_State *new_state)
    {
        delete this->_state;
        this->_state = new_state;
    };
    bool is_EOF() { return this->_eof; };
    std::string getOutput() { return this->_output; };
    void setOutput(std::string file) { this->_output = file; };
    std::string getIgnore() { return this->_ignore; };
    void addIgnore(std::string str) { this->_ignore += str + '\n'; };
};
std::string NORMAL_STATE::getline(Generator *generator)
{
    std::string token(generator->getToken());
    if (token == "-a" || token == "--add")
        generator->setState(new ADDCOPY_STATE());
    else if (token == "-c" || token == "--copy")
        generator->setState(new ADDCOPY_STATE());
    else if (token == "-e" || token == "--env")
        generator->setState(new ENV_STATE());
    else if (token == "-f" || token == "--from")
        generator->setState(new FROM_STATE());
    else if (token == "-h" || token == "--help")
    {
        showHelp();
        return "";
    }
    else if (token == "-i" || token == "--ignore")
    {
        generator->addIgnore(generator->getToken());
        return "";
    }
    else if (token == "-m" || token == "--cmd")
        generator->setState(new CMD_STATE());
    else if (token == "-n" || token == "--entrypoint")
        generator->setState(new ENTRY_STATE());
    else if (token == "-o" || token == "--output")
    {
        generator->setOutput(generator->getToken());
        return "";
    }
    else if (token == "-p" || token == "--expose")
        generator->setState(new EXPOSE_STATE());
    else if (token == "-r" || token == "--run")
        generator->setState(new RUN_STATE());
    else
        generator->setState(new ERROR_STATE());
    return generator->getline();
}
std::string ADDCOPY_STATE::getline(Generator *generator)
{
    std::vector<std::string> argv = split(generator->getToken(), ":");
    if (argv.size() != 2)
    {
        generator->setState(new ERROR_STATE());
        return "";
    }
    generator->setState(new NORMAL_STATE());
    if (generator->peekToken(-2).find("-c") != std::string::npos)
        return "COPY [\"" + argv[0] + "\", \"" + argv[1] + "\"]\n";
    else
        return "ADD [\"" + argv[0] + "\", \"" + argv[1] + "\"]\n";
}
std::string ENV_STATE::getline(Generator *generator)
{
    generator->setState(new NORMAL_STATE());
    return "ENV " + generator->getToken() + "\n";
}
std::string FROM_STATE::getline(Generator *generator) // TODO:force command
{
    generator->setState(new NORMAL_STATE());
    return "FROM " + generator->getToken() + "\n";
}
std::string CMD_STATE::getline(Generator *generator)
{
    generator->setState(new NORMAL_STATE());
    return "CMD " + generator->getToken() + "\n";
}
std::string ENTRY_STATE::getline(Generator *generator)
{
    generator->setState(new NORMAL_STATE());
    return "ENTRYPOINT " + generator->getToken() + "\n";
}
std::string EXPOSE_STATE::getline(Generator *generator)
{
    generator->setState(new NORMAL_STATE());
    return "EXPOSE " + generator->getToken() + "\n";
}
std::string RUN_STATE::getline(Generator *generator)
{
    generator->setState(new NORMAL_STATE());
    return "RUN " + generator->getToken() + "\n";
}
std::string ERROR_STATE::getline(Generator *generator)
{
    std::cout << "\e[33mSomething wrong!\e[0m\n";
    showHelp();
    return "";
}
void showHelp()
{
    std::ifstream fin("help_info");
    if (fin.is_open())
        std::cout << fin.rdbuf();
    fin.close();
    exit(1);
}
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