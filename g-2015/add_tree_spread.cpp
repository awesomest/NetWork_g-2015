/*木構造のネットワークに枝を加えながら伝播時間を計算するプログラム*/
#include <iostream>
#include <set>
#include <queue>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <random>
using namespace std;

#define read 1
#define write 2
#define desc 1
#define asc 2

vector< set<int> > network;//ネットワーク本体のつながり
bool *informReceived;//情報が受け取っているかどうか、添字が頂点番号
int *degree;//頂点の次数、添字が頂点番号
bool *dead;
string FILENAME;//生成するファイルの名前
int NETWORKSIZE,USENETWORKDATA,SPREADCOUNT,EXISTDATA;
string DIRECTORYNEAME;

class fileProtData
{
public:
  int times;
  int collision;
  int add_Edge;
  fileProtData();
  void averageData(int num){
    times = times/num;
    collision = collision/num;
    add_Edge = add_Edge/num;
  }
  void operator+=(fileProtData obj){
    times += obj.times;
    collision += obj.collision;
    add_Edge += obj.add_Edge;
  }
  void init(){
    times = 0;
    collision = 0;
    add_Edge = 0;
  }
};
inline fileProtData::fileProtData(){
  times = 0;
  collision = 0;
  add_Edge = 0;
}

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
    //cout << "./"+directory+"/" + name +"_"+No+".dat" << endl;
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
  if(readFile.fail()){return;}//ファイル読み込みが失敗したら抜ける
  //ネットワーク生成
  while(readFile>>node1>>node2)
    {
      
      network[node1].insert(node2);
      network[node2].insert(node1);
      degree[node1]++;
      degree[node2]++;
    }
 
  readFile.close();
}
void addEdge(int num){
	int node1,node2,count;
	count = 0;
	random_device rd;
	mt19937_64 random_engine(rd());
	while(count < num){
		node1=random_engine()%NETWORKSIZE;
		node2=random_engine()%NETWORKSIZE;
		if((node1 == node2 || network[node1].count(node2) || network[node2].count(node1))){
			continue;//すでに枝が張られていればやりなおし
		}
		network[node1].insert(node2);
		network[node2].insert(node1);
		degree[node1]++;
		degree[node2]++;
		count++;
	}
}
//inversecontorol用の重み計算
double totalInverseWeight(vector< vector<int> > &edge,int source)
{
  double weight=0;
  //隣接点すべての重みの和の計算
  for(int i = 0;i < edge[source].size();i++){
    weight+=1.0/edge[edge[source][i]].size();
  }
  return weight;
}

double totalProportionWeight(vector< vector<int> > &edge,int source)
{
  double weight=0;
  //隣接点すべての重みの和の計算
  for(int i = 0;i < edge[source].size();i++){
    weight+=edge[edge[source][i]].size();
  }

  return weight;
}


//ランダムに情報の送り先を決める流し方
fileProtData noControlSpread(int source,int No,int add=0)
{
  fileProtData protData;
  int collision;
  collision=0;
  vector<vector<int> > edge(NETWORKSIZE);
  mt19937_64 random_engine(No);
  int target,sumInformReceived=1,times=0;
  //edgeの初期化
  for(int node=0;node<NETWORKSIZE;node++)
    copy(network[node].begin(),network[node].end(),back_inserter(edge[node]));
  //cout << "graph copy finish" << endl;
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
      while(informSpreadSource--)
    	{
        
    	  source=sourceList.front(); sourceList.pop(); 
        if(dead[source]){
          collision++;
          sourceList.push(source);
          continue;
        }
    	  double r = (double)random_engine()/(random_engine.max());
        int random = r * edge[source].size();
    	  target=edge[source][random];
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
          for(int rc = 0;rc < edge[source].size();rc++){
            if(informReceived[edge[source][rc]]){
              receiveCount++;
            }
          }
          if(receiveCount == edge[source].size()){
            dead[source] = 1;
          }
        }
        sourceList.push(source);

      }
  }
  //writeFile<< add <<" "<< times <<endl;
  //noise << times << " " << collision << endl;
  protData.times = times;
  protData.add_Edge = add;
  protData.collision = collision;
  free(dead);
  free(informReceived);
  edge.clear();
  return protData;
}

