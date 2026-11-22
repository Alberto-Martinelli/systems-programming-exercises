#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <memory>
#include <sstream>
#include <vector>

using namespace std;

void write_file(string filename, string text);
void read_file(string filename); // for debug purposes
int* thread_read_file(string filename, int* &arr, int &length);
void write_output(string filename, vector<int> arr);

int main(void){
    int n = 3;

    string *input_files = new string[n];
    input_files[0] = "input1.bin";
    input_files[1] = "input2.bin";
    input_files[2] = "input3.bin";
    string output_filename = "output.bin";

    // write_file(input_filename1, "5 23 45 67 9 23");
    // write_file(input_filename2, "12 90 65 34 24 12 0 89 29 54 12 78 3");
    // write_file(input_filename3, "3 9 5 1");

    int **collector = new int*[n]; // collects the pointer to array of each thread
    int *lengths = new int[n]; // lengths of the array of each thread
    thread *my_threads = new thread[n]; // dynamic array of threads
    
    // start the threads
    int i, j;
    for(i=0; i<n; i++){
        my_threads[i] = thread(thread_read_file, input_files[i], ref(collector[i]), ref(lengths[i]));
    }

    // wait for the termination of each thread
    for(i=0; i<n; i++){
        my_threads[i].join();  
    }

    // merge the n arrays into one sequence of integers (I chose a vector data structure)
    vector<int> final_arr;
    for(i=0; i<n; i++){
        for(j=0; j<lengths[i]; j++){
            final_arr.push_back(collector[i][j]);
        }
    }
    
    write_output(output_filename, final_arr); // writes the final array in output.bin
    read_file(output_filename); // reads output.bin and prints it on stdout

    // deallocate memory
    delete[] input_files;
    for(i=0; i<n; i++){
        delete[] collector[i];
    }
    delete[] collector;
    delete[] lengths;
    delete[] my_threads;
    return 0;
}

void read_file(string filename){
    ifstream my_file(filename, ifstream::in | ifstream::binary);
    if(!my_file.is_open()){
        cerr << "Error opening file " << filename << endl;
        exit(EXIT_FAILURE);
    }
    
    int val;
    cout << "Content of the file '" << filename << "':" << endl;
    while(my_file.read((char*)&val, sizeof(int))){
        cout << val << " ";
    }
    
    cout << "\n";

    my_file.close();
}

void write_file(string filename, string text){
    ofstream my_file(filename, ofstream::out | ofstream::binary);
    if(!my_file.is_open()){
        cerr << "Error opening file " << filename << endl;
        exit(EXIT_FAILURE);
    }
    int size;
    istringstream iss(text);
    iss >> size; // reads the first integer of the string
    my_file.write((char *)&size, sizeof(int)); // writes it on the file

    int i, val;
    for(i=0; i<size; i++){
        iss >> val; // read int from the string
        // cout << val << " "; // debug
        my_file.write((char *)&val, sizeof(int));
    }
    
    // cout << "\n";

    my_file.close();
}

int* thread_read_file(string filename, int* &arr, int &length){
    // thread opens its input file
    ifstream my_file;
    my_file.open(filename, ifstream::in | ifstream::binary); // opens the file in binary mode
    if(!my_file.is_open()){
        cerr << "Error opening file " << filename << endl;
        exit(EXIT_FAILURE);
    }
    // reads the first integer value
    my_file.read((char *)&length, sizeof(int)); // reads the first integer (which is the # of integers to read next)
    arr = new int[length]; // arr is a pointer to a dynamic array of num integers (stored in the heap) (in case of error it throws an exception)

    int i;
    for(i=0; i<length; i++){
        // read into array the integers in the binary file
        my_file.read((char *)&(arr[i]), sizeof(int));
    }

    sort(arr, arr+length); // sort the array in ascending order

    my_file.close();

    return arr;
}

void write_output(string filename, vector<int> arr){
    ofstream my_file(filename, ofstream::out | ofstream::binary);
    if(!my_file.is_open()){
        cerr << "Error opening file " << filename << endl;
        exit(EXIT_FAILURE);
    }
    
    sort(arr.begin(), arr.end());

    int lenght = arr.size();
    my_file.write((char*)&lenght, sizeof(int));

    for(auto el : arr){
        my_file.write((char*)&el, sizeof(int));
    }

    my_file.close();
}