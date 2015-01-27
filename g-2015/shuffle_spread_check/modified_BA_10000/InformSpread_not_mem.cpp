//伝播速度限定モデルで情報拡散するプログラム,伝播状況の記憶無し版
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <algorithm>
#include <map>
#include <random>
#include <sstream>

#define read 1
#define write 2
#define desc 1
#define asc 2

using namespace std;

vector< vector<int> > network;//ネットワーク本体のつながり
bool *informReceived;//情報が受け取っているかどうか、添字が頂点番号
int *degree;//頂点の次数、添字が頂点番号
bool *dead;//隣接点すべてが情報を得ているかどうか判別する配列
string FILENAME;//生成するファイルの名前
int NETWORKSIZE,USENETWORKDATA,SPREADCOUNT,EXISTDATA;//使うネットワークのサイズ、データの数、一つのデータに対して何回実験するか、存在しているデータの数
string DIRECTORYNEAME;//データの格納されているディレクトリの名前
/*
struct NodeInform
{ int node,degree; };
*/
/*
bool degree_asc(NodeInform n1,NodeInform n2)//次数比較関数、昇順
{ return n1.degree<n2.degree; }

bool degree_desc(NodeInform n1,NodeInform n2)//次数比較関数、降順
{ return n1.degree>n2.degree; }
*/
//ファイル名を決める関数
string fileName(int number,int type,string name="",string directory="")
{
  ostringstream oss;
  string No;
  oss << number;
  No = oss.str();
  if(type==write){
    //cout << directory +"/"+ name + "/" +name +"_"+FILENAME+ "_time" + No + ".dat" << endl;
    return directory +"/"+ name + "/" +name +"_"+FILENAME+ "_time" + No + ".dat";
  }
  else{
    cout << "./"+directory+"/" + name +"_"+No+".dat" << endl;
    return "./"+directory+"/" + name +"_"+No+".dat";
  }
}
//頂点の隣接情報を記したファイルを読み込みネットワークを構成する関数
void rebuildNetwork(int No)
{
  free(degree);
  ifstream readFile;
  int node1,node2;

  degree = (int*)calloc(NETWORKSIZE,sizeof(int));
  for(int i=0;i<NETWORKSIZE;i++)network[i].clear();

  readFile.open(fileName(No,read,FILENAME,DIRECTORYNEAME).c_str());
  if(readFile.fail()){cout << "can't read network" << endl;return;}//ファイル読み込みが失敗したら抜ける
  //ネットワーク生成
  int a, b;
  readFile >> a >> b;
  while(readFile>>node1>>node2)
    {

      network[node1].push_back(node2);
      network[node2].push_back(node1);
      degree[node1]++;
      degree[node2]++;
    }
  readFile.close();
}

//inversecontorol用の重み計算
double totalInverseWeight(int source)
{
  double weight=0;
  //隣接点すべての重みの和の計算
  for(int i = 0;i < network[source].size();i++){
    weight+=1.0/network[network[source][i]].size();
  }

  return weight;
}
double totalProportionWeight(int source)
{
  double weight=0;
  //隣接点すべての重みの和の計算
  for(int i = 0;i < network[source].size();i++){
    weight+=network[network[source][i]].size();
  }

  return weight;
}
//ランダムに情報の送り先を決める流し方
void noControlSpread(int source,int No)
{
  //実験結果を書き込むファイルを作る
  ofstream writeFile,noise;
  writeFile.open(fileName(No,write,"NC","./informspread_not_mem/"+DIRECTORYNEAME).c_str());
  noise.open(fileName(No,write,"NC_noise","./informspread_not_mem/"+DIRECTORYNEAME).c_str());
  if(writeFile.fail())return;
  int collision;//行き場を失った頂点の数,すでに伝播し終えた頂点に送った回数
  collision=0;

  //vector<vector<int> > edge(NETWORKSIZE);

  mt19937_64 random_engine(No);//実験に使う乱数を生成
  int target,sumInformReceived=1,times=0;

  //情報獲得の確認用の配列を初期化
  informReceived = (bool*)calloc(NETWORKSIZE,sizeof(bool));
  dead = (bool*)calloc(NETWORKSIZE,sizeof(bool));
  queue<int> sourceList;
  sourceList.push(source);
  informReceived[source]=1;
  //情報の拡散
  while(sumInformReceived < NETWORKSIZE)
  {

      int informSpreadSource=sourceList.size();
      times++;
      collision = 0;
      while(informSpreadSource--)
    	{

    	  source=sourceList.front(); sourceList.pop();
        if(dead[source]){
          collision++;
          sourceList.push(source);
          continue;
        }
    	  double r = (double)random_engine()/(random_engine.max());
        int random = r * network[source].size();
    	  target=network[source][random];

    	  if(!informReceived[target])
    	  {
    	      sumInformReceived++;
    	      informReceived[target]=1;
    	      sourceList.push(target);
    	  }else{
            collision++;
        }
        //今見ている情報源の隣接点がすべて情報を得ている場合はこれ以上流す必要がない
        if(!dead[source]){
          int receiveCount = 0;
          for(int rc = 0;rc < network[source].size();rc++){
            if(informReceived[network[source][rc]]){
              receiveCount++;
            }
          }
          if(receiveCount == network[source].size()){
            dead[source] = 1;
          }
        }
        sourceList.push(source);
    	}
      writeFile << times << " " << sumInformReceived << endl;
      noise << times << " " << collision << endl;
  }
  free(dead);
  free(informReceived);
  writeFile.close();
  noise.close();
}

