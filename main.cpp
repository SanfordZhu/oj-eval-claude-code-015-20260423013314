#include <bits/stdc++.h>
using namespace std;

static const int BUCKETS = 16;
static inline int bucket_of(const string &key){ size_t h = std::hash<string>{}(key); return int(h & (BUCKETS - 1)); }
static inline string bucket_path(int b){ char buf[32]; snprintf(buf, sizeof(buf), "kv_bucket_%02d.dat", b); return string(buf); }

static array<bool, BUCKETS> cacheValid{};
static array<unordered_map<string, vector<int>>, BUCKETS> cache;

static void load_bucket(int b){
    if(cacheValid[b]) return;
    cache[b].clear();
    string path = bucket_path(b);
    ifstream fin(path, ios::in);
    if(fin.good()){
        string key; int v;
        while(fin >> key >> v){ cache[b][key].push_back(v); }
    }
    for(auto &kv : cache[b]){
        auto &vec = kv.second;
        sort(vec.begin(), vec.end());
        vec.erase(unique(vec.begin(), vec.end()), vec.end());
    }
    cacheValid[b] = true;
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
            cacheValid[b] = false;
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
            cacheValid[b] = false;
        }else if(cmd=="find"){
            string idx; cin >> idx; int b = bucket_of(idx);
            load_bucket(b);
            auto it = cache[b].find(idx);
            if(it==cache[b].end() || it->second.empty()){
                cout << "null\n";
            }else{
                const auto &vals = it->second;
                for(size_t k=0;k<vals.size();++k){ if(k) cout << ' '; cout << vals[k]; }
                cout << '\n';
            }
        }else{
            string rest; getline(cin, rest);
        }
    }
    return 0;
}
