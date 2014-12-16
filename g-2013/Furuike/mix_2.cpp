#include<algorithm>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<iostream>
#include<sstream>
#include<queue>
#include<stdio.h>
#include<time.h>

using namespace std;

struct vertex{
  int number;
  vector<vertex*> edge;
};

int read_degree(string name);
void read_graph(std::vector<vertex>&G,std::vector<int>&degree_distribution,int vertex_total,string name);
int start_node(int v);
int transfer(std::vector<vertex>&G,std::vector<int>&degree_distribution,int start,int vertex_total,int variety,string name2);
int choose_node(std::vector<vertex>&G,std::vector<int>&degree_distribution,std::vector< vector<int> > &no,int source);
int high_choose_node(std::vector<vertex>&G,std::vector<int>&degree_distribution,std::vector< vector<int> >&high,int *high_degree_total,int source);
int low_choose_node(std::vector<vertex>&G,std::vector<int>&degree_distribution,std::vector< vector<long double> >&low,int source);
long long int lcm(long long int x,long int y);
long long int gcd(long long int x,long int y);

int main(int argc,char * argv[]){

  int vertex_total;
  int start;

  vertex_total = read_degree(argv[1]);     //頂点数を読み込む処理。
  //cout << "頂点数： " << vertex_total << endl;

  //データ構造宣言。
  vector<vertex> G(vertex_total);
  vector<int> degree_distribution(vertex_total);

  read_graph(G,degree_distribution,vertex_total,argv[1]);     //グラフを読み込む処理。
  /*
  cout << "頂点番号確認" << endl;
  for(int i=0 ; i<vertex_total ; i++)
    {
  cout<< i << ": " << G[i].number << endl;
    }

  cout << "次数分布確認" << endl;
  for(int i=0 ; i<vertex_total ; i++)
    {
      cout << i << ": " << degree_distribution[i] << endl;
    } 
  */
  srand(atoi(argv[3]));     //乱数生成の種を宣言。

  start = start_node(vertex_total);     //スタートノードを決める。
  //cout << "start: " << start << endl;

  int variety = 0;     //伝播方法区別。
  cout << endl << "等確率の伝播開始" << endl;
  transfer(G,degree_distribution,start,vertex_total,variety,argv[2]);
  cout  << "等確率の伝播終了" << endl << endl;;
 
  variety = 1;
  cout << "次数が高い頂点優先の伝播開始" << endl; 
  transfer(G,degree_distribution,start,vertex_total,variety,argv[2]);
  cout << endl << "次数が高い頂点優先の伝播終了" << endl << endl;;
 
  variety = 2;
  cout << "次数が低い頂点優先の伝播開始" << endl;
  transfer(G,degree_distribution,start,vertex_total,variety,argv[2]);
  cout << "次数が低い頂点優先の伝播終了" << endl << endl;
  return 0;
}

//頂点数を読み込む関数。
int read_degree(string name){

  ifstream graph;
  graph.open(name.c_str());

  if(graph.fail())
    {
      cout << "読み込み失敗!!!" << endl;
    }

  int vertex,edge;

  for( int i=0 ; i<1; i++ )
    {
      graph >> vertex >> edge;
    }

  graph.close();
  return vertex;
}

//グラフを読み込む関数。
void read_graph(std::vector<vertex>&G,std::vector<int>&degree_distribution,int vertex_total,string name){
  
  //頂点番号を入力。
  for( int i=0 ; i<vertex_total ; i++)
    {
      G[i].number = i;
    }  

  ifstream graph;
  graph.open(name.c_str());

  for( int i=0 ; i<vertex_total ; i++ )
    {
      int c, d;
      graph >> c >> d;

      if( i )
	{
	  //cout << c << " " << d << endl;
	    
	  G[c].edge.push_back(&G[d]);
	  G[d].edge.push_back(&G[c]);
	  //次数を入力。
	  degree_distribution[c]++;
	  degree_distribution[d]++;   
	}
    }
  
  graph.close();
  return;
}

//スタートノードを決める関数。
int start_node(int vertex_total){
  int s;
  s = rand() % vertex_total;
  return s;
}

