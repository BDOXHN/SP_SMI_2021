#include <iostream>
#include <stdlib.h>
#include <vector>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>

using namespace std;

string currentIP;

// check if the given string is a numeric string or not
bool checkNum(const string& str){
    return !str.empty() &&
    (str.find_first_not_of("[0123456789]") == string::npos);
}

// split the given string using goven delimiter
vector<string> split(const string& str, char delim){
   auto i = 0;
   vector<string> list;
   auto pos = str.find(delim);
   while (pos != string::npos){
      list.push_back(str.substr(i, pos - i));
      i = ++pos;
      pos = str.find(delim, pos);
   }
   list.push_back(str.substr(i, str.length()));
   return list;
}

// validate IP address C class
bool validateIP(string ip){
    // split the string to tokens
    vector<string> slist = split(ip, '.');
    // check that tokens = 4
    if (slist.size() != 4){
        return false;
    }
    // check 1 token
    if (!checkNum(slist[0]) || stoi(slist[0]) < 192 || stoi(slist[0]) > 223){
        return false;
    }
    // check 2 token
    if (!checkNum(slist[1]) || stoi(slist[1]) < 0 || stoi(slist[1]) > 255){
        return false;
    }
    // check 3 token
    if (!checkNum(slist[2]) || stoi(slist[2]) < 0 || stoi(slist[2]) > 255){
        return false;
    }
    // check 4 token
    if (!checkNum(slist[3]) || (stoi(slist[3]) != 0)){
        return false;
    }
    return true;
} 

// ping range. Use only after validateIP
void pingRange(string ip, int min, int max){
    vector<string> slist1 = split(ip, '.');
    for (int j = 0; j < 4; j++){
        if (j == 3){
            currentIP += to_string(min);
        }
        else{
            currentIP = currentIP + slist1[j] + '.';
        }
    }
    //auto a1 = currentIP.c_str(); 
    execl("smi4_1.out", currentIP.c_str(), to_string(max).c_str(), NULL);
    currentIP.clear();
}

int main(int argc, char *argv[], char *enpv[]){
    string ipnet;
    int rv1, rv2, rv3, rv4;
    bool inputip = true;
    while (inputip){
        cout << "Input C class IP network ([192-223].[0-255].[0-255].0): ";
        cin >> ipnet;

        //check net for C class
        if (validateIP(ipnet)){
            cout << endl << "***It is a Valid IP network of C class***\n";
            break;
        }
        else{
            cout << endl << "***Invalid IP network***\n";
        }
    }

    // procces
    pid_t   proc1,
            proc2,
            proc3,
            proc4;

    proc1 = fork();
    proc2 = fork();
    proc3 = fork();
    proc4 = fork();

    if ((proc1 == 0) && (proc2 != 0) && (proc3 != 0) && (proc4 != 0)){
        rv1 = getpid();
        setpriority(PRIO_PROCESS, rv1, 5);
        pingRange(ipnet, 1, 63);
        exit(rv1);
    } else if ((proc1 != 0) && (proc2 == 0) && (proc3 != 0) && (proc4 != 0)){
        rv2 = getpid();
        setpriority(PRIO_PROCESS, rv2, 4);
        pingRange(ipnet, 64, 127);
        exit(rv2);
    } else if ((proc1 != 0) && (proc2 != 0) && (proc3 == 0) && (proc4 != 0)){
        rv3 = getpid();
        setpriority(PRIO_PROCESS, rv3, 3);
        pingRange(ipnet, 128, 191);
        exit(rv3);
    } else if ((proc1 != 0) && (proc2 != 0) && (proc3 != 0) && (proc4 == 0)){
        rv4 = getpid();
        setpriority(PRIO_PROCESS, rv1, 2);
        pingRange(ipnet, 192, 254);
        exit(rv4);
    } else if ((proc1 > 0) && (proc2 > 0) && (proc3 > 0) && (proc4 > 0)){
        sleep(15);
        kill(proc1, SIGKILL);
        kill(proc2, SIGKILL);
        kill(proc3, SIGKILL);
        kill(proc4, SIGKILL);
    }
}