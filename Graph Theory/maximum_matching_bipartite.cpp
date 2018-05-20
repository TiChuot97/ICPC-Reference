// Tested with https://icpcarchive.ecs.baylor.edu/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=6293

bool find_match(int u) {
    if (check[u] == flag)
        return false;
    check[u] = flag;
    FORU(v, 1, n)
        if (c[u][v] && (match[v] == 0 || find_match(match[v]))) {
            match[v] = u;
            return true;
        }
    return false;
}

void max_matching() {
    FORU(i, 1, m) {
        ++flag;
        if (find_match(i)) ++res;
    }
}
