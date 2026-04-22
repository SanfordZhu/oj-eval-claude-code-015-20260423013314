#include <bits/stdc++.h>
using namespace std;

static const int BUCKETS = 20; // within 20-file limit
static inline int bucket_of(const string &key){
    size_t h = std::hash<string>{}(key);
    return int(h % BUCKETS);
}
static inline string bucket_path(int b){
    char buf[32];
    snprintf(buf, sizeof(buf), "kv_bucket_%02d.dat", b);
    return string(buf);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; if(!(cin >> n)) return 0; string cmd;
    for(int i=0;i<n;++i){
        if(!(cin >> cmd)) break;
        if(cmd=="insert"){
            string idx; long long vll; cin >> idx >> vll; int val = (int)vll;
            int b = bucket_of(idx); string path = bucket_path(b);
            ofstream fout(path, ios::out | ios::app);
            fout << idx << ' ' << val << '\n';
        }else if(cmd=="delete"){
            string idx; long long vll; cin >> idx >> vll; int val = (int)vll;
            int b = bucket_of(idx); string path = bucket_path(b);
            vector<pair<string,int>> lines;
            ifstream fin(path, ios::in);
            if(fin.good()){
                string key; int v;
                while(fin >> key >> v){ if(!(key==idx && v==val)) lines.emplace_back(key, v); }
            }
            ofstream fout(path, ios::out | ios::trunc);
            for(auto &kv: lines) fout << kv.first << ' ' << kv.second << '\n';
        }else if(cmd=="find"){
            string idx; cin >> idx; int b = bucket_of(idx); string path = bucket_path(b);
            vector<int> vals; vals.reserve(64);
            ifstream fin(path, ios::in);
            if(fin.good()){
                string key; int v;
                while(fin >> key >> v){ if(key==idx) vals.push_back(v); }
            }
            if(vals.empty()){
                cout << "null\n";
            }else{
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
