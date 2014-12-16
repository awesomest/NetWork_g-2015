#include<algorithm>
#include<fstream>
#include<iostream>
#include<queue>
#include<vector>
#include<map>
#include<time.h>
#include<stdlib.h>
#include<math.h>

using namespace std;
#define MAX_N (10000)//最後に「;」は不要
#define read 1
#define write 2
#define TIMES 50
void make_scale(int i);
int decide_start();
int transfer_information_No(int start,int cnt);
int transfer_information_IN(int start,int cnt);
int INVERSE_choose_neighbor_node(int source);
int choose_neighbor_node(int source);
string fileName_No(int i,int method, string name, int digit);
string fileName_IN(int i,int method, string name, int digit);
string fileName(int i,int method, string name, int digit);

struct vertex{
  int number;
  vector<vertex*>edge;
};

vector<vertex> G(MAX_N+1);
vector<int>degree_distribution(MAX_N+1);
vector<int>Information_Table(MAX_N+1);
vector<int>START(TIMES);

int main(){

  //グラフのループ
  for(int i=0;i<100;i++)
    {
      time_t s,e;
      time(&s);
      make_scale(i+1);
      int cnt =1;
      //１つのグラフに対してのループ No controlから
      for(int j=0;j<50;j++,cnt++)
	{
	  int start; 
	  start=decide_start();
	  srand(j+1);
          cout<<"step "<<transfer_information_No(start,cnt)<<endl;
          time(&e);
	  cout<<"No control time passed "<<e-s<<endl;
	  cout<<"FINISH"<<j+1<<endl;
	}
      cnt=1;
      //Inverse control
      for(int j=0;j<50;j++,cnt++)
	{
	  int start; 
	  start=decide_start();
	  srand(j+1);
          cout<<"step "<<transfer_information_IN(start,cnt)<<endl;
	  time(&e);
	  cout<<"Inversse control time passed "<<e-s<<endl;
	  cout<<"FINISH"<<j+1<<endl;
	}
    }

  return 0;
}

string fileName(int i,int method, string name="",int digit=10)
{
  string No;
  while(i)
    {
      char n='0'+(i%digit);
      No=n+No;
      i/=digit;
    }
  if(method == read)
    {
      return "BA"+No+".dat";
    }
  else
    {
      return name+".csv";
    }
}

string fileName_No(int i,int method, string name="",int digit=10)
{
  string No;
  while(i)
    {
      char n='0'+(i%digit);
      No=n+No;
      i/=digit;
    }
  if(method == read)
    {
      return "BA"+No+".dat";
    }
  else
    {
      return name+".csv";
    }
}
string fileName_IN(int i,int method, string name="",int digit=10)
{
  string No;
  while(i)
    {
      char n='0'+(i%digit);
      No=n+No;
      i/=digit;
    }
  if(method == read)
    {
      return "BA"+No+".dat";
    }
  else
    {
      return name+".csv";
    }
}

void make_scale(int a){
  ifstream graph;
  vector<int> array;
  graph.open(fileName(a,read).c_str());
  
  for(int i=0;i<MAX_N;i++){
    int s,t;
    graph>>t>>s;
    array.push_back(s);
    array.push_back(t);
    G[s].edge.push_back(&G[t]);
    G[t].edge.push_back(&G[s]);  
  }

  //頂点番号を入力
  for(int i=0;i<MAX_N+1;i++){
    G[i].number=i;
  }
  
    //次数を入力 
  for(int e=0;e<2*MAX_N;e++){
    degree_distribution[array[e]]++;
  }
  graph.close();
  return;
}

//グラフ上からランダムにstartノードを選択する
int decide_start()
{
  srand((unsigned int)time(NULL));
  int s;
  s=rand()%(MAX_N+1);
  START.push_back(s);
  return s;
}

