#ifndef _P2C_MOD_H_
#define _P2C_MOD_H_

#include <iostream>
#include <vector>

class p2c_mod
{
private:
public:
    p2c_mod(){};
    ~p2c_mod(){};
    virtual int entry(std::string, std::string)
    {
        p2c_alerter::alerting(WARN, "mod entry undefine");
        return 1;
    };
    virtual std::vector<std::string> getCommand()
    {
        p2c_alerter::alerting(WARN, "mod getCommand() undefine");
        std::vector<std::string> null;
        return null;
    };
};

typedef p2c_mod *p2c_create_mod_t();

#endif