#include<iostream>
#include<fstream>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<cstring>
#include<queue>
#include<algorithm>
#include<map>

#define networkSize 1000000
#define useNetworkData 100
#define existData 1000
#define spreadCount 100
#define read 1
#define write 2
#define desc 1
#define asc 2

using namespace std;

vector<vector<int> > network(networkSize);
bool informReceived[networkSize];
int degree[networkSize];

struct TransferInform
{ int source,target; };

struct NodeInform
{ int node,degree; };

bool degree_asc(NodeInform n1,NodeInform n2)
{ return n1.degree<n2.degree; }

bool degree_desc(NodeInform n1,NodeInform n2)
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

	if(type==write)
		return name+"_time"+No+".dat";
	else
		return "data"+No+".dat";
}

void rebuildNetwork(int No)
{
	ifstream readFile;
	int node1,node2;

	memset(degree,0,sizeof(degree));

	for(int i=0;i<networkSize;i++)
		network[i].clear();

	readFile.open(fileName(No,read).c_str());
	while(readFile>>node1>>node2)
	{
		network[node1].push_back(node2);
		network[node2].push_back(node1);
		degree[node1]++;
		degree[node2]++;
	}

	readFile.close();
}

double totalInverseWeight(vector<vector<int> > &edge,int source)
{
	double weight=0;
	for(int i=0;i<edge[source].size();i++)
		weight+=1.0/network[edge[source][i]].size();

	return weight;
}

void noControlSpread(int source,int No)
{
	//ファイルを作り
	ofstream writeFile;
	writeFile.open(fileName(No,write,"NC").c_str());

	vector<vector<int> > edge(networkSize);
	vector<TransferInform> transferList;
	TransferInform transfer;
	int target,sumInformReceived=1,times=0;

	//情報獲得の確認用の配列を初期化
	memset(informReceived,0,sizeof(informReceived));

	//edgeの初期化
	for(int node=0;node<networkSize;node++)
		copy(network[node].begin(),network[node].end(),back_inserter(edge[node]));

	queue<int> sourceList;
	sourceList.push(source);
	informReceived[source]=1;

	//情報の拡散
	while(sumInformReceived!=networkSize)
	{
		int informSpreadSource=sourceList.size();
		times++;

		while(informSpreadSource--)
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
		/*****************
			このfor文はいらないのでは？
			Inverse Controllのプログラムの名残？
			あとで考える
		*****************/

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

void inverseControlSpread(int source,int No)
{
	//ファイルを作り
	ofstream writeFile;
	writeFile.open(fileName(No,write,"IC").c_str());

	vector<vector<int> > edge(networkSize);
	vector<double> targetSelect;
	vector<TransferInform> transferList;
	TransferInform transfer;
	int target,sumInformReceived=1,times=0;

	//情報獲得の確認用の配列を初期化
	memset(informReceived,0,sizeof(informReceived));

	//edgeの初期化
	for(int node=0;node<networkSize;node++)
		copy(network[node].begin(),network[node].end(),back_inserter(edge[node]));

	queue<int> sourceList;
	sourceList.push(source);
	informReceived[source]=1;

	//情報の拡散
	while(sumInformReceived!=networkSize)
	{
		int informSpreadSource=sourceList.size();
		times++;
		while(informSpreadSource--)
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

void degreeControlSpread(int source,int method,int No)
{
	//ファイルを作り
	ofstream writeFile;

	if(method==desc) 
		writeFile.open(fileName(No,write,"DC_desc").c_str());
	else
		writeFile.open(fileName(No,write,"DC_asc").c_str());

	vector<vector<NodeInform> > edge(networkSize);
	vector<TransferInform> transferList;
	TransferInform transfer;
	int target,neighbor,sumInformReceived=1,times=0;

	//情報獲得の確認用の配列を初期化
	memset(informReceived,0,sizeof(informReceived));

	//edgeの初期化
	for(int i=0;i<networkSize;i++)
	{
		edge[i].resize(network[i].size());

		for(int j=0;j<network[i].size();j++)
		{
			edge[i][j].node=network[i][j];
			edge[i][j].degree=degree[network[i][j]];
		}
	}

	//descかascのソート
	if(method==desc)
	{
		for(int i=0;i<networkSize;i++)
			sort(edge[i].begin(),edge[i].end(),degree_desc);
	}
	else
	{
		for(int i=0;i<networkSize;i++)
			sort(edge[i].begin(),edge[i].end(),degree_asc);
	}

	queue<int> sourceList;
	sourceList.push(source);
	informReceived[source]=1;

	//情報の拡散
	while(sumInformReceived!=networkSize)
	{
		int informSpreadSource=sourceList.size();
		times++;
		while(informSpreadSource--)
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
	vector<int> firstSource,useData;
	int count=1;
	ofstream writeFile;
	writeFile.open("network and first source list.txt");

	//使うnetworkのデータをラダンムで選び
	for(int i=0,number;i<useNetworkData;i++) 
	{ 
		do
		{
			number=(rand()%existData)+1;
		}while(find(useData.begin(),useData.end(),number)!=useData.end());

		useData.push_back(number);
	}

	/*******************************
		使うネットワークも100個の情報源も
		setに入れて重複を防ぐ
	 *******************************/

	// 100個のネットワークに対して
	for(int i=0;i<useNetworkData;i++)
	{
		firstSource.clear();
		rebuildNetwork(useData[i]);

		writeFile<<useData[i]<<endl; //使ったnetworkのデータを記録

		//最初のsourceをランダムで選び
		// 各ネットワークに100個ずつの情報源を与える
		for(int j=0,node=0;j<spreadCount;j++)
		{
			do
			{
				node=rand()%networkSize;
			}while(find(firstSource.begin(),firstSource.end(),node)!=firstSource.end());

			firstSource.push_back(node);
			writeFile<<node<<" "; //選ばれたsourceを記録
		}

		writeFile<<endl;

		for(int j=0;j<spreadCount;j++,count++)
		{
			srand(j+1);
			noControlSpread(firstSource[j],count); //近傍nodeをrandomで選び
			srand(j+1);
			inverseControlSpread(firstSource[j],count); //確率で次数が小さい近傍nodeを選ぴ安いように
			srand(j+1);
			degreeControlSpread(firstSource[j],desc,count); //近傍nodeの次数個数を降順で選び
			srand(j+1);
			degreeControlSpread(firstSource[j],asc,count); //近傍nodeの次数個数を昇順で選び
		}
	}

	writeFile.close();
	return 0;
}
