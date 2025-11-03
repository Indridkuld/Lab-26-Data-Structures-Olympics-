// COMSC-210 | Lab 26: Data Structures Olympics| Aidan Woodcock | 2025-11-02
// IDE used: Visual Studio Code
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <set>
#include <list>
#include <iomanip>
using namespace std;

// const int SZ = 20000, COLS = 3, ROWS = 4, TESTS = 4;
const int STRUCTURES = 3;
const int ROWS = 4, COLS = 3, SIMS = 15;
const int W1 = 10;
vector<string> data_vector;
list<string> data_list;
set<string> data_set;

int main() {
    int results[2][ROWS][COLS] = {0};
    string cd;
    for (int i = 0; i < SIMS; i++) { // run experiement SIMS times
        for (int t = 0; t < ROWS; t++)  
            for (int j = 0; j < COLS; j++) 
                results[0][t][j] = 0;
        
        // testing for READ operations
            for (int i = 0; i < STRUCTURES; i++) {
                ifstream fin("codes.txt");
                vector<string> tmp;
                auto start = chrono::high_resolution_clock::now();
                switch(i) {
                    case 0: {  // read into a vector
                        while (fin >> cd)
                                tmp.push_back(cd);
                        auto end = chrono::high_resolution_clock::now();
                        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                        results[0][0][i] = duration.count();
                        break;
                    }
                    case 1: {  // read into a list
                        while (fin >> cd)
                                data_list.push_back(cd);
                        auto end = chrono::high_resolution_clock::now();
                        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                        results[0][0][i] = duration.count();
                        break;
                    }
                    case 2: {  // read into a set
                        while (fin >> cd)
                                data_set.insert(cd);
                        auto end = chrono::high_resolution_clock::now();
                        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                        results[0][0][i] = duration.count();
                        break;
                    }
                }
                fin.close();
            }

            // testing for SORT operations
            for (int i = 0; i < STRUCTURES; i++) {
                auto start = chrono::high_resolution_clock::now();
                switch(i) {
                    case 0: {  // sort a vector
                        vector<string> tmp = data_vector;
                        sort(tmp.begin(), tmp.end());
                        auto end = chrono::high_resolution_clock::now();
                        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                        results[0][1][i] = duration.count();
                        break;
                    }
                    case 1: {  // sort a 
                        list<string> tmp(data_list.begin(), data_list.end());
                        data_list.sort();
                        auto end = chrono::high_resolution_clock::now();
                        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                        results[0][1][i] = duration.count();
                        break;
                    }
                    case 2: {  // can't sort a set, so set to -1
                        results[0][1][i] = -1;
                        break;
                    }
                }
            }

            // testing for INSERT operations
            for (int i = 0; i < STRUCTURES; i++) {
                auto start = chrono::high_resolution_clock::now();
                switch(i) {
                    case 0: {  // insert into a vector
                        vector<string> tmp = data_vector;
                        size_t ind = tmp.size() / 2;
                
                        data_vector.insert(data_vector.begin() + ind_v, "TESTCODE");
                        auto end = chrono::high_resolution_clock::now();
                        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                        results[0][2][i] = duration.count();
                        break;
                    }
                    case 1: {  // insert into a list
                        list<string> tmp(data_list.begin(), data_list.end());

                        auto it = tmp.begin();
                        advance(it, tmp.size() / 2);
                        data_list.insert(it, "TESTCODE");

                        auto end = chrono::high_resolution_clock::now();
                        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                        results[0][2][i] = duration.count();
                        break;
                    }
                    case 2: {  // insert into a set
                        data_set.insert("TESTCODE");
                        auto end = chrono::high_resolution_clock::now();
                        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                        results[2][i] = duration.count();
                        break;
                    }
                }
            }

            // testing for DELETE operations
            for (int i = 0; i < STRUCTURES; i++) {
                // select a target value in the vector 
                int ind = data_vector.size() / 2;
                string target_v = data_vector[ind];

                // select a target value in the list
                auto it1 = data_list.begin();
                advance(it1, ind);
                string target_l = *it1;

                // select a target value in the set
                auto it2 = data_set.begin();
                advance(it2, ind);
                string target_s = *it2;
                
                auto start = chrono::high_resolution_clock::now();
                switch(i) {
                    case 0: {  // delete by value from vector
                        data_vector.erase(remove(data_vector.begin(), data_vector.end(), target_v));
                        auto end = chrono::high_resolution_clock::now();
                        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                        results[3][i] = duration.count();
                        break;
                    }
                    case 1: {  // delete by value from list
                        data_list.remove(target_l);
                        auto end = chrono::high_resolution_clock::now();
                        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                        results[3][i] = duration.count();
                        break;
                    }
                    case 2: {  // delete by value from set
                        data_set.erase(target_s);    
                        auto end = chrono::high_resolution_clock::now();
                        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                        results[3][i] = duration.count();
                        break;
                    }
                }
            }
        }
    }     
    string labels[] = {"Read", "Sort", "Insert", "Delete"};
    cout << setw(W1) << "Operation" << setw(W1) << "Vector" << setw(W1) << "List"
         << setw(W1) << "Set" << endl;
    for (int i = 0; i < 4; i++) {
        cout << setw(W1) << labels[i];
        for (int j = 0; j < COLS; j++) 
            cout << setw(W1) << results[i][j];
        cout << endl;
    }
    

    return 0;
}