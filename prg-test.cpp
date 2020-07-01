#include "prg.h"
#include<iostream>

using namespace std;

int main() {
    int a = 4;
    PseudoRandomGenerator x;

    unsigned char key[16] = {'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a'};

    unsigned char out[48];
    for(int i=0; i<48; i++) {
        cout<<i<<" "<<(int)out[i]<< " ";
    }
    cout<<endl;
    x.generate_random_number(out, key, 48, a);

    for(int i=0; i<48; i++) {
        cout<<i<<" "<<(int)out[i]<<" ";
    }
}