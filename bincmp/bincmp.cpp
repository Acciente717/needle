#include <cstdio>
#include <vector>
#include <cstring>
#include <string>
#include <queue>
#include <cmath>
using namespace std;

static const int LENGTH = 30;
static const int MAX = 200;
static const double INF = 1e9;
static const double EPS = 1e-8;


int lf = 0, rf = 0, vertice = 0, map[MAX][MAX] = {};
double argA = 0, argB = 0, res = 0, cost[MAX][MAX] = {};

vector< vector<string> > left, right;

void read_in()
{
    char buffer[LENGTH] = {};
    scanf("%lf%lf", &argA, &argB);
    
    while(true)
    {
        memset(buffer, 0, sizeof(char) * LENGTH);
        scanf("%s", buffer);
        
        if(buffer[0] == '#') break;
        else if(buffer[0] == '!') left.push_back(vector<string>());
        else if(buffer[0] == '~') continue;
        else left.back().push_back(string(buffer));
    }
    
    while(true)
    {
        memset(buffer, 0, sizeof(char) * LENGTH);
        scanf("%s", buffer);
        
        if(buffer[0] == '#') break;
        else if(buffer[0] == '!') right.push_back(vector<string>());
        else if(buffer[0] == '~') continue;
        else right.back().push_back(string(buffer));
    }
    
    lf = (int) left.size(), rf = (int) right.size();
    vertice = lf + rf + 2;
}

int lcs(vector<string> &A, vector<string> &B)
{
    static int dp[2][MAX] = {};
    
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
    static double dest[MAX] = {};
    static bool used[MAX] = {};
    static int preffix[MAX] = {};
    
    memset(used, 0, sizeof(used));
    memset(preffix, -1, sizeof(preffix));
    for(int i = 0; i < vertice; i++) dest[i] = INF;
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
    }
    
    return true;
}


int main()
{
    read_in();
    if(vertice >= MAX) return -1;
    build();
    
    while(true)
    {
        bool flag = SPFA();
        if(!flag) break;
    }
    
    printf("%lf\n", res);
    
    return 0;
}








