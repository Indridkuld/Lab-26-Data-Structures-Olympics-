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
const int ROWS = 4, COLS = 3, SIMS = 15; // ROWS == number of operations (Read,Sort,Insert,Delete)
const int OPS = ROWS; // alias for clarity when we use a 3D cube: [op][structure][sim]
const int W1 = 10;

int main() {
    // Use a 3D array (cube) to store multiple simulation runs.
    // cube[op][structure][sim]
    // - op (0..ROWS-1): operation slice (0=Read,1=Sort,2=Insert,3=Delete)
    // - structure (0..STRUCTURES-1): data structure slice (0=Vector,1=List,2=Set)
    // - sim (0..SIMS-1): repeated measurement index for averaging/stability
    long long cube[OPS][STRUCTURES][SIMS] = {0};
    string cd;
    vector<string> data_vector;
    list<string> data_list;
    set<string> data_set;

    // Read the data once to populate master containers used for non-read tests.
    ifstream fin_master("codes.txt");
    if (!fin_master.is_open()) {
        cerr << "Error: could not open codes.txt (place a sample file next to the executable)\n";
        return 1;
    }
    while (fin_master >> cd) {
        data_vector.push_back(cd);
        data_list.push_back(cd);
        data_set.insert(cd);
    }
    fin_master.close();

    if (data_vector.empty()) {
        cerr << "Error: codes.txt produced no tokens.\n";
        return 1;
    }

    // Measure READ operations SIMS times per structure (open file each sim so the file is re-read)
    for (int i = 0; i < STRUCTURES; i++) {
        for (int s = 0; s < SIMS; s++) {
            auto start = chrono::high_resolution_clock::now();
            if (i == 0) { // vector
                vector<string> tmp;
                ifstream fin("codes.txt");
                while (fin >> cd) tmp.push_back(cd);
            } else if (i == 1) { // list
                list<string> tmp;
                ifstream fin("codes.txt");
                while (fin >> cd) tmp.push_back(cd);
            } else if (i == 2) { // set
                set<string> tmp;
                ifstream fin("codes.txt");
                while (fin >> cd) tmp.insert(cd);
            }
            auto end = chrono::high_resolution_clock::now();
            cube[0][i][s] = chrono::duration_cast<chrono::microseconds>(end - start).count();
        }
    }

    // SORT operations: perform SIMS runs and store each run in the cube.
    for (int i = 0; i < STRUCTURES; i++) {
        for (int s = 0; s < SIMS; s++) {
            if (i == 0) { // vector
                vector<string> tmp = data_vector; // copy from master
                auto start = chrono::high_resolution_clock::now();
                sort(tmp.begin(), tmp.end());
                auto end = chrono::high_resolution_clock::now();
                cube[1][i][s] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            } else if (i == 1) { // list
                list<string> tmp(data_list.begin(), data_list.end());
                auto start = chrono::high_resolution_clock::now();
                tmp.sort();
                auto end = chrono::high_resolution_clock::now();
                cube[1][i][s] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            } else { // set: not applicable
                cube[1][i][s] = -1;
            }
        }
    }

    // INSERT operations: copy master container each sim, then insert in middle.
    for (int i = 0; i < STRUCTURES; i++) {
        for (int s = 0; s < SIMS; s++) {
            if (i == 0) { // vector
                vector<string> tmp = data_vector;
                size_t ind_v = tmp.size() / 2;
                auto start = chrono::high_resolution_clock::now();
                tmp.insert(tmp.begin() + ind_v, "TESTCODE");
                auto end = chrono::high_resolution_clock::now();
                cube[2][i][s] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            } else if (i == 1) { // list
                list<string> tmp(data_list.begin(), data_list.end());
                auto it = tmp.begin();
                advance(it, tmp.size() / 2);
                auto start = chrono::high_resolution_clock::now();
                tmp.insert(it, "TESTCODE");
                auto end = chrono::high_resolution_clock::now();
                cube[2][i][s] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            } else { // set
                set<string> tmp = data_set;
                auto start = chrono::high_resolution_clock::now();
                tmp.insert("TESTCODE");
                auto end = chrono::high_resolution_clock::now();
                cube[2][i][s] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            }
        }
    }

    // DELETE operations: copy master container each sim, then delete the middle element/value.
    for (int i = 0; i < STRUCTURES; i++) {
        for (int s = 0; s < SIMS; s++) {
            if (i == 0) { // vector
                vector<string> tmp = data_vector;
                size_t ind = tmp.size() / 2;
                string target_v = tmp[ind];
                auto start = chrono::high_resolution_clock::now();
                tmp.erase(remove(tmp.begin(), tmp.end(), target_v), tmp.end());
                auto end = chrono::high_resolution_clock::now();
                cube[3][i][s] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            } else if (i == 1) { // list
                list<string> tmp(data_list.begin(), data_list.end());
                auto it1 = tmp.begin();
                advance(it1, tmp.size() / 2);
                string target_l = *it1;
                auto start = chrono::high_resolution_clock::now();
                tmp.remove(target_l);
                auto end = chrono::high_resolution_clock::now();
                cube[3][i][s] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            } else { // set
                set<string> tmp = data_set;
                auto it2 = tmp.begin();
                advance(it2, tmp.size() / 2);
                string target_s = *it2;
                auto start = chrono::high_resolution_clock::now();
                tmp.erase(target_s);
                auto end = chrono::high_resolution_clock::now();
                cube[3][i][s] = chrono::duration_cast<chrono::microseconds>(end - start).count();
            }
        }
    }

    string labels[] = {"Read", "Sort", "Insert", "Delete"};
    cout << setw(W1) << "Operation" << setw(W1) << "Vector" << setw(W1) << "List"
         << setw(W1) << "Set" << endl;
    // Print average across SIMS runs for each op/structure
    for (int op = 0; op < OPS; op++) {
        cout << setw(W1) << labels[op];
        for (int st = 0; st < STRUCTURES; st++) {
            long long sum = 0;
            int count = 0;
            for (int s = 0; s < SIMS; s++) {
                long long v = cube[op][st][s];
                if (v >= 0) { sum += v; ++count; }
            }
            if (count == 0) cout << setw(W1) << "N/A";
            else cout << setw(W1) << (sum / count);
        }
        cout << endl;
    }

    // Nominal sample output for testing: show a few raw cube entries
    cout << "\nSample raw timings (microseconds):\n";
    cout << "cube[Read][Vector][0] = " << cube[0][0][0] << "\n";
    cout << "cube[Sort][Vector][0] = " << cube[1][0][0] << "\n";
    cout << "cube[Insert][List][0] = " << cube[2][1][0] << "\n";
    

    return 0;
}