// World Finals 2015, C

bool spfa()
{
    int h, t, x, y;
    rep(i, T) dis[i] = inf, at[i] = 0;
    q[t = 1] = S; dis[S] = 0; at[S] = 1;
    h = 0;
    while (h != t)
    {
        ++h; if (h > 400) h = 1;
        x = q[h];
        foredge(i, x) if (e[i].c > 0)
        {
            y = e[i].a;
            if (dis[y] > dis[x] + e[i].v)
            {
                dis[y] = dis[x] + e[i].v;
                pre[y] = i;
                if (!at[y])
                {
                    ++t; if (t > 400) t = 1;
                    q[t] = y; at[y] = 1;
                }
            }
        }
        at[x] = 0;
    }
    return dis[T] != inf;
}

int main()
{
    int ans = 0;
    while (spfa())
    {
        ans += dis[T];
        for (int x = T; x; x = e[pre[x] ^ 1].a)
        {
            e[pre[x]].c--; e[pre[x] ^ 1].c++;
        }
    }
    return 0;
}
