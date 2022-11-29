#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <time.h>
#include <math.h>
#include <fstream>
#include <algorithm>
using namespace std;

#define maxvertexnum 90 /*最大顶点数设为100*/
#define EDGE_NUM 360
#define infinity 2147483647 /*当两点不连通时，则它们之间距离为最大值*/
#define writeDataPath "90_UG_1.txt"
typedef int vertextype;/*顶点的类型为整型*/
typedef int edgetype;/*边的权重为整形*/
 
enum GraphType{DG,UG};/*有向图，无向图*/
struct MGraph//邻接矩阵表示法的结构和类型声明
{
	vertextype V[maxvertexnum];//顶点集
	edgetype E[maxvertexnum][maxvertexnum];//边集
	vertextype list[maxvertexnum]; // 顶点次序，为了形成连通图
	int n,e;//顶点数，边数
	enum GraphType GType;//设定图的类型
};
int randomnum()//返回一个0-99之间的随机数
{
	int a;
	//srand(time(NULL));//设置随机数种子，使每次运行获取的随机数不同
	a=rand()%maxvertexnum;

	return a;
}
void connect(MGraph *G){
	for(int i = 0; i < G->n; i++){
		G->list[i] = i;
	}
	random_shuffle(G->list, G->list+G->n);
	for(int i = 0; i < G->n - 1; i++){
		G->E[G->list[i]][G->list[i+1]] = 1;
		G->E[G->list[i+1]][G->list[i]] = 1;
	}
}
void CreateMGraphUG(MGraph *G)//生成无向图
{
	int i,j,k,w;
	G->GType=UG;//无向图
	G->n=maxvertexnum;
	G->e=EDGE_NUM;
	for(i=0;i<G->n;i++)
	{
		G->V[i]=i;//为每个顶点都编号从0-99
	}

	for(i=0;i<G->n;i++)
	{
		for(j=0;j<G->n;j++)
		{
			G->E[i][j]=0;//初始化邻接矩阵
		}
	}
	connect(G);
	for(k=0;k<G->e - G->n;k++)
	{
		do{
			i=randomnum();//随机生成任意两顶点
			j=randomnum();
		}
		while(G->E[i][j] == 1 || i == j);
		//cout<<i<<" "<<j<<"\t";
		G->E[i][j]=1;//建立邻接矩阵，边的权重都为1
		G->E[j][i]=1;//因为是无向图，所以要对称
	}

}

int main()
{
    MGraph *UGG = new MGraph;
	CreateMGraphUG(UGG);
    fstream fout(writeDataPath);//这里采用构造方法，也可以使用open()方法
	for (int i = 0; i < maxvertexnum; i++)
	{
        for(int j = 0; j < maxvertexnum - 1; j++){
            fout << UGG->E[i][j] << " ";
        }
        
        fout << UGG->E[i][maxvertexnum-1] << endl;
	}
}