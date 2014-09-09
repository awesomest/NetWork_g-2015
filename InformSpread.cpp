#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <algorithm>
#include <map>
#include <set>
#include <random>

#define NETWORKSIZE 1000000
#define USENETWORKDATA 100
#define EXISTDATA 1000
#define SPREADCOUNT 100
#define READ 1
#define WRITE 2
#define DESC 1
#define ASC 2

using namespace std;

vector<vector<int> > network(NETWORKSIZE);
bool informReceived[NETWORKSIZE];
int degree[NETWORKSIZE];

struct TransferInform
{ int source,target; };

struct NodeInform
{ int node,degree; };

bool degree_ASC(NodeInform n1,NodeInform n2)
{ return n1.degree<n2.degree; }

bool degree_DESC(NodeInform n1,NodeInform n2)
{ return n1.degree>n2.degree; }

string fileName(int i,int type,string name="",int digit=10)
{
	string No;

	while(i)
	{
		char n='0'+(i%digit);
		No=n+No;
		i/=digit;
	}

	if(type==WRITE)
		return name+"_time"+No+".dat";
	else
		return "data"+No+".dat";
}

void rebuildNetwork(int No)
{
	ifstream READFile;
	int node1,node2;

	memset(degree,0,sizeof(degree));

	for(int i=0;i<NETWORKSIZE;i++)
		network[i].clear();

	READFile.open(fileName(No,READ).c_str());
	while(READFile>>node1>>node2)
	{
		network[node1].push_back(node2);
		network[node2].push_back(node1);
		degree[node1]++;
		degree[node2]++;
	}

	READFile.close();
}

double totalInverseWeight(vector<vector<int> > &edge,int source)
{
	double weight=0;
	for(int i=0;i<edge[source].size();i++)
		weight+=1.0/network[edge[source][i]].size();

	return weight;
}

void noControlSpREAD(int source,int No)
{
	//ファイルを作り
	ofstream writeFile;
	writeFile.open(fileName(No,WRITE,"NC").c_str());

	vector<vector<int> > edge(NETWORKSIZE);
	vector<TransferInform> transferList;
	TransferInform transfer;
	int target,sumInformReceived=1,times=0;

	//情報獲得の確認用の配列を初期化
	memset(informReceived,0,sizeof(informReceived));

	//edgeの初期化
	for(int node=0;node<NETWORKSIZE;node++)
		copy(network[node].begin(),network[node].end(),back_inserter(edge[node]));

	queue<int> sourceList;
	sourceList.push(source);
	informReceived[source]=1;

	//情報の拡散
	while(sumInformReceived!=NETWORKSIZE)
	{
		int informSpREADSource=sourceList.size();
		times++;

		while(informSpREADSource--)
		{
			source=sourceList.front(); sourceList.pop(); 

			if(!edge[source].size()) continue;

			target=edge[source][rand()%edge[source].size()];
			edge[source].erase(find(edge[source].begin(),edge[source].end(),target));

			transfer.source=source;
			transfer.target=target;
			transferList.push_back(transfer);

			if(edge[source].size()) sourceList.push(source);

			if(!informReceived[target])
			{
				sumInformReceived++;
				informReceived[target]=1;
				sourceList.push(target);
			}
		}

		//情報を送ってもらったnodeを再び送らないように
		for(int i=0;i<transferList.size();i++)
		{
			vector<int>::iterator v=find(edge[transferList[i].target].begin(),edge[transferList[i].target].end(),transferList[i].source);

			if(v!=edge[transferList[i].target].end())
				edge[transferList[i].target].erase(v);
		}

		transferList.clear();

		writeFile<<times<<" "<<sumInformReceived<<endl;
	}

	edge.clear();
	writeFile.close();
}

void inverseControlSpREAD(int source,int No)
{
	//ファイルを作り
	ofstream writeFile;
	writeFile.open(fileName(No,WRITE,"IC").c_str());

	vector<vector<int> > edge(NETWORKSIZE);
	vector<double> targetSelect;
	vector<TransferInform> transferList;
	TransferInform transfer;
	int target,sumInformReceived=1,times=0;

	//情報獲得の確認用の配列を初期化
	memset(informReceived,0,sizeof(informReceived));

	//edgeの初期化
	for(int node=0;node<NETWORKSIZE;node++)
		copy(network[node].begin(),network[node].end(),back_inserter(edge[node]));

	queue<int> sourceList;
	sourceList.push(source);
	informReceived[source]=1;

	//情報の拡散
	while(sumInformReceived!=NETWORKSIZE)
	{
		int informSpREADSource=sourceList.size();
		times++;
		while(informSpREADSource--)
		{
			source=sourceList.front(); sourceList.pop();
			if(!edge[source].size())
				continue;
			else if(edge[source].size()==1) 
			{
				target=edge[source][0];
				edge[source].clear();
				transfer.source=source;
				transfer.target=target;
				transferList.push_back(transfer);
			}
			else
			{
				double weight=totalInverseWeight(edge,source);

				for(int neighbor=0;neighbor<edge[source].size();neighbor++)
					targetSelect.push_back((1.0/network[edge[source][neighbor]].size())/weight);

				double p=(double)rand()/(RAND_MAX+1.0);

				weight=0;
				for(int neighbor=0;neighbor<edge[source].size();neighbor++)
				{
					weight+=targetSelect[neighbor];
					if(weight>p)
					{
						target=edge[source][neighbor];
						// この単位時間にターゲットが情報源に情報を伝達しようとする可能性がある
						// だからターゲットから情報源への伝達経路はまだ残しておく
						edge[source].erase(edge[source].begin()+neighbor);
						targetSelect.clear();
						break;
					}
				}
			}

			transfer.source=source;
			transfer.target=target;
			transferList.push_back(transfer);

			if(edge[source].size()) sourceList.push(source);

			if(!informReceived[target])
			{
				informReceived[target]=1;
				sumInformReceived++;
				sourceList.push(target);
			}
		}

		//情報を送ってもらったnodeを再び送らないように
		for(int i=0;i<transferList.size();i++)
		{
			vector<int>::iterator v=find(edge[transferList[i].target].begin(),edge[transferList[i].target].end(),transferList[i].source);

			if(v!=edge[transferList[i].target].end())
				edge[transferList[i].target].erase(v);
		}

		transferList.clear();

		writeFile<<times<<" "<<sumInformReceived<<endl;
	}

	edge.clear();
	writeFile.close();
}

