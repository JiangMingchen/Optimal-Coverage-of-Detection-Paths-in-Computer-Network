#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <time.h>
#include <math.h>
#include <fstream>
using namespace std;
#define NODE_NUM 100
#define EDGE_NUM 400
#define GRAPH_FILE "100_UG_1.txt"
class Grape {
private:
	struct sa_param {
		const double K = 1.3806488e-23;	//������������
		const double T0 = 1e5;	//��ʼ�¶�
		const double T_end = 1e-9;	//�����¶�
		const double D = 0.92;	//�����ٶ�
		const int L = 50;	//ͬ�¶��µ�������
		const int Times = 100;	//�˻����
		double T = T0;	//�¶ȼ�¼
	}sap;	//�˻��㷨����
	int num_nodes;	//�����
	int num_vector = 0;	//�߸���
	int G[NODE_NUM][NODE_NUM];	//�ڽӾ���
	int **visit_path;	//�߷��ʼ�¼
	int *visit_nodes;	//����ʼ�¼
	int num_path = 0;	//·������
	int min_path = 99999;	//��С·������
	float min_S = 99999;	//��С����
	vector<vector<int> > path;	//·����¼
	vector<vector<int> > Stack;	//ջ
	float ss = 0;	//��¼��ǰ·������
public:
	Grape() {
		init();
	}
	void init() {	//���ݳ�ʼ������ ����Ϊ��������
		num_nodes = NODE_NUM;
		num_vector = EDGE_NUM;
		visit_nodes = new int[num_nodes];
		//G = new int *[num_nodes];	//��
		visit_path = new int *[num_nodes];
		for (int i = 0; i < num_nodes; i++) {
			visit_nodes[i] = -1;
			//G[i] = new int[num_nodes];	//��
			visit_path[i] = new int[num_nodes];
			for (int j = 0; j < num_nodes; j++) {
				G[i][j] = -1;	//��ʼ��Ϊ-1
				visit_path[i][j] = -1;
			}
		}
		ifstream infile;//�����ȡ�ļ���������ڳ�����˵��in
		infile.open(GRAPH_FILE);//���ļ�
		for (int i = 0; i < num_nodes; i++)//������ѭ��
		{
			for (int j = 0; j < num_nodes; j++)//������ѭ��
			{
				infile >> G[i][j];//��ȡһ��ֵ���ո��Ʊ�������и�������д�뵽�����У����в���ѭ������
			}
		}
		infile.close();//��ȡ���֮��ر��ļ�
		/*
		num_vector = 6;
		G[0][1] = G[1][0] = 1;
		G[1][2] = G[2][1] = 1;
		G[2][3] = G[3][2] = 1;
		G[3][4] = G[4][3] = 1;
		G[4][1] = G[1][4] = 1;
		G[4][2] = G[2][4] = 1;
		*/
	}
	void input() {	//�������뺯��
		Del();
		cout << "Please enter the number of nodes:";
		cin >> num_nodes;
		//��ʼ������
		visit_nodes = new int[num_nodes];
		//G = new int *[num_nodes];	//��
		visit_path = new int *[num_nodes];
		for (int i = 0; i < num_nodes; i++) {
			visit_nodes[i] = -1;
			//G[i] = new int[num_nodes];	//��
			visit_path[i] = new int[num_nodes];
			for (int j = 0; j < num_nodes; j++) {
				G[i][j] = -1;	//��ʼ��Ϊ-1
				visit_path[i][j] = -1;
			}
		}
		cout << "Please enter the number of path(start with zero):";
		cin >> num_vector;
		for (int i = 0; i < num_vector; i++) {
			int a, b;
			cout << "Start Point:";
			cin >> a;
			cout << "End Point:";
			cin >> b;
			G[a][b] = 1;
			G[b][a] = 1;
		}
	}
	void Del() {	//�ͷſռ�
		num_nodes = 0;
		num_vector = 0;
		path.clear();
		num_path = 0;
		for (int i = 0; i < num_nodes; i++) {
			delete[] visit_path[i];
			//delete[] G[i];
		}
		delete[] visit_path;
		//delete[] G;
		delete[] visit_nodes;
	}
	void Count() {	//����·������		·�����ȷ���
		float avg = 0 , avg_s = 0;
		int L = 0;
		for (int i = 0; i < Stack.size(); i++) {
			L += (Stack[i].size() - 1);
		}
		avg = float(L) / Stack.size();	//��ǰ·��ƽ��ֵ
		for (int i = 0; i < Stack.size(); i++) {
			avg_s += ((Stack[i].size() - 1) - avg)*((Stack[i].size() - 1) - avg);
		}
		ss = avg_s / Stack.size();	//��ǰ·������
	}
	void Clean() {	//������� ��Ҫ������շ��ʼ�¼
		Stack.clear();	//���ջ
		for (int i = 0; i < num_nodes; i++) {
			visit_nodes[i] = -1;	//��յ���ʱ�
		}
		for (int i = 0; i < num_nodes; i++) {
			for (int j = 0; j < num_nodes; j++) {
				visit_path[i][j] = -1;	//��ձ߷��ʱ�
			}
		}
	}
	void BestCover() {
		for(int i =0;i<sap.Times;i++)	//����sap.Times���˻��㷨
			SA();
	}
	void SA() {		//ģ���˻��㷨
		srand(time(NULL));	//�����������
		Rand_Result();	//ʹ��DFS�������һ�����
		if (eval()) {	
			Updata();	//����
		}
		sap.T = sap.T0;
		while (sap.T > sap.T_end) {	
			for (int i = 0; i < sap.L; i++) {
				vector<vector<int> > Last = Stack;	//��¼��ǰ��
				int get = Rand_Next();	//����任�����µĽ�
				if (eval()) {	//�ж��Ƿ����
					Updata();	//����
				}
				else {	//������
					Stack = Last;	//�ָ�ԭ��
				}
			}sap.T *= sap.D;	//����
			//cout << "�¶�:" << sap.T << endl;
		}
	}
	void Rand_Result() {	//�������һ�����
		Clean();
		int i = rand() % num_nodes;		//���
		vector<int> s;
		Stack.push_back(s);		//ջ1 ·��1
		num_path = 1;
		int key = DFS(i);	//����
		//cout << key << endl;
	}
	int DFS(int u) {	//���ʽ�㲢��¼���ԭ������������ڻָ���
		Stack[num_path - 1].push_back(u);	//ѹջ��¼
		visit_nodes[u] = 1;		//���ʱ��¼
		int has1 = 0;
		int v = rand() % num_nodes;	//���
		for(int i=0;i<num_nodes;i++){
			v = (v + 1) % num_nodes;
			if (G[u][v] > 0 && visit_path[u][v] < 0) {	//�������·����·��δ������
				has1 = 1;
				break;
			}
		}
		if (has1 == 1) {	//����·����·��δ������
			visit_path[u][v] = visit_path[v][u] = 1;	//���ʾ����¼
			return DFS(v);	//����
		}
		//Print_Stack(num_path);
		//�ڵ�ǰ�����ڵ�·����δ�ҵ�����·��
		//���ҵ�ǰ�������·��
		int has2 = 0;
		int a = rand() % num_nodes, b = rand() % num_nodes;	//���
		for (int i = 0; i < num_nodes; i++) {
			a = (a + 1) % num_nodes;
			for (int j = 0; j < num_nodes; j++) {
				b = (b + 1) % num_nodes;
				if (G[a][b] > 0 && visit_path[a][b] < 0) {	//����·����δ������
					has2 = 1;
					break;
				}
			}
			if (has2 == 1)
				break;
		}
		if (has2 == 1) {	//����·����δ������ ԭ·���޷�����  ������µ�·����¼
			//cout << "Found!" << a << b << endl;
			num_path++;	//������·��
			//cout << "\tPath Add!!!!!" << endl;
			vector<int> s;
			Stack.push_back(s);	//������ջ(·��)��¼
			return DFS(a);
		}
		//����δ������		��Щ��û��·����֮����
		for (int i = 0; i < num_nodes; i++) {	
			if (visit_nodes[i] < 0) {
				num_path++;
				vector<int> s;
				s.push_back(i);
				Stack.push_back(s);	//������ջ(·��)��¼
			}
		}
		return 1;	//���б���������¼
	}
	int Rand_Next() {	//������� ����µĽ��
		int key = 0;
		int x = rand() % Stack.size(), y = rand() % Stack.size();	//�������·�� ����һ�α� xɾ�� y���
		for (int i = 0; i < Stack.size(); i++) {
			x = (x + 1) % Stack.size();		//���x
			if (Stack[x].size() < 2) {	//ȷ��x·�����Ȳ�Ϊ0	��ΪҪ��x��ɾ����	���뱣֤���ڱ�
				continue;
			}
			for (int j = 0; j < Stack.size(); j++) {
				y = (y + 1) % Stack.size();	//���y
				if (y == x) {	//ȷ��x,y����
					continue;
				}
				//���x·����β���� �� ������y·����β����������б任
				if (Stack[y][0] == Stack[x][0]) {
					int t = Stack[x][1];
					Stack[x].erase(Stack[x].begin());
					Stack[y].insert(Stack[y].begin(), t);
					key = 1;
				}
				else if (Stack[y][0] == Stack[x][1]) {
					int t = Stack[x][0];
					Stack[x].erase(Stack[x].begin());
					Stack[y].insert(Stack[y].begin(), t);
					key = 1;
				}
				else if (Stack[y][0] == Stack[x][Stack[x].size() - 1]) {
					int t = Stack[x][Stack[x].size() - 2];
					Stack[x].pop_back();
					Stack[y].insert(Stack[y].begin(), t);
					key = 1;
				}
				else if (Stack[y][0] == Stack[x][Stack[x].size() - 2]) {
					int t = Stack[x][Stack[x].size() - 1];
					Stack[x].pop_back();
					Stack[y].insert(Stack[y].begin(), t);
					key = 1;
				}
				else if (Stack[y][Stack[y].size() - 1] == Stack[x][0]) {
					int t = Stack[x][1];
					Stack[x].erase(Stack[x].begin());
					Stack[y].push_back(t);
					key = 1;
				}
				else if (Stack[y][Stack[y].size() - 1] == Stack[x][1]) {
					int t = Stack[x][0];
					Stack[x].erase(Stack[x].begin());
					Stack[y].push_back(t);
					key = 1;
				}
				else if (Stack[y][Stack[y].size() - 1] == Stack[x][Stack[x].size() - 1]) {
					int t = Stack[x][Stack[x].size() - 2];
					Stack[x].pop_back();
					Stack[y].push_back(t);
					key = 1;
				}
				else if (Stack[y][Stack[y].size() - 1] == Stack[x][Stack[x].size() - 2]) {
					int t = Stack[x][Stack[x].size() - 1];
					Stack[x].pop_back();
					Stack[y].push_back(t);
					key = 1;
				}
				if (key == 1) {	//������б任
					if (Stack[x].size() < 2) {	//x·���ޱ� ������·����ɾ��
						num_path--;	//����·������
						Stack.erase(Stack.begin() + x);	//ɾ��·����Ӧջ
					}
					return key;	//����key
				}
			}
		}
		return key;
	}
	void Updata() {	//�������Ž�
		min_path = num_path;
		min_S = ss;
		path = Stack;
	}
	int eval() {	//���� �Ƿ����
		Count();	//����·��������·�����ȷ���
		if (num_path < min_path)	//�ҵ�����·��
			return 1;
		else if (num_path == min_path && ss < min_S)	//��ͬ·�� �������С
			return 1;
		else if (up_f())	//���ʽ���
			return 1;
		else
			return 0;	//�ܾ�
	}
	int up_f() {	//δ���� ���ɸ��ʲ��ж�
		double t = -(((ss - min_S) + (num_path - min_path)) / (sap.T));	//���ɽ��ܸ���
		double f = exp(t);
		double rf = double(rand()) / double(RAND_MAX);
		//cout << "rf:" << rf << "\tf:" << f << endl;
		if (rf < f)
			return 1;	//����
		else
			return 0;	//�ܾ�
	}
	//��ӡ����
	void Print_Stack(int num) {	//��ӡ���Ϊnumջ���ݣ�·����
		cout << "Stack" << num << ":";
		for (int i = 0; i < Stack[num - 1].size(); i++) {
			cout << Stack[num - 1][i];
		}
		cout << endl;
	}
	void Print() {	//��ӡ���ҵ������Ž�
		cout << "Result:" << endl;	
		for (int i = 0; i < path.size(); i++) {
			cout << "Path:";
			for (int j = 0; j < path[i].size(); j++) {
				cout << path[i][j] << " ";
			}
			cout << "\n";
		}
		cout << endl << "Num:"<<min_path << "\tVar:" << min_S << endl;

	}
};

