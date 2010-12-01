#include <stdio.h>
#include "assert.h"
#include <fcntl.h>
#include <fstream>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    assert(argc >= 3);
    std::ifstream fin(argv[2]);
    std::ofstream fout(argv[1]);
    
    assert(fin);
    
    int id, msg, len;
    fout << "Red node : " << argv[1] << " - " << argv[2] << endl;
    while (fin >> id >> msg >> len)
    {
        int* buf;
        buf = (int*)malloc((3 + len) * sizeof(int));
        buf[0] = id;
        buf[1] = msg;
        buf[2] = len;
 
        for (int i = 0; i < len; ++i)
        {
            fin >> buf[i + 3];
        }
        
        fout << "Red node: " << argv[1] << " : " << id << " " << msg << endl;        
        assert(write(1, buf, (3 + len) * sizeof(int)) == (3 + len) * sizeof(int));
        free(buf);
    }
    fout << "Red node: " << argv[1] << " : exited"  << endl;        
    return 0;
}