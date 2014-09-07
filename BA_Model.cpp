#include<iostream>
#include<fstream>
#include<ctime>
#include<cstdlib>
#include<cstring>
#include<string>

#define networkSize 1000000
#define sumInitialNodes 2
#define count 1000

using namespace std;

unsigned int degree[networkSize];

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
  unsigned int sumDegree=sumInitialNodes*(sumInitialNodes-1);
  double p;

  //完全グラフを
  for(int node1=0;node1<sumInitialNodes;node1++)
    for(int node2=node1+1;node2<sumInitialNodes;node2++)
      {
	degree[node1]++;
	degree[node2]++;
       file<<node1<<" "<<node2<<endl;
      }

  for(int additionNode=sumInitialNodes;additionNode<networkSize;additionNode++)
    {
      while(!degree[additionNode]) //孤立点ならないように
	{
        /*****************************
				問題点:追加する頂点の次数が固定でない
        解決策:頂点数分のサイズの要素に次数の累積和を保存。
               乱数の総次数のmodを得る。
               その値がどの頂点の持つ次数なのかによって連結頂点を決定。
        *****************************/
	  for(int neighborSelection=0;neighborSelection<additionNode;neighborSelection++)
	    {
	      p=(double)degree[neighborSelection]/sumDegree;
	      if(p>((double)rand()/RAND_MAX))
		{
		  degree[neighborSelection]++; 
		  degree[additionNode]++;
		  file<<additionNode<<" "<<neighborSelection<<endl;
		}
	    }
	}
      sumDegree+=degree[additionNode]*2;
    }

  file.close();
 }

int main()
{
  srand(time(NULL));
  
  for(int i=0;i<count;i++)
    BA_model(i);
    
  return 0;
}
