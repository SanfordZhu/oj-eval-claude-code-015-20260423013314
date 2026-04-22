#include <bits/stdc++.h>
using namespace std;

static const int BUCKETS = 20; // within 20-file limit
static inline int bucket_of(const string &key){ size_t h = std::hash<string>{}(key); return int(h % BUCKETS); }
static inline string bucket_path(int b){ char buf[32]; snprintf(buf, sizeof(buf), "kv_bucket_%02d.dat", b); return string(buf); }

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
            fout << 'I' << ' ' << idx << ' ' << val << '\n';
        }else if(cmd=="delete"){
            string idx; long long vll; cin >> idx >> vll; int val = (int)vll;
            int b = bucket_of(idx); string path = bucket_path(b);
            ofstream fout(path, ios::out | ios::app);
            fout << 'D' << ' ' << idx << ' ' << val << '\n';
        }else if(cmd=="find"){
            string idx; cin >> idx; int b = bucket_of(idx); string path = bucket_path(b);
            unordered_set<int> present;
            ifstream fin(path, ios::in);
            string op, key; int v;
            while(fin >> op >> key){
                if(!(fin >> v)) break;
                if(key==idx){
                    if(op=="I") present.insert(v);
                    else if(op=="D"){
                        auto it = present.find(v);
                        if(it!=present.end()) present.erase(it);
                    }
                }
            }
            if(present.empty()){
                cout << "null\n";
            }else{
                vector<int> vals; vals.reserve(present.size());
                for(int x: present) vals.push_back(x);
                sort(vals.begin(), vals.end());
                for(size_t k=0;k<vals.size();++k){ if(k) cout << ' '; cout << vals[k]; }
                cout << '\n';
            }
        }else{
            string rest; getline(cin, rest);
        }
    }
    return 0;
}
