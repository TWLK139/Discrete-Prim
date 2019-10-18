#include<iostream>
#include <cstdlib>
#include <ctime>
#include<stdlib.h>

#define INF 65535								//���������
#define MaxVerNum 8								//������󶥵����

typedef int elementType;						//����ͼ�ж������������
typedef float eInfotype;						//����eInfo���������ͣ���Ȩֵ����������

typedef struct eNode							//���������Ľ��ṹ
{
	int adjVer;									//�ڽӶ�����Ϣ���˴�Ϊ�����ţ���1��ʼ
	eInfotype eInfo;							//�������б�ʾ�ߵ������Ϣ������ߵ�Ȩֵ
	struct eNode* next;							//ָ��������е���һ�����
} EdgeNode;										//������������

typedef struct vNode							//���嶥���Ľ��ṹ
{
	elementType data;							//���ͼ�ж�������ݽṹ
	EdgeNode* firstEdge;						//ָ��˶�������ĵ�һ���ߵ�ָ�룬���������ͷָ��
} VerNode;										//�����������

typedef struct GraphAdjLinkList					//����ͼ������ṹ
{
	VerNode VerList[MaxVerNum];					//�����
	int VerNum;									//������
	int ArcNum;									//����
} Graph;										//ͼ��������

typedef struct minEdgeType
{
	int v;										//������ѡһ�ζ˵Ķ�����
	eInfotype eWeight;							//�ߵ�Ȩֵ
} MinEdgeType;									//�������Ԫ������

typedef struct result							//�洢����Ľṹ
{
	elementType begin[MaxVerNum - 1];			//��ʼ��
	eInfotype weight[MaxVerNum - 1];			//Ȩֵ
	elementType end[MaxVerNum - 1];				//������

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

	//ʹ��Prim�㷨
	Prim(oilPipeline, 1, minOilPipeline);

	//��ӡ���
	for (int j = 0; j < MaxVerNum - 1; j++)
	{
		allLength += minOilPipeline.weight[j];
		cout << "��ʼ��" << minOilPipeline.begin[j] << "������" << minOilPipeline.end[j] << endl;
	}
	cout << "���裺" << allLength << "ǧ�ס�" << endl;

	//�ͷ�����Ŀռ�
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

//���ܣ��ж϶���vBegin��vEnde֮���Ƿ��бߣ������رߵ�Ȩֵ
//���أ�trueΪ�бߣ�falseΪ�ޱߣ�eWeightΪ�ߵ�Ȩֵ
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

//���ܣ���ʼ������TE
void InitialTE(Graph& G, MinEdgeType TE[], int vID)
{
	int i;
	eInfotype eWeight;									//��������������Ȩֵ
	for (i = 1; i <= G.VerNum; i++)
	{
		//��ʼ��������TE[]
		if (HasEdge(G, vID, i, eWeight))
		{
			TE[i - 1].v = vID;							//����ߣ�vID��i֮��ı�
			TE[i - 1].eWeight = eWeight;				//����ߵ�Ȩֵ
		}
		else
		{
			TE[i - 1].eWeight = INF;					//vID��i֮��û�бߣ�Ȩֵ��Ϊ�����
		}
	}
}

//���ܣ��ӱ߼�����TE[]�л�ȡȨֵ��С�ĺ�ѡ��
//���أ���ѡ����һ�˵Ķ�����
int GetMinEdge(Graph& G, MinEdgeType TE[])
{
	eInfotype eMin = INF;								//������С��Ȩֵ����ʼ��Ϊ�����
	int i, j = 0;
	for (i = 1; i <= G.VerNum; i++)
	{
		if (visited[i - 1] == false && TE[i - 1].eWeight < eMin)
		{
			j = i;
			eMin = TE[i - 1].eWeight;
		}
	}

	return j;											//��Ӧ�ıߣ�TE[j-1].v, j��Ϊѡ�еı�
}

//���ܣ�������vID��ѡ�б�Ϊ��ѡ����󣬸��º�ѡ�߼�
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

//Prim�㷨
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