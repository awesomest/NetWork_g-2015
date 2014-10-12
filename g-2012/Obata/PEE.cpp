#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
#define read 1
#define write 2

string fileName(int i,int method, string name, int digit);
int max(vector<int>::iterator a,vector<int>::iterator b);

int main(){
  vector<int>D;
  vector<int>::iterator a,b;
  ifstream A,B;
  ofstream EQ;
  for(int i=0;i<100;i++){
    A.open(fileName(i+1,read,"Max_Degree",10).c_str());
    int t;
    A>>t;
    D.push_back(t);
    A.close();
  }
  int SAIDAI;
  a = D.begin();
  b = D.end()-1;
  SAIDAI = max(a,b);
  cout<<"SAIDAI "<<SAIDAI<<endl;
  
  EQ.open(fileName(0,write,"A_PE",10).c_str());
  //1s–Ú
  for(int i=0;i<SAIDAI;i++){
    EQ<<i+1<<",";
  }
  EQ<<endl;
  for(int i=0;i<100;i++){
    B.open(fileName(i+1,read,"P_KAI_Pe_Analyze",10).c_str());
    for(int j=0;j<D[i];j++){
      double temp;
      B>>temp;
      if(temp != 0){
	EQ<<temp<<",";
      }
      else
	{
	  EQ<<",";
	}
    }
    EQ<<endl;
    B.close();
  }
  return 0;
}
int max(vector<int>::iterator a,vector<int>::iterator b){
  int max = -1;
  while(a != b){
    if((*a)>max){
      max = (*a);
    }
    a++;
  }
  return max;
}
string fileName(int i,int method,string name,int digit){
  string No;
  while(i){
    char n='0'+(i%digit);
    No=n+No;
    i/=digit;
  }
  if(method == read){
    return name+"_"+No+".dat";
  }
  else{
    return name+"_"+".csv";
  }
}