//次数の逆数を重みとして送り先を乱択する流し方
void inverseControlSpread(int source,int No)
{
  //実験結果を書き込むファイルを作る
  ofstream writeFile,noise;
  writeFile.open(fileName(No,write,"IC","./informspread_not_mem/"+DIRECTORYNEAME).c_str());
  noise.open(fileName(No,write,"IC_noise","./informspread_not_mem/"+DIRECTORYNEAME).c_str());
  if(writeFile.fail())return;
  //vector<set<int> > edge(NETWORKSIZE);
  vector<double> targetSelect;

  mt19937_64 random_engine(No);
  int collision;
  collision = 0;

  int target,sumInformReceived=1,times=0;

  //情報獲得の確認用の配列を初期化
  informReceived = (bool*)calloc(NETWORKSIZE,sizeof(bool));
  dead = (bool*)calloc(NETWORKSIZE,sizeof(bool));
  //edgeの初期化
  /*
  for(int node=0;node<NETWORKSIZE;node++)
    copy(network[node].begin(),network[node].end(),inserter(edge[node],edge[node].begin()));
  */
  queue<int> sourceList;
  sourceList.push(source);
  informReceived[source]=1;
  //情報の拡散
  while(sumInformReceived!=NETWORKSIZE)
    {
      int informSpreadSource=sourceList.size();
      times++;
      collision = 0;
      while(informSpreadSource--)
    	{
    	      source=sourceList.front(); sourceList.pop();
             if(dead[source]){
              collision++;
              sourceList.push(source);
              continue;
            }
    	      double weight=totalInverseWeight(source);
            //cout << weight << endl;
    	      for(int i = 0;i < network[source].size();i++){
    	     	  //情報源の隣接点iが送り先に選択される確率を格納
              targetSelect.push_back((1.0/network[network[source][i]].size())/weight);
            }

    	      double p=(double)random_engine()/(random_engine.max());
            //cout << "p = " << p << endl;
            weight = 0;
    	      for(int i = 0;i < network[source].size();i++)
        		{
        		  weight+=targetSelect[i];
        		  if(weight>p)
        		  {
        		      target=network[source][i];
        		      targetSelect.clear();
        		      break;
        		  }
        		}
            //cout << target << endl;
      	  //sourceList.push(source);
      	    if(!informReceived[target])
      	    {
      	      informReceived[target]=1;
      	      sumInformReceived++;
      	      sourceList.push(target);
      	    }else{
              collision++;
            }
            //今見ている情報源の隣接点がすべて情報を得ている場合はこれ以上流す必要がない
            if(!dead[source]){
              int receiveCount = 0;
              for(int rc = 0;rc < network[source].size();rc++){
                if(informReceived[network[source][rc]]){
                  receiveCount++;
                }
              }
              if(receiveCount == network[source].size()){
                dead[source] = 1;
              }
            }
            sourceList.push(source);
        }
      //cout << times << " " << sumInformReceived << endl;
      writeFile << times <<" " << sumInformReceived <<endl;
      noise << times << " " << collision << endl;
    }
  free(dead);
  free(informReceived);
  writeFile.close();
  noise.close();
}

