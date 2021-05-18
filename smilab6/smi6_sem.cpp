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
#include <semaphore.h>

using namespace std;

string currentIP;
string ipnet;
int rv1, rv2, rv3, rv4;
bool inputip = true;
int counter, min_ip, max_ip, status;

//semaphore
sem_t semaphore1, semaphore2, semaphore3, semaphore4;

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

    string a = "nmap -T5 -sn ";
    string b = currentIP;
    a += b;
    string c = "-";
    a += c;
    string d = to_string(max);
    a += d;
    cout << "Scan IPs: " << currentIP << "-" << d << endl;
    system(a.c_str());
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
    sem_post(&semaphore1);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    return NULL;
}


void* thread_fun_pingrange1(void* arg){
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    printf("\n>>> pid(thread group id): %d\n>>> tid(self thread id): %lu \n",
            getpid(),pthread_self());
    min_ip = 1;
    max_ip = 63;
    pingRange(ipnet, min_ip, max_ip);
    sem_post(&semaphore2);
 
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    return NULL;
}

void* thread_fun_pingrange2(void* arg){
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    printf("\n>>> pid(thread group id): %d\n>>> tid(self thread id): %lu \n",
            getpid(),pthread_self());
    sem_wait(&semaphore2);
    min_ip = 64;
    max_ip = 127;
    pingRange(ipnet, min_ip, max_ip);
    sem_post(&semaphore3);
 
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    return NULL;
}

void* thread_fun_pingrange3(void* arg){
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    printf("\n>>> pid(thread group id): %d\n>>> tid(self thread id): %lu \n",
            getpid(),pthread_self());
    sem_wait(&semaphore3);
    min_ip = 128;
    max_ip = 191;
    pingRange(ipnet, min_ip, max_ip);
    sem_post(&semaphore4);
 
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    return NULL;
}

void* thread_fun_pingrange4(void* arg){
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    printf("\n>>> pid(thread group id): %d\n>>> tid(self thread id): %lu \n",
            getpid(),pthread_self());
    sem_wait(&semaphore4);
    min_ip = 192;
    max_ip = 254;
    pingRange(ipnet, min_ip, max_ip);
    sem_post(&semaphore1);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    return NULL;
}


void* thread_fun_queue(void* arg){
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_t   thread_ping_range1,
                thread_ping_range2,
                thread_ping_range3,
                thread_ping_range4;
    sem_wait(&semaphore1);

    status = pthread_create(&thread_ping_range1, NULL, thread_fun_pingrange1, NULL);
    if (status != 0){
        printf("2p");
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    status = pthread_create(&thread_ping_range2, NULL, thread_fun_pingrange2, NULL);
    if (status != 0){
        printf("2p");
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    status = pthread_create(&thread_ping_range3, NULL, thread_fun_pingrange3, NULL);
    if (status != 0){
        printf("2p");
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    status = pthread_create(&thread_ping_range4, NULL, thread_fun_pingrange4, NULL);
    if (status != 0){
        printf("2p");
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    sem_wait(&semaphore1);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    return NULL;
}

int main(int argc, char *argv[], char *enpv[]){
    sem_init(&semaphore1, 1, 0);
    sem_init(&semaphore2, 1, 0);
    sem_init(&semaphore3, 1, 0);
    sem_init(&semaphore4, 1, 0);
    pthread_t thread_input;
    pthread_t thread_queue;
    status = pthread_create(&thread_input, NULL, thread_fun_inputip, NULL);
    if (status != 0){
        printf("1p");
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    status = pthread_create(&thread_queue, NULL, thread_fun_queue, NULL);
    if (status != 0){
        printf("1p");
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    

    pthread_cancel(thread_input);
    pthread_join(thread_input, NULL);
    pthread_cancel(thread_queue);
    pthread_join(thread_queue, NULL);
    sem_destroy(&semaphore1);
    sem_destroy(&semaphore2);
    sem_destroy(&semaphore3);
    sem_destroy(&semaphore4);
}