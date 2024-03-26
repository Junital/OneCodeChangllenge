/***********************************************
 * 题目：树上搜索
 * 描述：给一棵树，要求根据权重的差值最小作为查询，
        如果查错了，则剔除后代，否则保留后代，直到查出为止。
 * 思路：模拟。
 * 对象：QueryTree模拟查询树。
 * 注意：无。
 ***********************************************/

#include <bits/stdc++.h>

using namespace std;

#define rep(i, s, k) for (int i = s; i <= k; i++) // 定义快速循环
#define ll long long                              // 定义长整型

class QueryTree
{
private:
    int Num;                 // 结点数
    vector<ll> Weighs;       // 权重
    vector<int> Father;      // 父亲
    vector<vector<int>> Son; // 子节点
    ll WeighSum;             // 权重之和

    const ll MaxNum = 20000000010; // 最大值

    void FatherToSon()
    {
        Son.resize(Num + 1);

        rep(i, 2, Num)
        {
            Son[Father[i]].push_back(i);
        }
    }

    bool isFather(int father, int son)
    {
        int f = Father[son];
        while (f > 0)
        {
            if (f == father)
            {
                return true;
            }

            f = Father[f];
        }

        return false;
    }

public:
    /* 初始化，输入结点数 */
    QueryTree(int num) : Num(num)
    {
        Weighs.resize(Num + 1);
        Father.resize(Num + 1);
        WeighSum = 0;

        rep(i, 1, Num)
        {
            cin >> Weighs[i];
            WeighSum += Weighs[i];
        }

        rep(i, 2, Num)
        {
            cin >> Father[i];
        }

        FatherToSon();
    }

    /* 询问，输入询问的次数。 */
    void Query(int Times)
    {
        rep(i, 1, Times)
        {
            int Ans;
            cin >> Ans;

            vector<bool> Wait(Num + 1, true);
            ll ws = WeighSum;

            while (1)
            {
                ll MinWeight = MaxNum;
                int MinIdx = -1;
                vector<ll> W(Num + 1, 0);
                rep(i, 1, Num)
                {
                    if (Wait[i])
                    {
                        stack<int> sons; // 存储子节点编号
                        sons.push(i);

                        while (!sons.empty())
                        {
                            int idx = sons.top();
                            sons.pop();

                            W[i] += Weighs[idx];

                            if (Son[idx].size() > 0)
                            {
                                for (auto j : Son[idx])
                                {
                                    if (Wait[j])
                                    {
                                        sons.push(j);
                                    }
                                }
                            }
                        }

                        ll w = abs(ws - W[i] - W[i]);

                        if (w < MinWeight)
                        {
                            MinIdx = i;
                            MinWeight = w;
                        }
                    }
                }

                cout << MinIdx << " ";

                if (isFather(MinIdx, Ans) || MinIdx == Ans)
                {
                    /* 把自己和子节点保留，其余都删去 */
                    set<int> live;
                    stack<int> q;
                    live.insert(MinIdx);
                    q.push(MinIdx);

                    while (!q.empty())
                    {
                        int idx = q.top();
                        q.pop();

                        if (Son[idx].size() > 0)
                        {
                            for (auto j : Son[idx])
                            {
                                if (Wait[j])
                                {
                                    q.push(j);
                                    live.insert(j);
                                }
                            }
                        }
                    }

                    rep(i, 1, Num)
                    {
                        if (Wait[i] && live.count(i) == 0)
                        {
                            ws -= Weighs[i];
                            Wait[i] = false;
                        }
                    }
                }
                else
                {
                    /* 把自己和子节点都删去 */
                    stack<int> perish;
                    perish.push(MinIdx);

                    while (!perish.empty())
                    {
                        int idx = perish.top();
                        perish.pop();
                        Wait[idx] = false;
                        ws -= Weighs[idx];

                        if (Son[idx].size() > 0)
                        {
                            for (auto i : Son[idx])
                            {
                                perish.push(i);
                            }
                        }
                    }
                }

                int WaitNum = 0;
                rep(j, 1, Num)
                {
                    if (Wait[j])
                    {
                        WaitNum++;
                    }
                }

                if (WaitNum == 1)
                {
                    cout << endl;
                    break;
                }
            }
        }
    }
};

int main()
{
    int Num, QueryNum;

    cin >> Num >> QueryNum;

    QueryTree qt(Num);

    qt.Query(QueryNum);

    return 0;
}