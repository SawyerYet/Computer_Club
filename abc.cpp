#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include "ComputerClub.h"
using namespace std;

int main(int argc, char* argv[]) {
    string file = argv[1];
    ComputerClub ComputerClub(file);
    //cout << ComputerClub.GetPlaces() << endl;
    //cout << ComputerClub.GetStart_time() << endl;
    //cout << ComputerClub.GetEnd_time() << endl;
    //cout << ComputerClub.GetPrice() << endl;
    //for(string client : ComputerClub.GetEvents_client()){
    //    cout << client << endl;
    //}
    cout << ComputerClub << endl;

}