//No controlで情報伝播させる
int transfer_information_No(int start, int cnt){

  ofstream NoControl;
  NoControl.open(fileName_No(cnt,write,"SAMPLE_NO").c_str(),ios::app);
  Information_Table[start] = 1;

  if(cnt == 1)
    {
      for(int i=0;i<=100;i++)
	{
	  NoControl<<i<<"%"<<",";
	}
      NoControl<<endl;
    }

  //file出力に使用する3つの変数
  int KAISUU;
  int current;
  int prev = 0;
  
 
  int step=0;
  int Received_node=1;
  int target;
  queue<int> source;
  queue<int> new_source;
  //一番最初の処理
  source.push(start);
  Information_Table[start] = 1;

  while( Received_node < MAX_N+1)
    {
      step++;
      //0%を0にする
      if(step==1)
	{
	  NoControl<<0<<",";
	}

      int size = source.size();
      for(int i=0;i<size;i++)
	{
	  target = choose_neighbor_node(source.front());
	  if(Information_Table[target] == 0)
	    {
	      Received_node++;
	      new_source.push(target);
	    }
	  source.push(source.front());
	  source.pop();
	}
     
      //情報更新する
      while(!new_source.empty())
	{
	  Information_Table[new_source.front()] = 1;
	  source.push(new_source.front());
	  new_source.pop();
	}
      
      //source nodeの中から,全隣接ノードが情報を持っていると消す
      size = source.size();
      for(int i=0;i < size; i++)
	{
	  int handan = 1;

	  for(int j=0;j<degree_distribution[source.front()];j++)
	    {
	      //隣接ノードが情報を持っていなければ,ループを抜ける
	      if(Information_Table[(*G[source.front()].edge[j]).number] == 0)
		{
		  handan =0;
		  break;
		}
	    }
	  //全隣接ノードが情報を持っていれば,sourceから外す。
	  if(handan)
	    {
	      source.pop();
	    }
	  else
	    {
	      //全隣接ノードのうち一つでも情報を持っていなければ、再びsourceノードにいれる。
	      source.push(source.front());
	      source.pop();
	    }
	}

 //ここにデータをとる細工をする 初めて１％を超えたstepを記録する
    current = Received_node/100;
    if((current-prev)!=0)
      {
	int temp;
	temp = current-prev;
	if(temp == 1)
	  {
	    NoControl<<step<<",";
	    prev = current;
	  }
	else
	  {
	    for(int j=0;temp > 0;j++,temp--)
	      {
		if(temp == 1)
		  {
		    NoControl<<step<<",";
		  }
		else
		  {
		    NoControl<<",";
		  }
	      }
	    prev = current;
	  }
      }

    cout<<"step "<<step<<" Received_node "<<Received_node<<endl;
    if(step == 1000)
      {
	break;
      }

    }//whileはここまで

  NoControl<<endl;
  
  //初期化,念のため
  while(!source.empty())
    {
      source.pop();
    }
  while(!new_source.empty())
    {
      new_source.pop();
    }
  
  fill(Information_Table.begin(),Information_Table.end(),0);
  NoControl.close();
  return step;  
}

//No Control
int choose_neighbor_node(int source){
  vector<int> neighbor1;
  
  //NoControl verはこれでいいはず。選択される機会が次数分高いだけ。。
  //neighbor1にsource_nodeの連結頂点の番号を入力
  for(int i=0;i<G[source].edge.size();i++){
    neighbor1.push_back((*G[source].edge[i]).number);
  }
  int target_node;
  target_node=neighbor1[rand()%(neighbor1.size())];
  return target_node;
}

