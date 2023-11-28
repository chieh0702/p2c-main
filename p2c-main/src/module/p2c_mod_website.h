#include <p2c_lib.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <dlfcn.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

std::set<std::string> findPath(std::string path, std::string str)
{
    std::set<std::string> result;
    if (path[path.size() - 1] != '/')
        path += '/';
    DIR *dirp = opendir(path.c_str());
    if (!dirp)
        p2c_alerter::alerting(ERROR, "Open", path, "failed: ", strerror(errno));
    while (1)
    {
        dirent *entry = readdir(dirp);
        if (!entry)
            break;
        if (entry->d_type == DT_DIR && entry->d_name[0] != '.')
        {
            std::set<std::string> temp = findPath(path + entry->d_name + '/', str);
            result.insert(temp.begin(), temp.end());
        }
        if (entry->d_type == DT_REG)
        {
            std::ifstream file(path + entry->d_name);
            if (file.fail())
            {
                closedir(dirp);
                p2c_alerter::alerting(ERROR, "Open", path + entry->d_name, "failed: ", strerror(errno));
            }
            std::string token;
            while (file >> token)
            {
                if (token == str)
                {
                    file >> token;
                    if (token[token.size() - 1] != '/')
                        token += '/';
                    result.insert(token);
                }
            }
            file.close();
        }
    }
    closedir(dirp);
    return result;
}

void parser_Apache()
{
    const char apache_path[] = "/etc/apache2/";
    p2c_alerter::alerting(DEBUG, "start parser apache.", "apache path:", apache_path);
    argTable->addArg("RUN", "apt-get update; apt-get install apache2 -y");
    argTable->addArg("COPY", apache_path);
    argTable->addArg("COPY", apache_path);
    std::set<std::string> docRoot = findPath(apache_path, "DocumentRoot");
    for (std::string path : docRoot)
    {
        p2c_alerter::alerting(DEBUG, "find DocumentRoot:", path);
        argTable->addArg("COPY", path);
        argTable->addArg("COPY", path);
    }
}

void parser_Nginx()
{
}