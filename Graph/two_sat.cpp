// Tested with: http://www.spoj.com/problems/TORNJEVI/
// Note: for tarjan(), see tarjan.cpp

void topo_sort() {
    for (int i = 1; i <= num_comp; ++i)
        if (deg[i] == 0) q.push(i);
    int num = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int i = 0; i < new_adj[u].size(); ++i) {
            int v = new_adj[u][i];
            --deg[v];
            if (deg[v] == 0) q.push(v);
        }
        position[u] = ++num;
    }
}

bool two_sat() {
    for (int i = 0; i < list_node.size(); ++i)
        if (!num[list_node[i]]) tarjan(list_node[i]);
    for (int i = 0; i < list_node.size(); ++i) {
        int u = list_node[i];
        if (comp[u] == comp[neg[u]]) return false;
        for (int j = 0; j < adj[u].size(); ++j) {
            int v = adj[u][j];
            if (comp[u] == comp[v]) continue;
            new_adj[comp[u]].push_back(comp[v]);
            ++deg[comp[v]];
        }
    }
    topo_sort();
    for (int i = 0; i < list_node.size(); ++i) {
        int u = list_node[i];
        if (position[comp[u]] > position[comp[neg[u]]])
            check[u] = 1; // Pick u (otherwise pick !u)
    }
    return true;
}
