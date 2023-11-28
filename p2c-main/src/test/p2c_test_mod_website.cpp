#define _ALERT_LEVEL_ 0

#include <p2c_lib.h>

#include <iostream>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[])
{
    p2c_liblist libList;
    argTable = new p2c_argtable;
    libList.callModFunc("--website");
    cout << argTable->getJSON() << '\n';
    return 0;
}
