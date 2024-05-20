#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include "ComputerClub.h"
using namespace std;

int main(int argc, char* argv[]) {
    string file = argv[1];
    ComputerClub ComputerClub(file);
    cout << ComputerClub << endl;
}