void DFS_method(){
	Grape G;	//�������
	G.Rand_Result();
	G.Count();
	G.Updata();
	G.Print();	//������
	G.Del();	//�ͷſռ�
}
void SA_method(){
	Grape G;	//�������
	G.BestCover();	//�����㷨
	G.Print();	//������
	G.Del();	//�ͷſռ�
}

int main() {
	DFS_method(); // DFS
	//SA_method();  // SA
}
/*
const double K = 1.3806488e-23;	//������������
const double T0 = 1e5;	//��ʼ�¶�
const double T_end = 1e-9;	//�����¶�
const double D = 0.90;	//�����ٶ�
const int L = 100;	//ͬ�¶��µ�������
const int Times = 100;	//�˻����
double T = T0;	//�¶ȼ�¼
10_UG_1.txt
Result:
Path:0 5 7 4 6 5 3 6 0 9 4 0 1 7 3 8 7 2 5 9 1 3 4 1 2 6 7 9 2 
Path:1 8 4 5 8 2 0 3 9 8 0 7 

Num:2   Var:72.25


const double K = 1.3806488e-23;	//������������
const double T0 = 1e5;	//��ʼ�¶�
const double T_end = 1e-9;	//�����¶�
const double D = 0.97;	//�����ٶ�
const int L = 100;	//ͬ�¶��µ�������
const int Times = 100;	//�˻����
double T = T0;	//�¶ȼ�¼
20_UG_1.txt
Result:
Path:19 18 5 9 7 4 14 5 15 8 3 5 0 19 9 16 17 6 11 8 14 15 6 12 8 13 18 10 3 9 17 
Path:3 16 4 18 11 15 16 10 19 1 9 4 0 12 18 15 
Path:12 15 19 2 12 9 11 1 13 7 12 10 11 
Path:13 16 0 17 18 1 17 8 1 7 3 14 17 15 13 14 
Path:0 13 10 8 4 5 2 6 

Num:5   Var:58.96


const double K = 1.3806488e-23;	//������������
const double T0 = 1e5;	//��ʼ�¶�
const double T_end = 1e-9;	//�����¶�
const double D = 0.91;	//�����ٶ�
const int L = 100;	//ͬ�¶��µ�������
const int Times = 100;	//�˻����
double T = T0;	//�¶ȼ�¼
50_UG_1.txt
Result:
Path:7 9 38 7 26 30 13 45 44 28 14 23 38 49 24 25 11 35 20 23 22 10 39 
Path:40 29 49 28 16 35 17 20 15 44 26 19 37 3 1 43 19 41 10 4 44 34 46 35 45 12 6 39 5 4 23 29 1 47 
Path:24 3 47 26 0 44 40 5 26 8 44 20 
Path:17 21 28 33 5 30 3 33 41 28 47 4 27 29 9 0 6 1 21 29 4 31 3 16 22 30 9 23 39 22 32 2 25 10 7 44 11 7 47 27 43 36 
Path:45 17 25 45 26 12 27 8 37 16 40 17 39 48 16 
Path:8 12 40 31 24 46 5 6 23 47 8 23 
Path:3 19 25 34 9 31 21 30 16 29 2 18 12 17 22 48 
Path:5 32 
Path:37 14 47 33 35 18 37 45 18 44 38 
Path:34 16 2 13 20 12 11 42 49 31 38 13 
Path:35 47 46 18 40 15 14 31 48 1 49 7 0 8 10 33 48 15 42 45 19 47 15 
Path:4 48 35 13 36 7 15 35 6 

Num:12  Var:115.576


const double K = 1.3806488e-23;	//������������
const double T0 = 1e5;	//��ʼ�¶�
const double T_end = 1e-9;	//�����¶�
const double D = 0.88;	//�����ٶ�
const int L = 100;	//ͬ�¶��µ�������
const int Times = 100;	//�˻����
double T = T0;	//�¶ȼ�¼
30_UG_1.txt
Result:
Path:18 9 12 2 7 10 2 21 7 18 11 21 6 13 10 22 11 29 5 17 8 20 16 12 26 21 9 20 23 10 3 7 29 22 5 8 23 11 8 14 0 5 20 4 15 8 24 23 6 25 13 18 4 
Path:16 22 15 9 27 8 26 22 19 27 16 25 22 
Path:3 12 15 25 26 7 19 17 9 1 8 2 28 23 19 3 20 28 24 25 7 24 26 3 24 4 16 0 2 9 14 
Path:13 11 14 15 17 1 28 4 10 
Path:21 18 25 1 23 27 7 0 13 27 
Path:19 20 21 27 18 23 16 5 15 

Num:6   Var:266.139


const double K = 1.3806488e-23;	//������������
const double T0 = 1e5;	//��ʼ�¶�
const double T_end = 1e-9;	//�����¶�
const double D = 0.95;	//�����ٶ�
const int L = 100;	//ͬ�¶��µ�������
const int Times = 100;	//�˻����
double T = T0;	//�¶ȼ�¼
40_UG_1.txt
Result:
Path:29 3 2 10 13 26 34 31 21 29 24 38 17 8 34 4 15 25 10 23 27 32 0 5 15 26 17 36 39 0 9 23 14 20 37 5 25 17 30 39 32 6 37 35 8 21 7 32 28 3 11 9 20 4 25 29 19 20 18 36 
Path:31 30 4 37 17 18 11 14 24 16 5 12 22 39 7 9 33 0 23 20 
Path:26 3 22 11 28 13 1 38 14 25 2 26 23 16 15 6 25 33 
Path:12 0 16 30 8 37 34 11 27 33 18 21 
Path:6 11 2 12 18 15 19 32 10 31 9 
Path:16 33 1 
Path:28 24 8 12 15 35 29 37 
Path:4 36 28 10 33 31 28 15 7 36 21 17 
Path:27 4 38 19 21 22 5 
Path:2 6 14 3 18 35 14 16 35 13 0 15 
Path:7 25 1 9 39 15 32 

Num:11  Var:220.066


const double K = 1.3806488e-23;	//������������
const double T0 = 1e5;	//��ʼ�¶�
const double T_end = 1e-9;	//�����¶�
const double D = 0.91;	//�����ٶ�
const int L = 100;	//ͬ�¶��µ�������
const int Times = 10;	//�˻����
double T = T0;	//�¶ȼ�¼
100_UG_1.txt
Result:
Path:51 49 7 15 6 62 48 65 14 78 44 99 25 34 27 81 98 1 24 95 68 12 40 20 97 1 71 91 26 97 33 55 46 93 10 47 85 96 18 14 67 72 91 7 20 44 58 0 83 70 60 57 85 17 25 61 5 95 12 67 80 54 71 28 91 33 10 16 90 31 9 
Path:96 19 31 74 56 66 52 3 80 79 65 53 22 66 76 8 36 66 78 99 27 65 47 7 38 57 35 98 92 15 19 95 94 33 28 16 98 51 36 63 76 95 3 98 72 30 66 34 50 59 38 10 4 47 53 58 3 30 6 23 88 17 63 2 82 52 51 1 53 89 11 79 40 23 54 39 65 42 49 
Path:62 8 77 97 45 73 89 55 90 68 35 6 83 12 76 57 94 4 48 42 60 93 69 91 84 
Path:97 69 25 20 67 89 60 94 38 63 11 42 84 89 25 84 94 15 29 1 56 71 49 24 32 74 15 81 86 9 73 3 76 5 73 58 7 41 32 14 25 10 58 33 60 64 29 44 38 43 48 85 1 79 21 30 46 99 
Path:95 44 68 21 54 97 64 69 47 88 89 0 10 45 75 34 67 19 74 18 20 81 4 60 54 18 25 
Path:87 90 44 8 50 71 
Path:85 45 37 99 42 28 36 93 79 10 
Path:13 14 31 95 17 41 77 4 3 69 57 98 39 52 79 49 30 13 0 20 11 44 0 26 5 80 9 57 11 16 20 23 9 29 5 
Path:4 5 82 85 70 13 35 16 23 29 18 70 17 43 75 37 52 11 62 20 
Path:45 92 42 16 37 87 59 58 37 19 75 
Path:0 56 
Path:64 76 22 73 50 31 88 
Path:36 21 33 84 12 
Path:72 40 3 87 71 96 55 
Path:67 49 31 3 24 53 83 35 31 15 
Path:90 66 49 13 78 26 94 8 97 61 
Path:27 24 96 40 54 77 32 3 
Path:39 6 55 80 30 
Path:38 46 76 24 65 40 
Path:76 45 61 35 15 47 23 2 
Path:60 72 39 67 21 
Path:6 50 27 29 28 
Path:98 41 

Num:23  Var:465.618



const double K = 1.3806488e-23;	//������������
const double T0 = 1e5;	//��ʼ�¶�
const double T_end = 1e-9;	//�����¶�
const double D = 0.88;	//�����ٶ�
const int L = 100;	//ͬ�¶��µ�������
const int Times = 100;	//�˻����
double T = T0;	//�¶ȼ�¼
60_UG_1.txt
Result:
Path:11 8 37 54 38 53 2 12 25 6 50 5 52 15 12 26 54 58 1 36 45 14 4 7 21 57 27 55 8 29 37 14 46 34 6 15 5 29 53 31 58 5 0 18 34 57 4 28 45 25 48 47 45 44 57 37 25 16 32 7 15 55 20 19 15 33 38 56 55 57 38 59 26 3 22 5 
Path:18 23 42 46 43 37 26 10 32 58 
Path:25 22 38 15 34 32 39 57 16 50 53 10 35 17 48 57 40 52 20 49 19 22 11 29 0 1 56 24 54 20 30 50 51 41 44 11 53 4 
Path:51 53 20 10 39 18 51 2 23 48 13 29 27 0 24 55 9 17 56 20 40 16 38 6 54 23 56 28 50 39 12 44 0 37 48 55 28 54 
Path:45 57 42 3 15 
Path:46 22 40 21 13 55 59 33 40 
Path:14 53 41 7 49 43 50 45 1 9 44 8 32 19 27 47 35 36 13 17 15 39 21 
Path:34 51 30 19 38 17 31 50 33 43 27 23 36 30 43 
Path:20 34 45 49 3 33 24 48 
Path:3 11 4 10 43 40 34 10 
Path:13 41 
Path:24 38 7 33 9 35 46 0 43 18 26 
Path:23 49 31 8 51 9 49 22 52 

Num:13  Var:390.237


const double K = 1.3806488e-23;	//������������
const double T0 = 1e5;	//��ʼ�¶�
const double T_end = 1e-9;	//�����¶�
const double D = 0.97;	//�����ٶ�
const int L = 100;	//ͬ�¶��µ�������
const int Times = 10;	//�˻����
double T = T0;	//�¶ȼ�¼
70_UG_1.txt
Result:
Path:48 54 18 24 25 45 46 2 50 10 25 65 20 19 53 61 43 49 50 3 23 47 69 55 14 28 22 69 35 64 51 9 69 58 32 15 22 21 8 10 4 34 37 15 43 0 15 44 56 40 35 14 64 21 39 18 64 6 31 43 1 66 50 56 63 62 69 
Path:30 46 58 3 63 2 23 8 11 30 9 0 4 43 8 48 23 29 59 45 47 0 7 28 59 2 11 38 60 39 37 45 12 54 34 10 28 27 9 
Path:33 49 61 57 45 16 44 62 2 25 55 19 17 51 20 59 27 55 33 7 11 52 33 47 6 12 56 65 46 25 3 15 7 48 67 57 
Path:20 62 42 27 36 29 67 17 63 26 5 58 42 13 68 53 24 55 50 33 40 0 21 44 8 16 
Path:64 28 37 23 36 60 6 44 1 12 
Path:63 42 21 66 38 67 66 
Path:21 49 7 5 13 14 20 63 7 61 15 
Path:31 37 49 29 28 41 11 57 24 60 52 54 57 33 60 50 13 10 29 18 47 8 34 
Path:42 61 19 0 56 13 24 19 
Path:50 39 53 40 7 29 
Path:13 20 67 35 45 60 25 67 34 48 45 
Path:26 68 65 30 43 29 20 36 45 39 
Path:3 24 5 65 57 34 18 65 
Path:52 22 14 50 4 
Path:36 64 26 60 46 41 51 33 31 38 14 
Path:22 30 31 41 2 69 33 32 
Path:68 16 48 15 56 8 49 11 18 43 

Num:17  Var:254.242
*/