void proportionControlSpread(int source,int No){
  //実験結果を書き込むファイルを作り
  ofstream writeFile,noise;
  writeFile.open(fileName(No,write,"PC","./informspread_not_mem/"+DIRECTORYNEAME).c_str());
  noise.open(fileName(No,write,"PC_noise","./informspread_not_mem/"+DIRECTORYNEAME).c_str());
  if(writeFile.fail())return;
  //vector<set<int> > edge(NETWORKSIZE);
  vector<double> targetSelect;
  mt19937_64 random_engine(No);
  int collision;

  collision = 0;

  int target,sumInformReceived=1,times=0;

  //情報獲得の確認用の配列を初期化
  informReceived = (bool*)calloc(NETWORKSIZE,sizeof(bool));
  dead = (bool*)calloc(NETWORKSIZE,sizeof(bool));
  /*//edgeの初期化
  for(int node=0;node<NETWORKSIZE;node++)
    copy(network[node].begin(),network[node].end(),inserter(edge[node],edge[node].begin()));
  */
  queue<int> sourceList;
  sourceList.push(source);
  informReceived[source]=1;

  //情報の拡散
  while(sumInformReceived!=NETWORKSIZE)
    {
      int informSpreadSource=sourceList.size();
      times++;
      collision = 0;
      while(informSpreadSource--)
      {
            source=sourceList.front(); sourceList.pop();
             if(dead[source]){
              collision++;
              sourceList.push(source);
              continue;
             }
            double weight=totalProportionWeight(source);
            /*
            for(int i = 0;i <= network[source].size();i++){
              targetSelect.push_back(network[network[source][i]].size())/weight);
            }*/
            for(int i = 0;i < network[source].size();i++){
              targetSelect.push_back(network[network[source][i]].size()/weight);
            }
            double p=(double)random_engine()/(random_engine.max());
            weight=0;
            /*
            for(set<int>::iterator nbit = edge[source].begin();nbit != edge[source].end();nbit++,neighbor++)
            {
              weight+=targetSelect[neighbor];

              if(weight>p)
              {
                  target=*nbit;
                  //edge[source].erase(*nbit);
                  targetSelect.clear();
                  break;
              }
            }*/
            for(int i = 0;i < network[source].size();i++){
              weight+=targetSelect[i];
              if(weight > p){
                target=network[source][i];
                targetSelect.clear();
                break;
              }
            }

          //sourceList.push(source);

          if(!informReceived[target])
            {
              informReceived[target]=1;
              sumInformReceived++;
              sourceList.push(target);
            }else{
              collision++;
            }
            //今見ている情報源の隣接点がすべて情報を得ている場合はこれ以上流す必要がない
            if(!dead[source]){
              int receiveCount = 0;
              for(int rc = 0;rc < network[source].size();rc++){
                if(informReceived[network[source][rc]]){
                  receiveCount++;
                }
              }
              if(receiveCount == network[source].size()){
                dead[source] = 1;
              }
            }
            sourceList.push(source);
        }

      writeFile << times <<" " << sumInformReceived<<endl;
      noise << times << " " << collision << endl;
    }
  free(dead);
  free(informReceived);
  writeFile.close();
  noise.close();
}

/*
  コマンドライン引数を取る
  1:解析したいファイル名
  2:解析したいファイルがあるディレクトリ名
  2:頂点数
  3:使うネットワークの数
  4:一つのネットワークに対しての実験回数
  5:存在しているデータの数

*/
int main(int argc,char* argv[])
{
  cout << argv[1] << " " << argv[2] << " " << argv[3] << " " << argv[4] << " " << argv[5] << " " << argv[6] << endl;
  FILENAME = argv[1];//ファイル名
  DIRECTORYNEAME = argv[2];//
  NETWORKSIZE = atoi(argv[3]);//頂点数
  USENETWORKDATA = atoi(argv[4]);//使うネットワークの数
  SPREADCOUNT = atoi(argv[5]);//実験回数
  EXISTDATA = atoi(argv[6]);//存在しているデータの数
  informReceived = (bool*)calloc(NETWORKSIZE,sizeof(bool));
  degree = (int*)calloc(NETWORKSIZE,sizeof(int));
  string sourcelist;
  network.resize(NETWORKSIZE);
  pair< set<int>::iterator,bool > is;//firstSorceにデータか挿入されたかどうかの判定を格納するpair(insert return pair)
  srand(time(NULL));//シード生成
  set<int> firstSource,useData;//情報拡散の始点を格納,使うデータの番号を格納
  int count=0;//実験回数
  ofstream writeFile;//使ったデータと始点を書き込むファイルストリーム
  sourcelist = "./informspread_not_mem/" + DIRECTORYNEAME + "/network and first source list.txt";
  writeFile.open(sourcelist.c_str());
  if(writeFile.fail()){cout << "error" << endl;return 0;}
  //使うnetworkのデータをランダムに選ぶ
  while(useData.size() < USENETWORKDATA){
    int number=(rand()%EXISTDATA);
    useData.insert(number);
  }

  //選んだデータの数だけ実験を行う
  set<int>::iterator udit = useData.begin();
  while(udit != useData.end())
  {
      firstSource.clear();
      rebuildNetwork(*udit);//ネットワークを生成
      writeFile << *udit << endl; //使ったnetworkのデータを記録

      //最初のsourceをランダムで選ぶ
      while(firstSource.size() < SPREADCOUNT){
        int node = rand()%NETWORKSIZE;
        is = firstSource.insert(node);
        if(is.second)writeFile << node << " ";
      }

      writeFile<<endl;
      set<int>::iterator fsit = firstSource.begin();
      while(fsit != firstSource.end())
    	{
    	  noControlSpread(*fsit,count); //近傍nodeをrandomで選び
        cout << "NoControlSpread " << count << " finish" << endl;
    	  inverseControlSpread(*fsit,count); //確率で次数が小さい近傍nodeを選びやすいように
        cout << "InverseControlSpread "<< count << " finish" << endl;
        proportionControlSpread(*fsit,count);//確率で次数が大きい近傍を選びやすいように
        cout << "proportionControlSpread " << count << " finish" << endl;
        count++;
        fsit++;
    	}
      udit++;
  }

  free(degree);
  writeFile.close();
}
