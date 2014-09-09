#include<iostream>
#include<fstream>
#include<ctime>
#include<cstdlib>
#include<cstring>
#include<string>
#include<set>

#define NETWORKSIZE 1000000
#define SUMINITIALNODES 2
#define COUNT 1000
#define NEW_EDGE 2

using namespace std;

unsigned int degree[NETWORKSIZE];

string filename(int i,int digit=10)
{
	string No;
	while(i!=0)
	{
		char n='0'+(i%digit);
		No=n+No;
		i/=digit;
	}

	return "data"+No+".dat";
}

void BA_model(int i)
{ 
	ofstream file;
	file.open(filename(i+1).c_str());

	memset(degree,0,sizeof(degree));
	unsigned int sumDegree=SUMINITIALNODES*(SUMINITIALNODES-1);

	//完全グラフを
	for(int node1=0;node1<SUMINITIALNODES;node1++)
		for(int node2=node1+1;node2<SUMINITIALNODES;node2++)
		{
			degree[node1]++;
			degree[node2]++;
			file<<node1<<" "<<node2<<endl;
		}

	for(int additionNode=SUMINITIALNODES;additionNode<NETWORKSIZE;additionNode++)
	{
		set<int> connected;
		while(connected.size() < NEW_EDGE) //孤立点ならないように
		{
			//修正済み
			/*****************************
				問題点:追加する頂点の次数が固定でない
				解決策:頂点数分のサイズの要素に次数の累積和を保存。
				乱数の総次数のmodを得る。
				その値がどの頂点の持つ次数なのかによって連結頂点を決定。
			 *****************************/
			unsigned int r = rand() % sumDegree;

			int sum = 0;
			for(int neighborSelection=0;neighborSelection<additionNode;neighborSelection++)
			{
				sum += degree[neighborSelection];
				if (sum >= r) {
					connected.insert(neighborSelection);
				}
			}
		}
		set<int>::iterator it = connected.begin();
		for ( ; it!=connected.end(); it++) {
			degree[*it]++;
			degree[additionNode]++;
			file << additionNode << " " << *it << endl;
		}
		connected.clear();
		sumDegree+=degree[additionNode]*2;
	}

	file.close();
}

int main()
{
	srand(time(NULL));

	for(int i=0;i<COUNT;i++)
		BA_model(i);

	return 0;
}
