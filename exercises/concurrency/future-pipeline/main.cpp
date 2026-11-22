#include <iostream>
#include <future>
#include <thread>

using namespace std;

void take(promise<int> &p_in);
bool isPrime(int number);
void check(future<int> &f_in, promise<bool> &p_res);
void give(future<bool> &f_res);

int main(void){
    promise<int> p_in;
    auto f_in = p_in.get_future();

    promise<bool> p_res;
    auto f_res = p_res.get_future();

    thread takeT(take, ref(p_in));
    thread checkT(check, ref(f_in), ref(p_res));
    thread giveT(give, ref(f_res));

    takeT.join();
    checkT.join();
    giveT.join();
    return 0;
}

void take(promise<int> &p_in){
    int n;
    cout << "Insert a number: ";
    cin >> n;
    p_in.set_value(n);
}

void check(future<int> &f_in, promise<bool> &p_res){
    int n = f_in.get();
    bool is_prime = isPrime(n);
    p_res.set_value(is_prime);
}

void give(future<bool> &f_res){
    bool is_prime = f_res.get();
    if(is_prime)
        cout << "The number is prime";
    else
        cout << "The number is not prime";
}

bool isPrime(int number){
    if(number < 2) return false;
    if(number == 2) return true;
    if(number % 2 == 0) return false;
    for(int i=3; (i*i)<=number; i+=2){
        if(number % i == 0 ) return false;
    }
    return true;
}