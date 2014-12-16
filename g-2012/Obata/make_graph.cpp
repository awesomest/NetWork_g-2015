#include<algorithm>
#include<fstream>
#include<iostream>
#include<vector>
#include<time.h>
#include<stdlib.h>
using namespace std;
#define MAX_N (1000000)
#define read 1
#define write 2

void make_scale(int i);
string fileName(int i,int method,string name,int digit);

struct vertex{
  int number;
  vector<vertex*>edge;
};

vector<vertex> G(MAX_N+1);
vector<int>degree_distribution(MAX_N+1);

int main(){
  for(int i=0;i<100;i++){
  make_scale(i+1);
  cout<<"finish graph "<<i<<endl;
  }
  return 0;
}

void make_scale(int a){
  srand((unsigned int)time(NULL));
  vector<int> array;
  ofstream graph;
  graph.open(fileName(a,write,"BA",10).c_str());
  for(int i=0;i<MAX_N;i++){
    if(i==0){
      array.push_back(0);
      array.push_back(1);
      graph<<0<<" "<<1<<endl;
    }
    else{
      int t;
      t=rand()%(2*i);
      array.push_back(array[t]);
      array.push_back(i+1);
      graph<<array[t]<<" "<<i+1<<endl;
    }
  }
  graph.close();
  return ;
}
string fileName(int i,int method,string name,int digit){
  string No;
  while(i){
    char n='0'+(i%digit);
    No=n+No;
    i/=digit;
  }
  if(method == read){
    cout<<"are?"<<endl;
  }
  else{
    return name+"_"+No+".dat";
  }
}
