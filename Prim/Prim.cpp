#include<iostream>
#include <cstdlib>
#include <ctime>
#include<stdlib.h>

#define INF 65535								//定义无穷大
#define MaxVerNum 8								//定义最大顶点个数

typedef int elementType;						//定义图中顶点的数据类型
typedef float eInfotype;						//定义eInfo的数据类型，即权值的数据类型

typedef struct eNode							//定义边链表的结点结构
{
	int adjVer;									//邻接顶点信息，此处为顶点编号，从1开始
	eInfotype eInfo;							//边链表中表示边的相关信息，比如边的权值
	struct eNode* next;							//指向边链表中的下一个结点
} EdgeNode;										//边链表结点类型

typedef struct vNode							//定义顶点表的结点结构
{
	elementType data;							//存放图中顶点的数据结构
	EdgeNode* firstEdge;						//指向此顶点关联的第一条边的指针，即边链表的头指针
} VerNode;										//顶点表结点类型

typedef struct GraphAdjLinkList					//定义图的整体结构
{
	VerNode VerList[MaxVerNum];					//顶点表
	int VerNum;									//顶点数
	int ArcNum;									//边数
} Graph;										//图的类型名

typedef struct minEdgeType
{
	int v;										//边中已选一段端的顶点编号
	eInfotype eWeight;							//边的权值
} MinEdgeType;									//边数组的元素类型

typedef struct result							//存储结果的结构
{
	elementType begin[MaxVerNum - 1];			//起始点
	eInfotype weight[MaxVerNum - 1];			//权值
	elementType end[MaxVerNum - 1];				//结束点

} resType;

bool visited[MaxVerNum] = { false };

bool HasEdge(Graph& G, int vBegin, int vEnd, eInfotype& eWeight);
void InitialTE(Graph& G, MinEdgeType TE[], int vID);
int GetMinEdge(Graph& G, MinEdgeType TE[]);
void UpdateTE(Graph& G, MinEdgeType TE[], int vID);
void Prim(Graph& G, int vID, resType &res);

using namespace std;

int main(void)
{
	Graph oilPipeline;
	EdgeNode * ep = nullptr, * newEp = nullptr;
	eInfotype eInfo[MaxVerNum][MaxVerNum] = { {0,1.3,2.1,0.9,0.7,1.8,2.0,1.8},{1.3,0,0.9,1.8,1.2,2.8,2.3,1.1},{2.1,0.9,0,2.6,1.7,2.5,1.9,1.0},{0.9,1.8,2.6,0,0.7,1.6,1.5,0.9},{0.7,1.2,1.7,0.7,0,0.9,1.1,0.8},{1.8,2.8,2.5,1.6,0.9,0,0.6,1.0},{2.0,2.3,1.9,1.5,1.1,0.6,0,0.5},{1.8,1.1,1.0,0.9,0.8,1.0,0.5,0} };
	resType minOilPipeline;
	eInfotype allLength = 5;
	
	oilPipeline.ArcNum = 0;
	for (int i = 0; i < MaxVerNum; i++)
	{
		for (int j = 0; j < MaxVerNum; j++)
		{
			if (i == (MaxVerNum - 1 - j)) {
				continue;
			}
			newEp = new EdgeNode;
			newEp->next = ep;
			newEp->adjVer = 8 - j;
			newEp->eInfo = eInfo[i][MaxVerNum - 1 - j];
			ep = newEp;
			oilPipeline.ArcNum++;
		}
		oilPipeline.VerList[i].data = i + 1;
		oilPipeline.VerList[i].firstEdge = ep;
	}
	oilPipeline.ArcNum /= 2;
	oilPipeline.VerNum = MaxVerNum;

	//使用Prim算法
	Prim(oilPipeline, 1, minOilPipeline);

	//打印结果
	for (int j = 0; j < MaxVerNum - 1; j++)
	{
		allLength += minOilPipeline.weight[j];
		cout << "起始：" << minOilPipeline.begin[j] << "结束：" << minOilPipeline.end[j] << endl;
	}
	cout << "共需：" << allLength << "千米。" << endl;

	//释放申请的空间
	for (int i = 0; i < MaxVerNum; i++)
	{
		for (int j = 0; j < MaxVerNum-1; j++)
		{
			newEp = oilPipeline.VerList[i].firstEdge;
			oilPipeline.VerList[i].firstEdge = newEp->next;
			free(newEp);
		}
	}

	return 0;
}

//功能：判断顶点vBegin和vEnde之间是否有边，并返回边的权值
//返回：true为有边；false为无边；eWeight为边的权值
bool HasEdge(Graph& G, int vBegin, int vEnd, eInfotype& eWeight)
{
	EdgeNode* p;
	bool f = false;
	eWeight = INF;
	p = G.VerList[vBegin - 1].firstEdge;
	while (p)
	{
		if (p->adjVer == vEnd)
		{
			f = true;
			eWeight = p->eInfo;
			break;
		}
		p = p->next;
	}

	return f;
}

//功能：初始化数组TE
void InitialTE(Graph& G, MinEdgeType TE[], int vID)
{
	int i;
	eInfotype eWeight;									//定义变量保存变量权值
	for (i = 1; i <= G.VerNum; i++)
	{
		//初始化边数组TE[]
		if (HasEdge(G, vID, i, eWeight))
		{
			TE[i - 1].v = vID;							//保存边，vID与i之间的边
			TE[i - 1].eWeight = eWeight;				//保存边的权值
		}
		else
		{
			TE[i - 1].eWeight = INF;					//vID与i之间没有边，权值置为无穷大
		}
	}
}

//功能：从边集数组TE[]中获取权值最小的侯选边
//返回：侯选边另一端的顶点编号
int GetMinEdge(Graph& G, MinEdgeType TE[])
{
	eInfotype eMin = INF;								//保存最小的权值，初始化为无穷大
	int i, j = 0;
	for (i = 1; i <= G.VerNum; i++)
	{
		if (visited[i - 1] == false && TE[i - 1].eWeight < eMin)
		{
			j = i;
			eMin = TE[i - 1].eWeight;
		}
	}

	return j;											//对应的边（TE[j-1].v, j）为选中的边
}

//功能：当顶点vID被选中变为已选顶点后，更新侯选边集
void UpdateTE(Graph& G, MinEdgeType TE[], int vID)
{
	int i, j;
	eInfotype eWeight;
	for (i = 1; i <= G.VerNum; i++)
	{
		if (visited[i - 1] == false)
		{
			if (HasEdge(G, vID, i, eWeight) && eWeight < TE[i - 1].eWeight)
			{
				TE[i - 1].v = vID;
				TE[i - 1].eWeight = eWeight;
			}
		}
	}
}

//Prim算法
void Prim(Graph& G, int vID, resType& res)
{
	MinEdgeType TE[MaxVerNum];
	int i;
	int curID;
	InitialTE(G, TE, vID);
	visited[vID - 1] = true;
	for (i = 1; i < G.VerNum; i++)
	{
		curID = GetMinEdge(G, TE);
		visited[curID - 1] = true;
		UpdateTE(G, TE, curID);
		res.begin[i - 1] = TE[curID - 1].v;
		res.weight[i - 1] = TE[curID - 1].eWeight;
		res.end[i - 1] = curID;
	}
}