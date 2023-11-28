#include "p2c_mod_website.h"

class p2c_mod_website : public p2c_mod
{
public:
    p2c_mod_website()
    {
        p2c_alerter::alerting(DEBUG, "mod website init");
    };
    ~p2c_mod_website(){};
    virtual int entry(std::string, std::string) override
    {
        if (stat("/etc/init.d/apache2", NULL))
            parser_Apache();
        else if (stat("/etc/init.d/nginx", NULL))
            parser_Nginx();
        return 0;
    };
    virtual std::vector<std::string> getCommand() override
    {
        std::vector<std::string> cmd{"--website"};
        return cmd;
    };
};

extern "C" p2c_mod *p2c_create_mod()
{
    return new p2c_mod_website;
}