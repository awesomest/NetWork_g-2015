#include<algorithm>
#include<fstream>
#include<iostream>
#include<sstream>
#include<vector>
#include<time.h>
#include<stdlib.h>

using namespace std;

void make_graph(int i, int v, int e, string name);
string fileName(int i,string name);

int main(){

  srand((unsigned int)time(NULL));

  int v,e;
  int s;
  string name;

  cout << "ファイル名を入力して下さい。" << endl;
  cin >> name;
  cout << "頂点数と辺数を入力して下さい。" << endl;
  cin >> v >> e;
  cout << "グラフの数を入力して下さい。" << endl;
  cin >> s;
  
  for(int i=0 ; i<s ; i++)
    {
      make_graph(i+1,v,e,name);
      cout << "finish graph:" << i+1 << endl;
    }
  return 0;
}

void make_graph(int a, int b, int c, string name){
  vector<int> array;
  int t;
  ofstream graph;
  graph.open(fileName(a,name).c_str());
  graph << b << " " << c << endl;
  array.push_back(0);
  array.push_back(1);
  graph << 0 << " " << 1 << endl;
  for(int i=1 ; i<c ; i++)
    {
      t = rand()%(2*i);
      array.push_back(array[t]);
      array.push_back(i+1);
      graph << array[t] << " " << i+1 << endl;
    }
  graph.close();
  return;
}

string fileName(int i, string name){
  ostringstream ost;
  string No;
  ost << i;
  No = ost.str();
  return name+"_"+No+".txt";
}
