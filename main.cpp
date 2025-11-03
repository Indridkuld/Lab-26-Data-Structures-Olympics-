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

int main() {
    // results[0] = per-run (ROWS x COLS); results[1] = accumulators (sums across runs)
    int results[2][ROWS][COLS] = {0};
    string cd;

    for (int run = 0; run < SIMS; ++run) {
        // zero per-run slice
        for (int r = 0; r < ROWS; ++r)
            for (int c = 0; c < COLS; ++c)
                results[0][r][c] = 0;

        // read master containers once per run
        vector<string> master_vec;
        list<string> master_list;
        set<string> master_set;
        ifstream fin_master("codes.txt");
        if (!fin_master.is_open()) {
            cerr << "Error: could not open codes.txt\n";
            return 1;
        }
        while (fin_master >> cd) {
            master_vec.push_back(cd);
            master_list.push_back(cd);
            master_set.insert(cd);
        }
        fin_master.close();

        if (master_vec.empty()) {
            cerr << "Error: codes.txt produced no tokens.\n";
            return 1;
        }

        // testing for READ operations
        for (int i = 0; i < STRUCTURES; i++) {
            ifstream fin("codes.txt");
            auto start = chrono::high_resolution_clock::now();
            switch(i) {
                case 0: {  // read into a vector
                    vector<string> tmp;
                    while (fin >> cd)
                        tmp.push_back(cd);
                    break;
                }
                case 1: {  // read into a list
                    list<string> tmp;
                    while (fin >> cd)
                        tmp.push_back(cd);
                    break;
                }
                case 2: {  // read into a set
                    set<string> tmp;
                    while (fin >> cd)
                        tmp.insert(cd);
                    break;
                }
            }
            auto end = chrono::high_resolution_clock::now();
            results[0][0][i] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            fin.close();
        }

        // testing for SORT operations
        for (int i = 0; i < STRUCTURES; i++) {
            auto start = chrono::high_resolution_clock::now();
            switch(i) {
                case 0: {  // sort a vector
                    vector<string> tmp = master_vec;
                    sort(tmp.begin(), tmp.end());
                    break;
                }
                case 1: {  // sort a list
                    list<string> tmp(master_list.begin(), master_list.end());
                    tmp.sort();
                    break;
                }
                case 2: {  // can't sort a set, so record 0
                    break;
                }
            }
            auto end = chrono::high_resolution_clock::now();
            results[0][1][i] = chrono::duration_cast<chrono::microseconds>(end - start).count();
        }

        // testing for INSERT operations
        for (int i = 0; i < STRUCTURES; i++) {
            auto start = chrono::high_resolution_clock::now();
            switch(i) {
                case 0: {  // insert into a vector
                    vector<string> tmp = master_vec;
                    size_t ind = tmp.size() / 2;
                    tmp.insert(tmp.begin() + ind, "TESTCODE");
                    break;
                }
                case 1: {  // insert into a list
                    list<string> tmp(master_list.begin(), master_list.end());
                    auto it = tmp.begin();
                    advance(it, tmp.size() / 2);
                    tmp.insert(it, "TESTCODE");
                    break;
                }
                case 2: {  // insert into a set
                    set<string> tmp = master_set;
                    tmp.insert("TESTCODE");
                    break;
                }
            }
            auto end = chrono::high_resolution_clock::now();
            results[0][2][i] = chrono::duration_cast<chrono::microseconds>(end - start).count();
        }

        // testing for DELETE operations
        for (int i = 0; i < STRUCTURES; i++) {
            auto start = chrono::high_resolution_clock::now();
            switch(i) {
                case 0: {  // delete by value from vector
                    vector<string> tmp = master_vec;
                    size_t ind = tmp.size() / 2;
                    string target = tmp[ind];
                    tmp.erase(remove(tmp.begin(), tmp.end(), target), tmp.end());
                    break;
                }
                case 1: {  // delete by value from list
                    list<string> tmp(master_list.begin(), master_list.end());
                    auto it = tmp.begin();
                    advance(it, tmp.size() / 2);
                    string target = *it;
                    tmp.remove(target);
                    break;
                }
                case 2: {  // delete by value from set
                    set<string> tmp = master_set;
                    auto it = tmp.begin();
                    advance(it, tmp.size() / 2);
                    string target = *it;
                    tmp.erase(target);
                    break;
                }
            }
            auto end = chrono::high_resolution_clock::now();
            results[0][3][i] = chrono::duration_cast<chrono::microseconds>(end - start).count();
        }

        // accumulate this run
        for (int r = 0; r < ROWS; ++r)
            for (int c = 0; c < COLS; ++c)
                results[1][r][c] += results[0][r][c];
    }

    string labels[] = {"Read", "Sort", "Insert", "Delete"};
    cout << "Number of simulations: " << SIMS << endl;
    cout << setw(W1) << "Operation" << setw(W1) << "Vector" << setw(W1) << "List"
         << setw(W1) << "Set" << endl;
    for (int i = 0; i < ROWS; i++) {
        cout << setw(W1) << labels[i];
        for (int j = 0; j < COLS; j++) 
            cout << setw(W1) << (results[1][i][j] / SIMS);
        cout << endl;
    }

    return 0;
}