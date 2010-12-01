#include <signal.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    int B;
    assert(read(0, &B, sizeof(int)) == sizeof(int));
    char *fileName = "monster.log";
    std::ofstream fout(fileName);
    
    fout << "Monster : " << B << endl;    
    
    for (int i = 0; i < B; ++i)
    {
        int pid;
        assert(read(0, &pid, sizeof(int)) == sizeof(int));
        fout << "Monster pid: " << pid << endl;    
        kill(pid, SIGKILL);
    }
    fout << "Monster finished" << endl;        
}

