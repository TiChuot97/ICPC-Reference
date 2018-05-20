// Tested with: http://poj.org/problem?id=1741

void build_tree(int u, vector < int > &node) {
    node.push_back(u);
    child[u] = 1;
    for (int i = 0; i < adj[u].size(); ++i) {
        int v = adj[u][i].first;
        int w = adj[u][i].second;
        if (v == parent[u]) continue;
        parent[v] = u;
        d[v] = d[u] + w;
        build_tree(v, node);
        child[u] += child[v];
    }
}

void build(int root) {
    node_list.clear();
    parent[root] = 0;
    d[root] = 0;
    build_tree(root, node_list);
}

bool is_root(int u, int s) {
    for (int i = 0; i < adj[u].size(); ++i) {
        int v = adj[u][i].first;
        if (parent[u] == v) continue;
        if (child[v] > (s / 2)) return false;
    }
    return ((s - child[u]) <= (s / 2));
}

int find_root(int u) {
    build(u);
    for (int i = 0; i < node_list.size(); ++i)
        if (is_root(node_list[i], node_list.size())) {
            int root = node_list[i];
            build(root);
            return root;
        }
    return -1;
}

void delete_adj(int u, int v) {
    for (int i = 0; i < adj[u].size(); ++i)
        if (adj[u][i].first == v) {
            swap(adj[u][i], adj[u][adj[u].size() - 1]);
            adj[u].pop_back();
            break;
        }
}

int centroid_decomposition(int u) {
    int root = find_root(u);

    for (auto v : node_list)
        dist[root][v] = d[v];

    // Do stuffs here...

    for (int i = 0; i < adj[root].size(); ++i) {
        int v = adj[root][i].first;
        delete_adj(v, root);
        int child = centroid_decomposition(v);
        centroid_parent[child] = root;
    }

    return root;
}
