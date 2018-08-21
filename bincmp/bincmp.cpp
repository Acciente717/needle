#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
using namespace std;

static const double INF = 1e9;
static const double EPS = 1e-8;

static int lf = 0, rf = 0, vertice = 0;
static double argA = 0, argB = 0, res = 0;

static vector<int> _map;
static vector<decltype(_map.begin())> map;
static vector<double> _cost;
static vector<decltype(_cost.begin())> cost;

static vector<double> dest;
static vector<bool> used;
static vector<int> preffix; 

static vector< vector<string> > leftvec, rightvec;

static void read_in()
{
    string buffer;
    cin >> argA >> argB;
    
    while(true)
    {
        cin >> buffer;
        
        if(buffer[0] == '#') break;
        else if(buffer[0] == '!') leftvec.push_back(vector<string>());
        else if(buffer[0] == '~') continue;
        else leftvec.back().push_back(buffer);
        
    }
    
    while(true)
    {
        cin >> buffer;
        
        if(buffer[0] == '#') break;
        else if(buffer[0] == '!') rightvec.push_back(vector<string>());
        else if(buffer[0] == '~') continue;
        else rightvec.back().push_back(buffer);
    }
    
    lf = static_cast<int>(leftvec.size()), rf = static_cast<int>(rightvec.size());
    vertice = lf + rf + 5;
    
    _map.resize(vertice * vertice);
    fill(_map.begin(), _map.end(), 0);
    map.resize(vertice);
    for(int i = 0; i < vertice; i++) map[i] = _map.begin() + (i * vertice);
    
    _cost.resize(vertice * vertice);
    fill(_cost.begin(), _cost.end(), 0);
    cost.resize(vertice);
    for(int i = 0; i < vertice; i++) cost[i] = _cost.begin() + (i * vertice);

    dest.resize(vertice);
    used.resize(vertice);
    preffix.resize(vertice);
}

static int lcs(vector<string> &A, vector<string> &B)
{
    static vector<int> dp[2];
    
    int ptr = 0, lsize = static_cast<int>(A.size()), rsize = static_cast<int>(B.size());
    if (rsize >= dp[0].size())
        dp[0].resize(rsize + 1), dp[1].resize(rsize + 1);
    fill(dp[0].begin(), dp[0].end(), 0);
    
    for(int i = 0; i < lsize; i++)
    {
        ptr ^= 1;
        dp[ptr][0] = 0;
        for(int j = 1; j <= rsize; j++)
        {
            dp[ptr][j] = max(dp[ptr][j - 1], dp[ptr ^ 1][j]);
            if(A[i] == B[j - 1]) dp[ptr][j] = max(dp[ptr][j], dp[ptr ^ 1][j - 1] + 1);
            else dp[ptr][j] = max(dp[ptr][j], dp[ptr ^ 1][j - 1]);
        }
    }
    return dp[ptr][rsize];
}

static void build()
{
    for(int i = 0; i < lf; i++)
        map[0][i + 2] = static_cast<int>(leftvec[i].size()), cost[0][i + 2] = 0;
    for(int i = 0; i < rf; i++)
        map[i + 2 + lf][1] = static_cast<int>(rightvec[i].size()), cost[i + 2 + lf][1] = 0;
    
    for(int i = 0; i < lf; i++)
    {
        for(int j = 0; j < rf; j++)
        {
            int len = lcs(leftvec[i], rightvec[j]);
            double weight = 1.0 / (1.0 + exp((-argA) * len / min(leftvec[i].size(), rightvec[j].size()) + argB));
            
            map[i + 2][j + 2 + lf] = len;
            cost[i + 2][j + 2 + lf] = -weight, cost[j + 2 + lf][i + 2] = weight;
        }
    }
}

static bool SPFA()
{
    for(int i = 0; i < vertice; i++)
    {
        dest[i] = INF;
        used[i] = false;
        preffix[i] = -1;
    }
    dest[0] = 0;
    
    queue<int> test;
    test.push(0);
    
    while(!test.empty())
    {
        int last = test.front();
        test.pop();
        used[last] = false;
        
        for(int i = 0; i < vertice; i++)
        {
            if(map[last][i] <= 0 || dest[i] <= dest[last] + cost[last][i] + EPS) continue;
            dest[i] = dest[last] + cost[last][i];
            preffix[i] = last;
            
            if(used[i]) continue;
            used[i] = true;
            test.push(i);
        }
    }
    
    if(dest[1] + EPS >= 0) return false;
    
    int last = 1, MIN = INF;
    while(last != 0)
    {
        MIN = min(MIN, map[preffix[last]][last]);
        last = preffix[last];
    }
    
    last = 1;
    res += MIN * dest[1];
    
    while(last != 0)
    {
        map[preffix[last]][last] -= MIN;
        map[last][preffix[last]] += MIN;
        last = preffix[last];
    }
    
    return true;
}

int main()
{
    read_in();
    build();
    
    while(true)
    {
        bool flag = SPFA();
        if(!flag) break;
    }
    
    int total = 0;
    for(int i = 0; i < leftvec.size(); i++) total += leftvec[i].size();
    
    cout << (-res / total) << endl;
    
    return 0;
}
