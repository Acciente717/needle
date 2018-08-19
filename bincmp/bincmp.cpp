#include <cstdio>
#include <vector>
#include <cstring>
#include <string>
#include <queue>
#include <cmath>
using namespace std;

static const int LENGTH = 100;
static const int INSTRUCT = 200000;
static const double INF = 1e9;
static const double EPS = 1e-8;


static int lf = 0, rf = 0, vertice = 0, **map = NULL, *_map = NULL;
static double argA = 0, argB = 0, res = 0, **cost = NULL, *_cost = NULL;

static double *dest = NULL;
static bool *used = NULL;
static int *preffix = NULL;

static vector< vector<string> > left, right;

void read_in()
{
    char buffer[LENGTH] = {};
    scanf("%lf%lf", &argA, &argB);
    
    while(true)
    {
        scanf("%s", buffer);
//        printf("%s\n", buffer);
        
        if(buffer[0] == '#') break;
        else if(buffer[0] == '!') left.push_back(vector<string>());
        else if(buffer[0] == '~') continue;
        else left.back().push_back(string(buffer));
        
    }
    
    while(true)
    {
        scanf("%s", buffer);
        
        if(buffer[0] == '#') break;
        else if(buffer[0] == '!') right.push_back(vector<string>());
        else if(buffer[0] == '~') continue;
        else right.back().push_back(string(buffer));
    }
    
    lf = (int) left.size(), rf = (int) right.size();
    vertice = lf + rf + 5;
//    printf("%d\n", vertice);
    
    _map = new int[vertice * vertice];
    
    memset(_map, 0, sizeof(int) * vertice * vertice);
    map = new int *[vertice];
    for(int i = 0; i < vertice; i++) map[i] = &_map[i * vertice];
    
    _cost = new double[vertice * vertice];
    
    memset(_cost, 0, sizeof(double) * vertice * vertice);
    cost = new double *[vertice];
    for(int i = 0; i < vertice; i++) cost[i] = &_cost[i * vertice];
    
    dest = new double[vertice];
    used = new bool[vertice];
    preffix = new int[vertice];

    
}

int lcs(vector<string> &A, vector<string> &B)
{
    static int dp[2][INSTRUCT] = {};
    
    int ptr = 0, lsize = (int)A.size(), rsize = (int)B.size();
    memset(dp[0], 0, sizeof(dp[0]));
    
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

void build()
{
    for(int i = 0; i < lf; i++) map[0][i + 2] = (int)left[i].size(), cost[0][i + 2] = 0;
    for(int i = 0; i < rf; i++) map[i + 2 + lf][1] = (int)right[i].size(), cost[i + 2 + lf][1] = 0;
    
    for(int i = 0; i < lf; i++)
    {
        for(int j = 0; j < rf; j++)
        {
            int len = lcs(left[i], right[j]);
            double weight = 1.0 / (1.0 + exp((-argA) * len / min(left[i].size(), right[j].size()) + argB));
            
            map[i + 2][j + 2 + lf] = len;
            cost[i + 2][j + 2 + lf] = -weight, cost[j + 2 + lf][i + 2] = weight;
        }
    }
}

bool SPFA()
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
//    printf("res : %lf\n", res);
    
    while(last != 0)
    {
        map[preffix[last]][last] -= MIN;
        map[last][preffix[last]] += MIN;
        last = preffix[last];
    }
    
    return true;
}

void destroy()
{
    delete[] map;
    delete[] _map;
    delete[] cost;
    delete[] _cost;
    delete[] dest;
    delete[] used;
    delete[] preffix;
    
}

int main()
{
//    freopen("/Users/chentianyu/Documents/ICS/needle-master/test_data", "r", stdin);
    read_in();
    build();
    
    while(true)
    {
        bool flag = SPFA();
        if(!flag) break;
    }
    
    int total = 0;
    for(int i = 0; i < left.size(); i++) total += left[i].size();
    
    printf("%lf\n", -res / total);
    destroy();
    
    return 0;
}
