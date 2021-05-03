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
#include <pthread.h>

using namespace std;

string currentIP;
string ipnet;
int rv1, rv2, rv3, rv4;
bool inputip = true;
int counter, min_ip, max_ip, status;

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

    string a = "nmap -T5 -sn ";
    string b = currentIP;
    a += b;
    string c = "-";
    a += c;
    string d = to_string(max);
    a += d;
    cout << "Scan IPs: " << currentIP << "-" << d << endl;
    //string a = argv[1];
    system(a.c_str());
    //execl("smi5_1.out", currentIP.c_str(), to_string(max).c_str(), NULL);
    currentIP.clear();
}

void* thread_fun_inputip(void* arg){
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    printf("\n>>> pid(thread group id): %d\n>>> tid(self thread id): %lu \n",
            getpid(),pthread_self());
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
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    //teper process mozhno zavershit
    pthread_testcancel(); //ostanov
    return NULL;
}

void* thread_fun_pingrange(void* arg){
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    printf("\n>>> pid(thread group id): %d\n>>> tid(self thread id): %lu \n",
            getpid(),pthread_self());
    if (counter == 0) {
        pingRange(ipnet, 1, 63);
    }
    if (counter == 1) {
        pingRange(ipnet, 64, 127);
    }
    if (counter == 2) {
        pingRange(ipnet, 128, 191);
    }
    if (counter == 3) {
        pingRange(ipnet, 192, 254);
    }
    //pingRange(ipnet, min_ip, max_ip);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    //teper process mozhno zavershit
    pthread_testcancel(); //ostanov
    return NULL;
}


int main(int argc, char *argv[], char *enpv[]){
    pthread_t thread_input;
    pthread_t thread_ping_range[ 4 ];
    status = pthread_create(&thread_input, NULL, thread_fun_inputip, NULL);
    if (status != 0){
        printf("1p");
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    sleep(15);
    //pthread_cancel(thread_input);
    //pthread_join(thread_input, NULL);

    for (counter = 0; counter < 4; counter++){
        if (counter == 0){
            min_ip = 1;
            max_ip = 63;
            status = pthread_create(&thread_ping_range[counter], NULL, thread_fun_pingrange, NULL);
            if (status != 0){
                printf("2p");
                perror("pthread_create");
                exit(EXIT_FAILURE);
            }
            sleep(10);
            continue;
        }
        if (counter == 1){
            min_ip = 64;
            max_ip = 127;
            status = pthread_create(&thread_ping_range[counter], NULL, thread_fun_pingrange, NULL);
            if (status != 0){
                printf("3p");
                perror("pthread_create");
                exit(EXIT_FAILURE);
            }
            sleep(10);
            continue;
        }
        if (counter == 2){
            min_ip = 128;
            max_ip = 191;
            status = pthread_create(&thread_ping_range[counter], NULL, thread_fun_pingrange, NULL);
            if (status != 0){
                printf("4p");
                perror("pthread_create");
                exit(EXIT_FAILURE);
            }
            sleep(10);
            continue;
        }
        if (counter == 3){
            min_ip = 192;
            max_ip = 254;
            status = pthread_create(&thread_ping_range[counter], NULL, thread_fun_pingrange, NULL);
            if (status != 0){
                printf("5p");
                perror("pthread_create");
                exit(EXIT_FAILURE);
            }
            sleep(10);
            continue;
        }
    }

    pthread_cancel(thread_input);
    pthread_join(thread_input, NULL);
    for (int counter = 0; counter < 4; counter++){
        pthread_cancel(thread_ping_range[counter]);
        pthread_join(thread_ping_range[counter], NULL);
    }
}