//次数の逆数を重みとして送り先を乱択する流し方
fileProtData inverseControlSpread(int source,int No,int add=0)
{
  fileProtData protData;
  vector<vector<int> > edge(NETWORKSIZE);
  vector<double> targetSelect;
  
  mt19937_64 random_engine(No);
  int collision;
  collision = 0;

  int target,sumInformReceived=1,times=0;
  
  //情報獲得の確認用の配列を初期化
  informReceived = (bool*)calloc(NETWORKSIZE,sizeof(bool));
  dead = (bool*)calloc(NETWORKSIZE,sizeof(bool));

  for(int node=0;node<NETWORKSIZE;node++)
    copy(network[node].begin(),network[node].end(),back_inserter(edge[node]));

  queue<int> sourceList;
  sourceList.push(source);
  informReceived[source]=1;
  
  //情報の拡散
  while(sumInformReceived!=NETWORKSIZE)
    {
      int informSpreadSource=sourceList.size();
      times++;
      while(informSpreadSource--)
    	{
    	  source=sourceList.front(); sourceList.pop();
         if(dead[source]){
          collision++;
          sourceList.push(source);
          continue;
         }
    	      double weight=totalInverseWeight(edge,source);
             for(int i = 0;i < edge[source].size();i++){
              //情報源の隣接点iが送り先に選択される確率を格納
              targetSelect.push_back((1.0/edge[edge[source][i]].size())/weight);
            }
    	      
    	      double p=(double)random_engine()/(random_engine.max());
    	      weight=0;
            for(int i = 0;i < edge[source].size();i++)
            {
              weight+=targetSelect[i];
              if(weight>p)
              {
                  target=edge[source][i];
                  targetSelect.clear();
                  break;
              }
            }
      	  
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
              for(int rc = 0;rc < edge[source].size();rc++){
                if(informReceived[edge[source][rc]]){
                  receiveCount++;
                }
              }
              if(receiveCount == edge[source].size()){
                dead[source] = 1;
              }
            }
            sourceList.push(source);
          }
    }
    protData.add_Edge = add;
    protData.times = times;
    protData.collision = collision;
  free(dead);
  free(informReceived);
  edge.clear();
  return protData;
}

