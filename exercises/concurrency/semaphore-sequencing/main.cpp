#include <iostream>
#include <thread>
#include <semaphore>
#include <unistd.h>
#include <mutex>

using namespace std;

int n;
counting_semaphore s1{0}, s2{0}, s3{0}, s4{0}, s5{0};

void tp();
void tm();
void ts();
void tnl();

int main(int argc, char** argv){
    // if(argc != 2){
    //     cerr << "Wrong number of arguments" << endl;
    //     exit(EXIT_FAILURE);
    // }
    // n = atoi(argv[1]);
    n = 10;

    thread t1(tp);
    thread t2(tm);
    thread t3(ts);
    thread t4(tnl);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}

void tp(){
    int count = 0;
    while(1){
        cout << "+";
        count++;
        if(count==n){
            sleep(1);
            count = 0;
            s4.release(); // signal t4 to print newline
            s5.acquire(); // wait for t4
            s2.release(); // t2's turn
            s1.acquire(); // stop here
        }
    }
}

void tm(){
    s2.acquire(); // stop here
    int count = 0;
    while(1){
        cout << "-";
        count++;
        if(count==n){
            sleep(1);
            count = 0;
            s4.release(); // signal t4 to print newline
            s5.acquire(); // wait for t4
            s3.release(); // signal t3
            s2.acquire(); // stop here
        }
    }
}

void ts(){
    s3.acquire(); // stop here
    int count = 0;
    while(1){
        cout << "*";
        count++;
        if(count==n){
            sleep(1);
            count = 0;
            s4.release(); // signal t4 to print newline
            s5.acquire(); // wait for t4
            s1.release(); // signal t1
            s3.acquire(); // stop here
        }
    }
}

void tnl(){
    s4.acquire(); // stop here
    while(1){
        cout << endl;
        sleep(1);
        s5.release(); // "I'm done printing newline"
        s4.acquire(); // stop here
    }
}