#include <p2c_lib.h>
#include <dlfcn.h>

using namespace std;
int main(int argc, char const *argv[])
{
    void *mod = dlopen("/lib/p2c/p2c_generator_dockerfile.so", RTLD_LAZY);
    cerr << dlerror() << '\n';
    dlerror();
    dlclose(mod);
    return 0;
}