int transfer_information_IN(int start, int cnt){
 
  ofstream NoControl;
  NoControl.open(fileName_IN(cnt,write,"SAMPLE_INVERSE").c_str(),ios::app);
  Information_Table[start]= 1;

  if(cnt == 1)
    {
      for(int i=0;i<=100;i++)
	{
	  NoControl<<i<<"%"<<",";
	}
      NoControl<<endl;
    }

  //file出力につかう３つの変数
  int KAISUU;
  int current;
  int prev = 0;
  

  int step=0;
  int Received_node = 1;
  int target;
  queue<int> source;
  queue<int> new_source;
  //一番最初の処理
  source.push(start);
  Information_Table[start] = 1;

  while( Received_node < MAX_N+1)
    {
      step++;
      if(step == 1)
	{
	  NoControl<<0<<",";
	}

      int size = source.size();
      for(int i=0;i<size;i++)
	{
	  target = INVERSE_choose_neighbor_node(source.front());
	  if(Information_Table[target] == 0)
	    {
	      Received_node++;
	      new_source.push(target);
	    }
	  source.push(source.front());
	  source.pop();
	}

      //情報更新する
      while(!new_source.empty())
	{
	  Information_Table[new_source.front()] = 1;
	  source.push(new_source.front());
	  new_source.pop();
	}

      //source nodeの中から,全隣接ノードが情報を持っていると消す
      size = source.size();
      for(int i=0;i < size; i++)
	{
	  int handan=1;


	  for(int j=0;j < degree_distribution[source.front()];j++)
	    {
	      //隣接ノードが情報を持っていなければ,handanを0にする
	      if(Information_Table[(*G[source.front()].edge[j]).number] == 0)
		{
		  handan = 0;
		}
	    }
	  //全隣接ノードが情報を持っていれば,sourceから外す。
	  if(handan)
	    {
	      source.pop();
	    }
	  else
	    {
	      //全隣接ノードのうち一つでも情報を持っていなければ、再びsourceノードにいれる。
	      source.push(source.front());
	      source.pop();
	    }
	}

	  //ここにデータをとる細工をする
      current = Received_node/100;
      if((current-prev)!=0)
	{
	  int temp;
	  temp = current-prev;
	  if(temp == 1)
	    {
	      NoControl<<step<<",";
	      prev = current;
	    }
	  else
	    {
	      for(int j=0;temp > 0;j++,temp--){
		if(temp == 1)
		  {
		    NoControl<<step<<",";
		  }
		else
		  {
		    NoControl<<",";
		  }
	      }
	      prev = current;
	    }
	} 
      cout<<"step "<<step<<" Received_node "<<Received_node<<endl;
      if(step == 1000)
	{
	  break;
	}
      
    }//whileはここまで
  
  NoControl<<endl;
  
  //初期化,念のため  
  while(!source.empty())
    {
      source.pop();
    }
  while(!new_source.empty())
    {
      new_source.pop();
    }
  
  fill(Information_Table.begin(),Information_Table.end(),0);
  NoControl.close();
  return step;  
}

map<int, vector<int> > neighbor2_cache;
int INVERSE_choose_neighbor_node(int source){
  int gcm;
  int min_koubai;
  int total_seki=0;

  vector<int> neighbor2;
  map<int, vector<int> >::iterator it2 = neighbor2_cache.find(source);
  if (it2 != neighbor2_cache.end()) {
    neighbor2 = (*it2).second;
    return neighbor2[rand()%(neighbor2.size())];
  }

  vector<int> neighbor1;//隣接頂点の番号を入力
  //neighbor1にsource_nodeの連結頂点の番号を入力
  for(int i=0;i<G[source].edge.size();i++){
    neighbor1.push_back((*G[source].edge[i]).number);
  }
  
  for(int i=0;i<neighbor1.size();i++){
    total_seki += degree_distribution[neighbor1[i]];
  }
  //GCLをこの下にのせる
  vector<int>::iterator the_array;
  vector<int>::iterator the_array_end;
  the_array = neighbor1.begin();
  the_array_end = (neighbor1.end()-1);
  
  if(the_array == the_array_end)
    {
      return *the_array;
    }    
  if(the_array != the_array_end){
    vector<int> temp_array;
    for(int i=0;i<neighbor1.size();i++){
      temp_array.push_back(total_seki / degree_distribution[neighbor1[i]]);
      //temp_array.push_back((int)sqrt((double)(total_seki / degree_distribution[neighbor1[i]])) + 10);
    }
    for(int i=0;i<neighbor1.size();i++){
      for(int j=0;j<temp_array[i];j++){
        neighbor2.push_back(neighbor1[i]);
      }
    }

    neighbor2_cache[source] = neighbor2;
    
    int target_node;
    target_node = neighbor2[rand()%(neighbor2.size())];
    return target_node;
  }
}

