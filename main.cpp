#include <bits/stdc++.h>
using namespace std;

static const string DB_FILE = "db.log";

// Trim newline and carriage return
static inline void rstrip(string &s){
    while(!s.empty() && (s.back()=='\n' || s.back()=='\r')) s.pop_back();
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Ensure db file exists
    {
        ifstream fin(DB_FILE, ios::in);
        if(!fin.good()){
            ofstream fout(DB_FILE, ios::out | ios::app);
        }
    }

    int n;
    if(!(cin >> n)) return 0;
    string cmd;
    for(int i=0;i<n;++i){
        if(!(cin >> cmd)) break;
        if(cmd=="insert"){
            string idx; long long val; // value fits in int, read as long long then cast
            cin >> idx >> val;
            // append to log: I index value\n
            ofstream fout(DB_FILE, ios::out | ios::app);
            fout << 'I' << ' ' << idx << ' ' << (int)val << '\n';
        }else if(cmd=="delete"){
            string idx; long long val;
            cin >> idx >> val;
            ofstream fout(DB_FILE, ios::out | ios::app);
            fout << 'D' << ' ' << idx << ' ' << (int)val << '\n';
        }else if(cmd=="find"){
            string idx; cin >> idx;
            // Scan log and compute values set for this key
            // Use unordered_set to avoid duplicates, then sort before output
            unordered_set<int> present;
            // We'll simulate inserts/deletes: start empty, apply operations for this idx
            ifstream fin(DB_FILE, ios::in);
            string op, key; int value;
            while(fin >> op >> key){
                if(!(fin >> value)) break;
                if(key==idx){
                    if(op=="I"){
                        present.insert(value);
                    }else if(op=="D"){
                        auto it = present.find(value);
                        if(it!=present.end()) present.erase(it);
                        // If value not currently present, deletion keeps it absent
                    }
                }
            }
            if(present.empty()){
                cout << "null\n";
            }else{
                vector<int> vals; vals.reserve(present.size());
                for(int v: present) vals.push_back(v);
                sort(vals.begin(), vals.end());
                for(size_t k=0;k<vals.size();++k){
                    if(k) cout << ' ';
                    cout << vals[k];
                }
                cout << '\n';
            }
        }else{
            // Unknown command, ignore safely
            string restOfLine;
            getline(cin, restOfLine);
        }
    }
    return 0;
}
