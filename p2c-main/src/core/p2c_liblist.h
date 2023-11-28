#ifndef _P2C_LIBLIST_H_
#define _P2C_LIBLIST_H_

#include <iostream>
#include <vector>
#include <map>
#include <dlfcn.h>
#include <dirent.h>
#include <cstring>
#include <sstream>

#include <p2c_alerter.h>
#include <p2c_mod.h>

class p2c_liblist
{
private:
    std::map<std::string, p2c_mod *> _mod_map;
    std::map<std::string, p2c_mod *> _gen_map;
    std::vector<void *> _opened_lib;
    std::vector<p2c_mod *> _created_mod;

public:
    p2c_liblist() { this->loadLibrary(); };
    ~p2c_liblist();
    void loadLibrary();
    int callModFunc(std::string);
    int callGenFunc(std::string);
    bool mod_count(std::string arg)
    {
        std::size_t pos = arg.find(" ");
        std::string key = arg;
        if (pos != std::string::npos)
            key = arg.substr(0, pos);
        return _mod_map.count(key) == 1 ? true : false;
    };
    bool gen_count(std::string arg)
    {
        std::size_t pos = arg.find(" ");
        std::string key = arg;
        if (pos != std::string::npos)
            key = arg.substr(0, pos);
        return _gen_map.count(key) == 1 ? true : false;
    };
};

p2c_liblist::~p2c_liblist()
{
    // free all library
    for (p2c_mod *_mod : _created_mod)
        if (_mod)
            delete _mod;
    for (void *_lib : _opened_lib)
        if (_lib)
            dlclose(_lib);
}

void p2c_liblist::loadLibrary()
{
    const char LIB_PATH[] = "../../lib/"; // TODO: change to /lib/p2c/module/
    DIR *dirp = opendir(LIB_PATH);
    if (!dirp)
    {
        p2c_alerter::alerting(ERROR, "Open module directory failed: ", strerror(errno));
        return;
    }
    while (1)
    {
        dirent *entry = readdir(dirp);
        if (!entry)
            break;
        if (memcmp(entry->d_name, "p2c", 3) != 0) // ignore otherFile
            continue;
        // push dll point to map
        if (((memcmp(entry->d_name, "p2c_mod", 7) == 0) || (memcmp(entry->d_name, "p2c_gen", 7) == 0)) && (memcmp(strchr(entry->d_name, '.'), ".so", 3) == 0))
        {
            char full_path[256];
            strcpy(full_path, LIB_PATH);
            strcat(full_path, entry->d_name);
            void *mod = dlopen(full_path, RTLD_LAZY);
            if (!mod)
            {
                p2c_alerter::alerting(WARN, "library load failed:", entry->d_name);
                dlerror();
                continue;
            }
            this->_opened_lib.push_back(mod);
            p2c_alerter::alerting(DEBUG, "'p2c_liblist':88: library loaded", entry->d_name);
            p2c_create_mod_t *create_mod = (p2c_create_mod_t *)dlsym(mod, "p2c_create_mod");
            const char *dlsym_error = dlerror();
            if (dlsym_error)
            {
                p2c_alerter::alerting(WARN, "library open failed:", dlsym_error);
                continue;
            }
            p2c_mod *new_mod = create_mod();
            this->_created_mod.push_back(new_mod);
            p2c_alerter::alerting(DEBUG, "'p2c_liblist':98: library opened", entry->d_name);
            for (std::string key : new_mod->getCommand())
            {
                if ((memcmp(entry->d_name, "p2c_mod", 7) == 0))
                    this->_mod_map[key] = new_mod;
                else
                    this->_gen_map[key] = new_mod;
            }
        }
        else
            continue;
    }
    closedir(dirp);
    p2c_alerter::alerting(TRACE, "loaded", _created_mod.size(), "library.");
}

int p2c_liblist::callModFunc(std::string arg)
{
    std::size_t pos = arg.find(" ");
    std::string cmd = arg, token;
    if (pos != std::string::npos)
    {
        cmd = arg.substr(0, pos);
        token = arg.substr(pos + 1);
    }
    p2c_alerter::alerting(DEBUG, "'p2c_liblist':123: callModFunc() key=", cmd);
    if (this->_mod_map.count(cmd))
        return this->_mod_map[cmd]->entry(cmd, token);
    else
    {
        p2c_alerter::alerting(ERROR, "Usage: p2c [options]\n");
        return 1;
    }
}
int p2c_liblist::callGenFunc(std::string arg)
{
    std::size_t pos = arg.find(" ");
    std::string cmd = arg, token;
    if (pos != std::string::npos)
    {
        cmd = arg.substr(0, pos);
        token = arg.substr(pos + 1);
    }
    p2c_alerter::alerting(DEBUG, "'p2c_liblist':141: callGenFunc() key=", cmd);
    if (this->_gen_map.count(cmd))
        return this->_gen_map[cmd]->entry(cmd, token);
    else
    {
        p2c_alerter::alerting(ERROR, "Usage: p2c [options]\n");
        return 1;
    }
}

#endif
