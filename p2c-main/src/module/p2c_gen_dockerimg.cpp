#include <p2c_lib.h>

#include <iostream>

class p2c_gen_dockerimg : public p2c_mod
{
private:
public:
    p2c_gen_dockerimg()
    {
        p2c_alerter::alerting(DEBUG, "mod dockerimg init");
    };
    ~p2c_gen_dockerimg(){};
    virtual int entry(std::string, std::string) override
    {
        system("docker build ./ -t p2c:latest"); // TODO: custom docker image name
                                                 //       check dockerfile exist
    };
    virtual std::vector<std::string> getCommand() override
    {
        std::vector<std::string> cmd{"--dockerimg"};
        return cmd;
    };
};

extern "C" p2c_mod *p2c_create_mod()
{
    return new p2c_gen_dockerimg;
}