void degreeControlSpREAD(int source,int method,int No)
{
	//ファイルを作り
	ofstream writeFile;

	if(method==DESC) 
		writeFile.open(fileName(No,WRITE,"DC_DESC").c_str());
	else
		writeFile.open(fileName(No,WRITE,"DC_ASC").c_str());

	vector<vector<NodeInform> > edge(NETWORKSIZE);
	vector<TransferInform> transferList;
	TransferInform transfer;
	int target,neighbor,sumInformReceived=1,times=0;

	//情報獲得の確認用の配列を初期化
	memset(informReceived,0,sizeof(informReceived));

	//edgeの初期化
	for(int i=0;i<NETWORKSIZE;i++)
	{
		edge[i].resize(network[i].size());

		for(int j=0;j<network[i].size();j++)
		{
			edge[i][j].node=network[i][j];
			edge[i][j].degree=degree[network[i][j]];
		}
	}

	//DESCかASCのソート
	if(method==DESC)
	{
		for(int i=0;i<NETWORKSIZE;i++)
			sort(edge[i].begin(),edge[i].end(),degree_DESC);
	}
	else
	{
		for(int i=0;i<NETWORKSIZE;i++)
			sort(edge[i].begin(),edge[i].end(),degree_ASC);
	}

	queue<int> sourceList;
	sourceList.push(source);
	informReceived[source]=1;

	//情報の拡散
	while(sumInformReceived!=NETWORKSIZE)
	{
		int informSpREADSource=sourceList.size();
		times++;
		while(informSpREADSource--)
		{
			source=sourceList.front(); sourceList.pop();

			if(!edge[source].size()) continue;

			for(neighbor=1;neighbor<edge[source].size() && edge[source][neighbor].degree==edge[source][neighbor-1].degree;neighbor++);

			int select=rand()%neighbor;
			target=edge[source][select].node;
			edge[source].erase(edge[source].begin()+select);

			transfer.source=source;
			transfer.target=target;
			transferList.push_back(transfer);

			if(edge[source].size()) sourceList.push(source);

			if(!informReceived[target])
			{
				sumInformReceived++;
				informReceived[target]=1;
				sourceList.push(target);
			}
		}

		//情報を送ってもらったnodeを再び送らないように
		for(int i=0;i<transferList.size();i++)
		{
			vector<NodeInform>::iterator p;

			for(int j=0;j<edge[transferList[i].target].size();j++)
				if(edge[transferList[i].target][j].node==transferList[i].source)
				{
					p=edge[transferList[i].target].begin();
					edge[transferList[i].target].erase(p+j);
					break;
				}
		}

		transferList.clear();

		writeFile<<times<<" "<<sumInformReceived<<endl;
	}

	edge.clear();
	writeFile.close();
}

int main()
{
	srand(time(NULL));
	set<int> firstSource,useData;
	int count=1;
	ofstream writeFile;
	writeFile.open("network and first source list.txt");

	//使うnetworkのデータをラダンムで選び
	while(useData.size()<USENETWORKDATA)
	{ 
		int number=(rand()%EXISTDATA)+1;

		useData.insert(number);
	}

	// 修正済み
	/*******************************
		使うネットワークも100個の情報源も
		setに入れて重複を防ぐ
	 *******************************/

	// 100個のネットワークに対して
	set<int>::iterator it = useData.begin();
	for( ; it!=useData.end(); it++)
	{
		firstSource.clear();
		rebuildNetwork(*it);

		writeFile << *it << endl; //使ったnetworkのデータを記録

		//最初のsourceをランダムで選び
		// 各ネットワークに100個ずつの情報源を与える
		while(firstSource.size()<SPREADCOUNT)
		{
			int node=rand()%NETWORKSIZE;

			if (!firstSource.count(node)){
				firstSource.insert(node);
				writeFile << node << " "; //選ばれたsourceを記録
			}
		}

		writeFile<<endl;

		set<int>::iterator ite = firstSource.begin();
		for(int j=0;j<SPREADCOUNT;j++,count++,ite++)
		{
			srand(j+1);
			noControlSpREAD(*ite,count); //近傍nodeをrandomで選び
			srand(j+1);
			inverseControlSpREAD(*ite,count); //確率で次数が小さい近傍nodeを選ぴ安いように
			srand(j+1);
			degreeControlSpREAD(*ite,DESC,count); //近傍nodeの次数個数を降順で選び
			srand(j+1);
			degreeControlSpREAD(*ite,ASC,count); //近傍nodeの次数個数を昇順で選び
		}
	}

	writeFile.close();
	return 0;
}
