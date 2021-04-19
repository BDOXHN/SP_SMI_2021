#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
using namespace std;

int main(int argc, char *argv[]){
    string a = "nmap -T5 -sn ";
    string b = argv[0];
    a += b;
    string c = "-";
    a += c;
    string d = argv[1];
    a += d;
    cout << "Scan IPs: " << argv[0] << "-" << argv[1] << endl;
    //string a = argv[1];
    system(a.c_str());
}