#include <iostream>
#include<stack>
#include<vector>
#include <fstream>
using namespace std;
#define NODE_NUM 100
#define EDGE_NUM 400
#define GRAPH_FILE "100_UG_1.txt"
const int MAXV = 1000;//节点个数最大值
const int INF = 10000000;//非常大的数
int G[MAXV][MAXV];//邻接矩阵
bool init[MAXV][MAXV];//初始图记录矩阵，如果初始的图中"u-v"存在，则init[u][v] = init[v][u] = true;
int deg[MAXV] = {0};//存储节点度数的散列表
int N;//节点个数
int path_count = 0;//路径条数
stack<int>S;
int add = 0;//为了构建欧拉图，添加节点个数
int path[MAXV];//最终路径
int path_num = 0;//路径节点数

void DFS(int x)
{
    S.push(x);
    for(int i=0;i<N + add;i++)
    {
        if(G[x][i] > 0)
        {
            G[i][x]=G[x][i]=0;//删除此边  
            DFS(i);
            break;
        }
    }
}


//Fleury算法
void Fleury(int x)
{
    S.push(x);
    while(!S.empty())
    {
        int b = 0;
        for(int i = 0; i < N + add;i++)
        {
            if(G[S.top()][i] > 0)
            {
                b=1;
                break;
            }
        }
        if(b==0)
        {
            path[path_num++] = S.top();
            S.pop();
        }
        else 
        {
            int y=S.top();
            S.pop();
            DFS(y);//如果有，就dfs
        }
    }
}

int make_euler()//生成欧拉图
{
    int start = 1;
    vector<int> node(2);//用于存储两个已发现的奇度数节点
    int count = 0;
    int save = 0;
    for(int i = 0; i < N; i++)
    {
        if(deg[i] % 2 != 0)
        {
            node[count++] = i;

            if(count == 2 && save == 1)
            {
                if(G[node[0]][node[1]] == 0)//如果两节点原来没有边连通
                {
                    G[node[0]][node[1]] = G[node[1]][node[0]] = 1;//将发现的两个奇度数节点连线
                    deg[node[0]]++;
                    deg[node[1]]++;
                }
                else//如果两节点连通,则添加节点
                {
                    G[node[0]][N + add] = G[N + add][node[0]] = 2;
                    G[node[1]][N + add] = G[N + add][node[1]] = 2;
                    add++;
                    deg[node[0]]++;
                    deg[node[1]]++;
                }
                count = 0;//重新计数
            }
            else if(count == 2 && save == 0){
                start = node[1];
                count = 0;
                save = 1;
            }
        }
    }
    return start;
}
float Count(vector<vector<int> > pathList) {	//计算路径个数		路径长度方差
		float avg = 0 , avg_s = 0;
		int L = 0;
		for (int i = 0; i < pathList.size(); i++) {
			L += (pathList[i].size() - 1);
		}
		avg = float(L) / pathList.size();	//当前路径平均值
		for (int i = 0; i < pathList.size(); i++) {
			avg_s += ((pathList[i].size() - 1) - avg)*((pathList[i].size() - 1) - avg);
		}
		float ss = avg_s / pathList.size();	//当前路径方差
        return ss;
}
int main() 
{
    int num_nodes = NODE_NUM;
    int num_edges = EDGE_NUM;
    N = num_nodes;
    //初始化邻接矩阵和初始图记录矩阵 
    ifstream infile;//定义读取文件流，相对于程序来说是in
    infile.open(GRAPH_FILE);//打开文件
    for (int i = 0; i < num_nodes; i++)//定义行循环
    {
        for (int j = 0; j < num_nodes; j++)//定义列循环
        {
            infile >> G[i][j];//读取一个值（空格、制表符、换行隔开）就写入到矩阵中，行列不断循环进行
            if(G[i][j] == 1){
                init[i][j] = init[j][i] = true;
                deg[i]++;
            }
        }
    }
    infile.close();//读取完成之后关闭文件

    int start;
    int odd = 0;
    for(int i = 0; i < num_nodes; i++){
        if(deg[i] % 2 != 0){
            odd++;
        }
    }
    printf("%d\n", odd);
    start = make_euler();
    Fleury(start);
    printf("\n");
    cout << "Result：" << endl;
    vector<vector<int> > pathList;
    vector<int> one_path;
    for(int i = 0; i < path_num - 1; i++)
    {
        if(path[i] < N){
            one_path.push_back(path[i]);
        }
        if(init[path[i]][path[i+1]] == true && path[i+1] < N){
            continue;
        } 
        else{
            if(one_path.size() > 0){
                pathList.push_back(one_path);
                one_path.clear();
            }
        }
    }
    if(path[path_num] < N){
        one_path.push_back(path[path_num]);
    }
    pathList.push_back(one_path);
    for(int i = 0; i < pathList.size(); i++){
        printf("Path:");
        for(int j = 0; j < pathList[i].size(); j++){
            cout<<pathList[i][j] << " ";
        }
        cout << endl;
    }
    cout << "Num: "  << pathList.size() << " ";
    cout << "Var：" << Count(pathList) << endl;
    return 0;
}