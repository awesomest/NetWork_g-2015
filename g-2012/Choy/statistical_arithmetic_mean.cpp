#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cstdlib>

#define useData 10000
#define desc 1
#define asc 2

using namespace std;

string fileName(int i,string name="",int digit=10)
{
  string No;
  
  while(i)
    {
      char n='0'+(i%10);
      No=n+No;
      i/=10;
    }

  return name+"_time"+No+".dat";
}

void show(vector<vector<long long> > &table,string name)
{
  ofstream writeFile;
  writeFile.open("statistical(arithmetic mean)_table.csv",ios::app);
  
  writeFile<<name;

  for(int i=0;i<100;i++)
    if(table[i][0]) writeFile<<","<<table[i][0]/table[i][1];
    else writeFile<<",";
 
  writeFile<<endl;

  writeFile.close();
}

void statistical_IC()
{
  vector<vector<long long> > table(500,vector<long long>(2,0));
  ifstream readFile;
  
  for(int i=1,step,totalReceivedNode;i<=useData;i++)
    {
      readFile.open(fileName(i,"IC").c_str());
     
      while(readFile>>step>>totalReceivedNode)
	{
	  if(step==500) break;
	  table[step-1][0]+=totalReceivedNode; 
	  table[step-1][1]++;
	}
      
      readFile.close();
    }

  show(table,"IC");
 }

void statistical_NC()
{
  vector<vector<long long> > table(500,vector<long long>(2,0));
  ifstream readFile;

  for(int i=1,step,set,totalReceivedNode;i<=useData;i++)
    {
      set=1;
      readFile.open(fileName(i,"NC").c_str());

      while(readFile>>step>>totalReceivedNode)
	{
	  if(step==500) break;
	  table[step-1][0]+=totalReceivedNode;
	  table[step-1][1]++;
	}
 
      readFile.close();
    }

  show(table,"NC");
}

void statistical_DC(int method)
{
  vector<vector<long long> > table(500,vector<long long>(2,0));
  ifstream readFile;

  for(int i=1,step,set,totalReceivedNode;i<=useData;i++)
    {
      set=1;
      if(method==asc)
	readFile.open(fileName(i,"DC_asc").c_str());
      else
	readFile.open(fileName(i,"DC_desc").c_str());
      
      while(readFile>>step>>totalReceivedNode)
	{
	  if(step==500) break;
	  table[step-1][0]+=totalReceivedNode;
	  table[step-1][1]++;
	}
      
      readFile.close();
    }

  if(method==asc)
    show(table,"DC_asc");
  else
    show(table,"DC_desc");
}

int main()
{
  ofstream writeFile;
  writeFile.open("statistical(arithmetic mean)_table.csv",ios::app);

  for(int i=0;i<100;i++)
    if((i+1)%10) writeFile<<",";
    else writeFile<<","<<i+1;
  
  writeFile<<endl;
  writeFile.close();
  
  statistical_IC();
  statistical_NC();
  statistical_DC(asc);
  statistical_DC(desc);
  
  return 0;  
}
