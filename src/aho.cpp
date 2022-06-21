#include <bits/stdc++.h>
 
using namespace std;
 
const int al_size = 26;
vector<bool> res;
 
class Bor {
private:
    class Node {
    public:
        vector<int> kids = vector<int>(al_size, -1);
        vector<int> go = vector<int>(al_size, -1);
        int parent = -1;
        int sufflink = -1;
        int up = -1;
        int charToParent = -1;
        bool isLeaf = false;
        vector<int> leafPatternNumber;
 
        int number;
 
        explicit Node(int p = -1, int letter = -1, bool state = false, int num = 0) : parent(p), charToParent(letter),
                                                                                      isLeaf(state), number(num) {}
 
        Node(const Node &other) = default;
 
        bool IsRoot() const {
            if (parent == -1)
                return true;
            return false;
        }
    };
 
public:
    Bor() {
        bor.emplace_back();
        ++count;
    }
 
    void addString(const string &s, int patternNumber) {
        int cur = 0;
        for (auto &c: s) {
            if (bor[cur].kids[c - 'a'] == -1) {
                bor.emplace_back(cur, c - 'a', false, count);
                bor[cur].kids[c - 'a'] = count++;
            }
            cur = bor[cur].kids[c - 'a'];
        }
        bor[cur].isLeaf = true;
        bor[cur].leafPatternNumber.push_back(patternNumber);
    }
 
    int getLink(int v, int c) {
        if (bor[v].go[c] == -1) {
            if (bor[v].kids[c] != -1) {
                bor[v].go[c] = bor[v].kids[c];
            } else if (bor[v].IsRoot()) {
                bor[v].go[c] = 0;
            } else {
                bor[v].go[c] = getLink(getSuffLink(v), c);
            }
        }
        return bor[v].go[c];
    }
 
    int getSuffLink(int v) {
        if (bor[v].sufflink == -1) {
            if (bor[v].IsRoot() || bor[v].parent == 0) {
                bor[v].sufflink = 0;
            } else {
                bor[v].sufflink = getLink(getSuffLink(bor[v].parent), bor[v].charToParent);
            }
        }
        return bor[v].sufflink;
    }
 
    int getUp(int v) {
        if (bor[v].up == -1) {
            if (bor[getSuffLink(v)].isLeaf) {
                bor[v].up = getSuffLink(v);
            } else if (getSuffLink(v) == 0) {
                bor[v].up = 0;
            } else {
                bor[v].up = getUp(getSuffLink(v));
            }
        }
        return bor[v].up;
    }
 
    void bfs() {
        queue<int> q;
        q.push(0);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int &x: bor[v].kids) {
                if (x != -1)
                    q.push(x);
                getUp(x);
            }
        }
    }
 
    void processText(const string &t) {
        int cur = 0;
        for (auto &c: t) {
            if (bor[cur].kids[c - 'a'] > -1){
                cur = bor[cur].kids[c - 'a'];
            }
            else{
                int temp = cur;
                if (bor[cur].go[c - 'a'] > -1){
                    cur = bor[cur].go[c - 'a'];
                }
                else{
                    while (cur > 0 && bor[cur].kids[c - 'a'] == -1){
                        cur = bor[cur].sufflink;
                    }
                    if (bor[cur].kids[c - 'a'] != -1){
                        bor[temp].go[c - 'a'] = bor[cur].kids[c - 'a'];
                        cur = bor[cur].kids[c - 'a'];
                    }
                    else{
                        bor[temp].go[c - 'a'] = 0;
                        cur = 0;
                    }
                }
            }
 
            if (bor[cur].isLeaf) {
                for (int i: bor[cur].leafPatternNumber) {
                    res[i] = true;
                }
                bor[cur].isLeaf = false;
            }
 
            int comp_link = cur;
            while (comp_link > 0 && bor[comp_link].up != -1) {
                int tmp_link = comp_link;
                comp_link = bor[comp_link].up;
                bor[tmp_link].up = -1;
                for (int i: bor[comp_link].leafPatternNumber) {
                    res[i] = true;
                }
            }
        }
    }
 
private:
    int count = 0;
    vector<Node> bor;
};
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    //ifstream cin ("test1.txt");
    int n;
    cin >> n;
    res.resize(n);
    Bor solution;
    for (int i = 0; i < n; ++i) {
        string tmp;
        cin >> tmp;
        solution.addString(tmp, i);
    }
    solution.bfs();
    string t;
    cin >> t;
    solution.processText(t);
 
    for (bool flag: res) {
        if (flag) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
 
    return 0;
}