fileProtData proportionControlSpread(int source,int No,int add=0){
  fileProtData protData;
  vector<vector<int> > edge(NETWORKSIZE);
  vector<double> targetSelect;
  mt19937_64 random_engine(No);
  int collision;

  collision = 0;
  //edgeの初期化
  for(int node=0;node<NETWORKSIZE;node++)
    copy(network[node].begin(),network[node].end(),inserter(edge[node],edge[node].begin()));
  
  int target,sumInformReceived=1,times=0;
  
  //情報獲得の確認用の配列を初期化
  informReceived = (bool*)calloc(NETWORKSIZE,sizeof(bool));
  dead = (bool*)calloc(NETWORKSIZE,sizeof(bool));

  queue<int> sourceList;
  sourceList.push(source);
  informReceived[source]=1;
  
  //情報の拡散
  while(sumInformReceived!=NETWORKSIZE)
    {
      int informSpreadSource=sourceList.size();
      times++;
      while(informSpreadSource--)
      {
            source=sourceList.front(); sourceList.pop();
            if(dead[source]){
              collision++;
              sourceList.push(source);
              continue;
             }
            double weight=totalProportionWeight(edge,source);
            /*
            for(int i = 0;i <= network[source].size();i++){
              targetSelect.push_back(network[network[source][i]].size())/weight);
            }*/
            for(int i = 0;i < edge[source].size();i++){
              targetSelect.push_back(edge[edge[source][i]].size()/weight);
            }
            double p=(double)random_engine()/(random_engine.max());
            weight=0;
            
            for(int i = 0;i < edge[source].size();i++){
              weight+=targetSelect[i];
              if(weight > p){
                target=edge[source][i];
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
              for(int rc = 0;rc < edge[source].size();rc++){
                if(informReceived[edge[source][rc]]){
                  receiveCount++;
                }
              }
              if(receiveCount == edge[source].size()){
                dead[source] = 1;
              }
            }
            sourceList.push(source);
        }
     
    }
    protData.times = times;
    protData.collision = collision;
    protData.add_Edge = add;
  free(dead);
  free(informReceived);
  return protData;
}
/*
  コマンドライン引数を取る
  1:解析したいファイル名
  2:解析したいファイルがあるディレクトリ名
  3:頂点数
  4:使うネットワークの数
  5:一つのネットワークに対しての実験回数
  6:存在しているデータの数
*/

int main(int argc,char* argv[]){
	FILENAME=argv[1];
	DIRECTORYNEAME=argv[2];
	NETWORKSIZE = atoi(argv[3]);
	USENETWORKDATA = atoi(argv[4]);
	SPREADCOUNT = atoi(argv[5]);
	EXISTDATA = atoi(argv[6]);
	informReceived = (bool*)calloc(NETWORKSIZE,sizeof(bool));
  	degree = (int*)calloc(NETWORKSIZE,sizeof(int));
 	string sourcelist;
 	network.resize(NETWORKSIZE);
 	pair< set<int>::iterator,bool > is;
 	srand(time(NULL));
	set<int> firstSource,useData;
	int count=0;//実験回数
	ofstream writeFile;
	sourcelist = "./addEdgeSpread/" + DIRECTORYNEAME + "/network and first source list.txt";
	cout << sourcelist << endl;
 	writeFile.open(sourcelist.c_str());
 	if(writeFile.fail()){cout << "sourceList can't make" << endl;return 0;}
 	//使うnetworkのデータをランダムで選ぶ
 	while(useData.size() < USENETWORKDATA){
 		int number=(rand()%EXISTDATA);
 		useData.insert(number);
 	}
 	set<int>::iterator udit = useData.begin();
 	while(udit != useData.end())
 	{
 		firstSource.clear();
 		//rebuildNetwork(*udit);
 		writeFile << *udit << endl;

      //最初のsourceをランダムで選び
      while(firstSource.size() < SPREADCOUNT){
        int node = rand()%NETWORKSIZE;
        is = firstSource.insert(node);
        if(is.second)writeFile << node << " ";
      }

      writeFile<<endl;
      //ファイルを作り
      rebuildNetwork(*udit);
      ofstream IC_File,IC_noise,NC_File,NC_noise,PC_File,PC_noise;
      NC_File.open(fileName(count,write,"NC","./add_edge_spread/"+DIRECTORYNEAME).c_str());
      NC_noise.open(fileName(count,write,"NC_noise","./add_edge_spread/"+DIRECTORYNEAME).c_str());
      IC_File.open(fileName(count,write,"IC","./add_edge_spread/"+DIRECTORYNEAME).c_str());
      IC_noise.open(fileName(count,write,"IC_noise","./add_edge_spread/"+DIRECTORYNEAME).c_str());
      PC_File.open(fileName(count,write,"PC","./add_edge_spread/"+DIRECTORYNEAME).c_str());
      PC_noise.open(fileName(count,write,"PC_noise","./add_edge_spread/"+DIRECTORYNEAME).c_str());
      if(NC_File.fail() || NC_noise.fail() || IC_File.fail() || IC_noise.fail() || PC_File.fail() || PC_noise.fail() ){cout << "cant make" << endl;return 0;}
      for(int i = 0;i <= NETWORKSIZE/100;i++){
        fileProtData NC_averageData,IC_averageData,PC_averageData;
        //addEdge(NETWORKSIZE/10);
        set<int>::iterator fsit = firstSource.begin();
        while(fsit != firstSource.end())
        {
          NC_averageData += noControlSpread(*fsit,count,i*NETWORKSIZE/100); //近傍nodeをrandomで選び
            //cout << "NoControlSpread " << " finish" << endl;
          IC_averageData += inverseControlSpread(*fsit,count,i*NETWORKSIZE/100); //確率で次数が小さい近傍nodeを選ぴ安いように
            //cout << "InverseControlSpread "<<  " finish" << endl;
          PC_averageData += proportionControlSpread(*fsit,count,i*NETWORKSIZE/100);
            //cout << "ProportionControlSpread " <<  " finish" << endl;
          fsit++;
        }
        NC_averageData.averageData(firstSource.size());
        IC_averageData.averageData(firstSource.size());
        PC_averageData.averageData(firstSource.size());
        cout << "NC times " << NC_averageData.times << " add_Edge " << NC_averageData.add_Edge << " collision " << NC_averageData.collision << endl;
        cout << "IC times " << IC_averageData.times << " add_Edge " << IC_averageData.add_Edge << " collision " << IC_averageData.collision << endl;
        cout << "PC times " << PC_averageData.times << " add_Edge " << PC_averageData.add_Edge << " collision " << PC_averageData.collision << endl;
        NC_File << NC_averageData.add_Edge << " " << NC_averageData.times << endl;
        NC_noise << NC_averageData.add_Edge << " " << NC_averageData.collision << endl;
        IC_File << IC_averageData.add_Edge << " " << IC_averageData.times << endl;
        IC_noise << IC_averageData.add_Edge << " " << IC_averageData.collision << endl;
        PC_File << PC_averageData.add_Edge << " " << PC_averageData.times << endl;
        PC_noise << PC_averageData.add_Edge << " " << PC_averageData.collision << endl;
        addEdge(NETWORKSIZE/10);
      }
      NC_File.close();
      NC_noise.close();
      IC_File.close();
      IC_noise.close();
      PC_File.close();
      PC_noise.close();
      count++;
      udit++;
    }
 	free(degree);
 	writeFile.close();
}
/*
 for(int i = 1;i < NETWORKSIZE/10;i++){
          addEdge(NETWORKSIZE/10);
        noControlSpread(*fsit,count,NC_File,NC_noise,i*NETWORKSIZE/10); //近傍nodeをrandomで選び
          cout << "NoControlSpread " << count << " add edge " << i*NETWORKSIZE/10 << "finish" << endl;
        inverseControlSpread(*fsit,count,IC_File,IC_noise,i*NETWORKSIZE/10); //確率で次数が小さい近傍nodeを選ぴ安いように
          cout << "InverseControlSpread "<< count << " add edge " << i*NETWORKSIZE/10 << " finish" << endl;
        proportionControlSpread(*fsit,count,PC_File,PC_noise,i*NETWORKSIZE/10);
          cout << "ProportionControlSpread " << count << " finish" << endl;

*/