//伝播関数。
int transfer(std::vector<vertex>&G,std::vector<int>&degree_distribution,int start,int vertex_total,int variety,string name2)
{
  ofstream out;
  out.open(name2.c_str(),ios::app);

  out << start << " ";
  out << degree_distribution[start] << " ";

  vector<int> information(vertex_total);     //情報テーブル。

  information[start] = 1;
  int information_total = 1;
  int target;
  int step = 0;
  int percent;
  int a = 0;
  int b = 1;

  queue<int> source;
  queue<int> new_source;

  vector< vector <int> > no(vertex_total);     //等確率で使うvector 。
  vector< vector <int> > high(vertex_total);     //次数高い優先で使う vector 。
  int high_degree_total[vertex_total];     //次数高い優先で使う配列。
  vector< vector <long double> > low(vertex_total);     //次数低い優先で使うvector。

  source.push(start);
 
  while( information_total < vertex_total )
    {
      //cout << "-----step: " << step << "-----" << endl; 
      step++;

      //cout << "source.size: " << source.size() << endl;     //ソースノードの数確認。
      for(int i=0 ; i<source.size() ; i++)
	{
	  //cout << "source.number: " << source.front() << endl;
	  //ターゲットノードを決める。
	  if(variety == 0)
	    target = choose_node(G,degree_distribution,no,source.front());
	  else if(variety == 1)
	    target = high_choose_node(G,degree_distribution,high,high_degree_total,source.front());
	  else if(variety == 2)
	    target = low_choose_node(G,degree_distribution,low,source.front());

	  //cout << "target_node: " << target << endl;

	  if(information[target] == 0 )     //ターゲットノードが情報を持ってなかったら。
	    {
	      information_total++;     //情報を持ってるノードが１増える。
	      new_source.push(target);     //ひとまず入れとく。
	    }
	  
	  source.push(source.front());     //末尾へ追加。
	  source.pop();     //先頭からの取り出し。
	}

      //cout << "total: " << information_total << endl;
      
      //新しくソースノードとなったノードを source に入れる。
      while(!new_source.empty())     //空になるまで。
	{
	  information[new_source.front()] = 1;     // 情報を持っている に変える。
	  source.push(new_source.front());     //ソースノードに加える。
	  new_source.pop();     //捨てる。
	}
      
      //ソースノードの全隣接頂点が情報を持っていると消す。
      for(int i=0 ; i<source.size() ; i++)
	{
	  int judge = 1;
	  
	  for(int j=0 ; j<degree_distribution[source.front()] ; j++)     //次数の数だけまわす。
	    {
	      if(information[(*G[source.front()].edge[j]).number] == 0)     //情報をもっていない頂点が見つかったら。
		{
		  judge = 0;
		  break;
		}
	    }
	  
	  if(judge)     //全隣接頂点が情報を持っていれば、source から外す
	    {
	      source.pop();
	    }
	  else     //情報を持っていないノードがあるなら、再び source ノードに入れる
	    {
	      source.push(source.front());
	      source.pop();
	    }
	}
  
      //データを記録。
      //１％毎にその step を記録。 
      
      percent = information_total / 100;
      
      if(percent > a)
	{
	  if(percent == b)
	    {
	      out << step << " ";
	      b++;
	      a = percent;
	    }
	  else
	    {
	      for(int j=a ; j<=percent ; j++)
		{
		  if(j == b)
		    {
		      out << step << " ";
		      b++;
		    }
		}
	      a = percent;
	    }
	}
    }
  
  //初期化。
  while(!source.empty())
    {
      source.pop();
    }
  while(!new_source.empty())
    {
      new_source.pop();
    }

  out << endl;

  // fill は指定され範囲内の要素を、指定した要素に書き換え。
  fill(information.begin(),information.end(),0);
  out.close();
  return step-1;
}

//等確率でターゲットノードを選択。
int choose_node(std::vector<vertex>&G,std::vector<int>&degree_distribution,std::vector< vector<int> >&no,int source){
 
 int target_node;

 if( !no[source].empty() )
   {
     target_node = no[source][rand()%(no[source].size())];
     return target_node;
     
   }else {
   
     //ソースノードの次数分まわす。
     //隣接頂点を high[source] に入れる。
     for(int i=0 ; i<G[source].edge.size() ; i++ )
       {
	 no[source].push_back((*G[source].edge[i]).number);
	 //	 cout << "number: " << (*G[source].edge[i]).number << endl;
       }
     
     target_node = no[source][rand()%(no[source].size())];
     return target_node;
   }
}

