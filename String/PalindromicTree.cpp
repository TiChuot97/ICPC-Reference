#include <iostream>
#include <vector>
#define LL long long

using namespace std;

// Palindromic Tree
// Author: Gary
// Tested on SPOJ NUMOFPAL

// LSP: largest suffix-palindrome
// a string of length n has at most n different palindrome substrs

const int NMAX = 1000005;

struct Node {
    Node* next[26];  // to palindrome by extending me with a letter
    Node* sufflink;  // my LSP
    int len;  // length of this palindrome substring
    int num;  // number of palindrome substrs ending here
};
Node nodes[NMAX];
int n = 0;  // number of nodes in tree
vector<int> s;
LL ans = 0;

void build_tree () {
    nodes[0].len = -1; nodes[0].sufflink = &nodes[0];  // root 0
    nodes[1].len = 0; nodes[1].sufflink = &nodes[0];  // root 1
    n = 2;
    Node* suff = &nodes[1];  // node for LSP of processed prefix
    for (int i = 0; i < s.size(); i++) {
        // find LSP xAx
        Node* ptr = suff;
        while (1) {
            int j = i - 1 - ptr->len;
            if (j >= 0 && s[j] == s[i]) break;
            ptr = ptr->sufflink;
        }

        if (ptr->next[s[i]]) {  // palindrome substr already exists
            suff = ptr->next[s[i]];
        } else {  // add a new node
            suff = &nodes[n++];
            suff->len = ptr->len + 2;
            ptr->next[s[i]] = suff;
            if (suff->len == 1) {  // current LSP is trivial
                suff->sufflink = &nodes[1];
                suff->num = 1;
            } else {
                // find xAx's LSP xBx
                while (1) {
                    ptr = ptr->sufflink;
                    int j = i - 1 - ptr->len;
                    if (j >= 0 && s[j] == s[i]) break;
                }
                suff->sufflink = ptr->next[s[i]];
                suff->num = suff->sufflink->num + 1;
            }
        }
        ans += suff->num;
    }
}

int main () {
    string str; cin >> str;
    s.assign(str.length(), 0);
    for (int i = 0; i < str.length(); i++) s[i] = str[i];
    build_tree();
    cout << ans << endl;

    return 0;
}

