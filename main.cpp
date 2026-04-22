#include <bits/stdc++.h>
using namespace std;

static const int BUCKETS = 20; // keep within 20-file limit
static inline int bucket_of(const string &key){ size_t h = std::hash<string>{}(key); return int(h % BUCKETS); }
static inline string bucket_path(int b){ char buf[32]; snprintf(buf, sizeof(buf), "kv_bucket_%02d.dat", b); return string(buf); }
static inline string tmp_path(const string &path){ return path + ".tmp"; }

static void write_line(ofstream &out, const string &key, const vector<int> &vals){
    if(vals.empty()) return; // remove empty key on delete
    out << key;
    for(int v: vals) out << ' ' << v;
    out << '\n';
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; if(!(cin >> n)) return 0; string cmd;
    for(int i=0;i<n;++i){
        if(!(cin >> cmd)) break;
        if(cmd=="insert"){
            string idx; long long vll; cin >> idx >> vll; int val = (int)vll;
            int b = bucket_of(idx); string path = bucket_path(b); string tpath = tmp_path(path);
            ifstream fin(path, ios::in);
            ofstream fout(tpath, ios::out | ios::trunc);
            bool found = false;
            string line;
            while(true){
                string key; if(!(fin >> key)) break;
                vector<int> vals; int v;
                // read rest of line values until newline by peeking next char
                while(fin.peek()==' '){ fin.get(); if(!(fin >> v)) break; vals.push_back(v); }
                if(fin.peek()=='\n') fin.get();
                if(key==idx){
                    found = true;
                    vals.push_back(val);
                    sort(vals.begin(), vals.end());
                    vals.erase(unique(vals.begin(), vals.end()), vals.end());
                    write_line(fout, key, vals);
                }else{
                    write_line(fout, key, vals);
                }
            }
            if(!found){
                fout << idx << ' ' << val << '\n';
            }
            fin.close(); fout.close();
            // replace original
            std::remove(path.c_str());
            std::rename(tpath.c_str(), path.c_str());
        }else if(cmd=="delete"){
            string idx; long long vll; cin >> idx >> vll; int val = (int)vll;
            int b = bucket_of(idx); string path = bucket_path(b); string tpath = tmp_path(path);
            ifstream fin(path, ios::in);
            ofstream fout(tpath, ios::out | ios::trunc);
            bool found = false;
            string key; int v;
            while(fin >> key){
                vector<int> vals;
                while(fin.peek()==' '){ fin.get(); if(!(fin >> v)) break; vals.push_back(v); }
                if(fin.peek()=='\n') fin.get();
                if(key==idx){
                    found = true;
                    sort(vals.begin(), vals.end());
                    vals.erase(remove(vals.begin(), vals.end(), val), vals.end());
                    write_line(fout, key, vals);
                }else{
                    write_line(fout, key, vals);
                }
            }
            fin.close(); fout.close();
            std::remove(path.c_str());
            std::rename(tpath.c_str(), path.c_str());
        }else if(cmd=="find"){
            string idx; cin >> idx; int b = bucket_of(idx); string path = bucket_path(b);
            ifstream fin(path, ios::in);
            string key; int v; vector<int> vals;
            while(fin >> key){
                vector<int> tmp;
                while(fin.peek()==' '){ fin.get(); if(!(fin >> v)) break; tmp.push_back(v); }
                if(fin.peek()=='\n') fin.get();
                if(key==idx){ vals.swap(tmp); break; }
            }
            if(vals.empty()){
                cout << "null\n";
            }else{
                // already sorted and unique by construction, but ensure
                sort(vals.begin(), vals.end());
                vals.erase(unique(vals.begin(), vals.end()), vals.end());
                for(size_t k=0;k<vals.size();++k){ if(k) cout << ' '; cout << vals[k]; }
                cout << '\n';
            }
        }else{
            string rest; getline(cin, rest);
        }
    }
    return 0;
}