//次数高い頂点優先のターゲットノードを決める関数。
int high_choose_node(std::vector<vertex>&G,std::vector<int>&degree_distribution,std::vector< vector<int> >&high,int *high_degree_total,int source){

  int degree = 0;
  int degree_total = 0;
  int total = 0;
  int random = 0;
  int target_node = 0;

  if( !high[source].empty() )
    {
      random = rand() % high_degree_total[source];

      for( int i=0 ; i<high[source].size() ; i++ )
	{
	  total += high[source][i];

	  if(random < total)
	    {
	      target_node = (*G[source].edge[i]).number;
	      return target_node;
	    } 
	}

    }else{

    //cout << "source_degree: " << (G[source].edge).size() << endl;     //ソースノードの次数確認。
      
      //隣接頂点の次数を neighbor_degree に入れる。
      for(int i=0 ; i<(G[source].edge).size() ; i++)
	{
	  degree = (*G[source].edge[i]).number;
	  high[source].push_back(degree_distribution[degree]);     //隣接頂点の次数を入れる。
	  
	  //cout << "number :" << (*G[source].edge[i]).number << " " << "次数" << degree_distribution[degree] << endl;     //隣接頂点の頂点番号と次数を確認。
	  
	  degree_total += degree_distribution[degree];
	}
      
      high_degree_total[source] = degree_total;
      //cout << "degree_total: " << degree_total << endl;     //次数の合計を確認。
      
      random = rand() % degree_total;     //範囲内の乱数を生成。
      //cout << "random: " << random << endl;
      
      //ターゲットノードを決める処理。
      for(int i=0 ; i<high[source].size() ; i++)
	{
	  total += high[source][i];
	  
	  if(random < total)
	    {
	      target_node = (*G[source].edge[i]).number;
	      return target_node;
	    }
	}
    }
}

//次数低い頂点優先のターゲットノードを決める関数。
int low_choose_node(std::vector<vertex>&G,std::vector<int>&degree_distribution,std::vector< vector<long double> >&low,int source){ 

  long double random = 0;
  long double total = 0;
  
  long int target_node = 0;
  long double a = 0;
  long int b = 0;
  long int c = 0;
  long int d = 0;
  long long int neighbor_degree_lcm = 1;
  
  vector<long int> neighbor3_1;
  vector<long int> neighbor_degree;

  if( !low[source].empty() )
    {
      random = 1.0 * rand() / RAND_MAX;

      for(int i=0 ; i<low[source].size() ; i++)
	{
	  total += low[source][i];

	  if(random < total)
	    {
	      target_node = (*G[source].edge[i]).number;
	      return target_node;
	    }
	}

    }else{
      
      //隣接頂点の次数を neighbor_degree に入れる。
      for(int i=0 ; i<(G[source].edge).size() ; i++)
	{
      d = (*G[source].edge[i]).number;
      neighbor_degree.push_back(degree_distribution[d]);
      
      //cout << "number: " << d << " " << "次数: " << degree_distribution[d] << endl;
	}

      //cout << "個数: " << neighbor_degree.size() << endl;

      //隣接頂点の次数の最小公倍数を求める。
      for(int i=0 ; i<neighbor_degree.size() ; i++)
	{
	  //cout << neighbor_degree_lcm << " " << neighbor_degree[i] << endl;
	  neighbor_degree_lcm = lcm(neighbor_degree_lcm,neighbor_degree[i]);     //最小公倍数を求める処理。
	}

      //cout << "最小公倍数: " << neighbor_degree_lcm << endl;
      
      //隣接頂点の次数に最小公倍数を掛けた数を neighbor3_1 に入れる。
      for(int i=0 ; i<neighbor_degree.size() ; i++)
	{
	  c = neighbor_degree_lcm / neighbor_degree[i];     //分子の計算。
	  neighbor3_1.push_back(c);
	}
      
      //割る数の計算。
      for(int i=0 ; i<neighbor_degree.size() ; i++)
	{
	  b += (neighbor_degree_lcm / neighbor_degree[i]);     //分母の計算。
	}
      
      // neighbor3_2 に neighbor3_1 / b の計算結果を入れる。
      for(int i=0 ; i<neighbor3_1.size() ; i++)
	{     
	  a = 1.0 * neighbor3_1[i] / b;     //各頂点が選ばれる確率の計算。    
	  low[source].push_back(a);
	}
 
      random = 1.0 * rand() / RAND_MAX;     //0~1までの乱数を生成。
      //cout << "random:" << random << endl;
      
      for(int i=0 ; i<low[source].size() ; i++ )
	{
	  total += low[source][i];
      
	  if(random < total)
	    {
	      target_node = (*G[source].edge[i]).number;
	      return target_node;
	    }
	}
      neighbor3_1.clear();
      neighbor_degree.clear();
    }
}

//最大公倍数を求める関数。
long long int lcm(long long int x, long int y){
  return ( x * y / gcd(x,y) );
}

//最小公約数を求める関数。
long long int gcd(long long int x, long int y){

  long long int r = 0;
  while((r = x % y) != 0 )
    {
      x = y;
      y = r;
    }
  return y;
}
