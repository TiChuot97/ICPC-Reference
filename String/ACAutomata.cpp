#include <iostream>
#include <cstdio>
#include <queue>
#include <set>

using namespace std;

// AC Automata
// Author: Gary
// Tested on HDU 2222

struct Node {
	Node* next[26];
	Node* fail;
	int cnt;
	Node (Node* root) {
		memset(next, NULL, sizeof(next));
		fail = root;
		cnt = 0;
	}
};
Node* root;

void insert (string s) {
	Node* curr = root;
	for (int i = 0; i < s.length(); i++) {
		int j = s[i] - 'a';
		if (curr->next[j] == NULL) {
			curr->next[j] = new Node(root);
		}
		curr = curr->next[j];
	}
	curr->cnt++;
}

void make_fail () {
	queue<Node*> q;
	for (int i = 0; i < 26; i++) {
		if (root->next[i]) {
			q.push(root->next[i]);
		}
	}
	while (!q.empty()) {
		Node* node = q.front(); q.pop();
		for (int i = 0; i < 26; i++) {
			if (node->next[i]) {
				q.push(node->next[i]);
				Node* f = node->fail;
				while (f != root && !f->next[i]) {
					f = f->fail;
				}
				if (f->next[i]) {
					f = f->next[i];
				}
				node->next[i]->fail = f;
			}
		}
	}
}

int work (string s) {
	set<Node*> seen;
	int cnt = 0;
	Node* curr = root;
	for (int i = 0; i < s.length(); i++) {
		int j = s[i] - 'a';
		while (curr != root && !curr->next[j]) {
			curr = curr->fail;
		}
		if (curr->next[j]) {
			curr = curr->next[j];
			Node* p = curr;
			while (p != root) {
				if (seen.find(p) != seen.end()) break;
				seen.insert(p);
				cnt += p->cnt;
				p = p->fail;
			}
		}
	}
	return cnt;
}

int main () {
	int t; scanf("%d", &t);
	while (t--) {
		int n; scanf("%d", &n);
		root = new Node(NULL);
		for (int i = 0; i < n; i++) {
			char s[100];
			scanf("%s", s);
			insert(string(s));
		}
		make_fail();
		char s[1000001];
		scanf("%s", s);
		cout << work(string(s)) << endl;
	}

	return 0;
}

