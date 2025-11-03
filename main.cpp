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
    // results[0] = per-run matrix (ROWS x COLS)
    // results[1] = accumulators (sums across runs)
    long long results[2][ROWS][COLS] = {0};
    string cd;

    // Run the experiment SIMS times
    for (int run = 0; run < SIMS; ++run) {
        // zero per-run slice
        for (int r = 0; r < ROWS; ++r)
            for (int c = 0; c < COLS; ++c)
                results[0][r][c] = 0;

        // Read master containers from file for this run
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

        // READ timings: re-open and read into temporary containers per structure
        for (int st = 0; st < STRUCTURES; ++st) {
            auto start = chrono::high_resolution_clock::now();
            if (st == 0) {
                vector<string> tmp;
                ifstream fin("codes.txt");
                while (fin >> cd) tmp.push_back(cd);
            } else if (st == 1) {
                list<string> tmp;
                ifstream fin("codes.txt");
                while (fin >> cd) tmp.push_back(cd);
            } else {
                set<string> tmp;
                ifstream fin("codes.txt");
                while (fin >> cd) tmp.insert(cd);
            }
            auto end = chrono::high_resolution_clock::now();
            results[0][0][st] = chrono::duration_cast<chrono::microseconds>(end - start).count();
        }

        // SORT timings
        for (int st = 0; st < STRUCTURES; ++st) {
            if (st == 0) {
                vector<string> tmp = master_vec;
                auto start = chrono::high_resolution_clock::now();
                sort(tmp.begin(), tmp.end());
                auto end = chrono::high_resolution_clock::now();
                results[0][1][st] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            } else if (st == 1) {
                list<string> tmp(master_list.begin(), master_list.end());
                auto start = chrono::high_resolution_clock::now();
                tmp.sort();
                auto end = chrono::high_resolution_clock::now();
                results[0][1][st] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            } else {
                // set is already ordered; we record 0 for clarity in averages
                results[0][1][st] = 0;
            }
        }

        // INSERT timings
        for (int st = 0; st < STRUCTURES; ++st) {
            if (st == 0) {
                vector<string> tmp = master_vec;
                size_t ind = tmp.size() / 2;
                auto start = chrono::high_resolution_clock::now();
                tmp.insert(tmp.begin() + ind, "TESTCODE");
                auto end = chrono::high_resolution_clock::now();
                results[0][2][st] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            } else if (st == 1) {
                list<string> tmp(master_list.begin(), master_list.end());
                auto it = tmp.begin();
                advance(it, tmp.size() / 2);
                auto start = chrono::high_resolution_clock::now();
                tmp.insert(it, "TESTCODE");
                auto end = chrono::high_resolution_clock::now();
                results[0][2][st] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            } else {
                set<string> tmp = master_set;
                auto start = chrono::high_resolution_clock::now();
                tmp.insert("TESTCODE");
                auto end = chrono::high_resolution_clock::now();
                results[0][2][st] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            }
        }

        // DELETE timings
        for (int st = 0; st < STRUCTURES; ++st) {
            if (st == 0) {
                vector<string> tmp = master_vec;
                size_t ind = tmp.size() / 2;
                string target = tmp[ind];
                auto start = chrono::high_resolution_clock::now();
                tmp.erase(remove(tmp.begin(), tmp.end(), target), tmp.end());
                auto end = chrono::high_resolution_clock::now();
                results[0][3][st] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            } else if (st == 1) {
                list<string> tmp(master_list.begin(), master_list.end());
                auto it = tmp.begin();
                advance(it, tmp.size() / 2);
                string target = *it;
                auto start = chrono::high_resolution_clock::now();
                tmp.remove(target);
                auto end = chrono::high_resolution_clock::now();
                results[0][3][st] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            } else {
                set<string> tmp = master_set;
                auto it = tmp.begin();
                advance(it, tmp.size() / 2);
                string target = *it;
                auto start = chrono::high_resolution_clock::now();
                tmp.erase(target);
                auto end = chrono::high_resolution_clock::now();
                results[0][3][st] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            }
        }

        // accumulate per-run into results[1]
        for (int r = 0; r < ROWS; ++r)
            for (int c = 0; c < COLS; ++c)
                results[1][r][c] += results[0][r][c];
    }
    string labels[] = {"Read", "Sort", "Insert", "Delete"};
    // Print final averaged results
    cout << "Number of simulations: " << SIMS << "\n";
    cout << setw(W1) << "Operation" << setw(W1) << "Vector" << setw(W1) << "List"
         << setw(W1) << "Set" << endl;
    for (int r = 0; r < ROWS; ++r) {
        cout << setw(W1) << labels[r];
        for (int c = 0; c < COLS; ++c) {
            long long avg = results[1][r][c] / SIMS;
            cout << setw(W1) << avg;
        }
        cout << endl;
    }
    

    return 0;
}