#define _ALERT_LEVEL_ 0

#include <iostream>
#include <p2c_lib.h>

using namespace std;

int main(int argc, char const *argv[])
{
    p2c_argtable args;
    args.initArgs(argc, argv);
    cout << "init_args{";
    queue<string> *command = args.getArg("init_args");
    if (command)
        while (!command->empty())
        {
            cout << '\"' << command->front() << "\"|";
            command->pop();
        }
    cout << "}\n\n";
    cout << "addArg 'say_hello' 'vector test'\n";
    args.addArg("say_hello", "hello tim");
    vector<string> vector = {"1", "2", "3"};
    args.addArg("vector test", vector);
    cout << "output JSON :\n"
         << args.getJSON() << "\n\n";
    cout << "get error arg 'none':";
    if (args.getArg("none"))
        cout << "vector size = " << args.getArg("none")->size();
    return 0;
}
