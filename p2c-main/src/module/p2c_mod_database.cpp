#include <p2c_lib.h>

#include <iostream>

class p2c_mod_database : public p2c_mod
{
private:
public:
    p2c_mod_database()
    {
        p2c_alerter::alerting(DEBUG, "mod dockerimg init");
    };
    ~p2c_mod_database(){};
    virtual int entry(std::string, std::string) override
    {
        /* TODO:
        if(mysql)
            mysqldump();
        if(mongodb)
            mongodump();
        if(postgresql)
            pg_dump();
        if(sqlite)
            sqlite3_dump();
        if(mariadb)
            mariadb_dump();
        if(redis)
            redis_dump();
        */
    };
    virtual std::vector<std::string> getCommand() override
    {
        std::vector<std::string> cmd{"--database"};
        return cmd;
    };
};

extern "C" p2c_mod *p2c_create_mod()
{
    return new p2c_mod_database;
}