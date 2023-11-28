#include "gendf.h"
using namespace std;
int main(int argc, char const *argv[])
{
    if (argc <= 1)
        showHelp();
    string dockerfile;
    vector<string> tokens;
    for (int i = 1; i < argc; i++)
        tokens.push_back(argv[i]);
    Generator generator = Generator(tokens);
    while (!generator.is_EOF())
        dockerfile += generator.getline();
    ofstream fout;
    fout.open(generator.getOutput() + "Dockerfile"); // TODO:"/"
    fout << dockerfile;                              // TODO:command order
    fout.close();
    if (!generator.getIgnore().empty())
    {
        fout.open(generator.getOutput() + ".dockerignore"); // TODO:"/"
        fout << generator.getIgnore();
        fout.close();
    }
    return 0;
}
