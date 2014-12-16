
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

vector< set<int> > network;//ネットワーク本体のつながり
bool *informReceived;//情報が受け取っているかどうか、添字が頂点番号
int *degree;//頂点の次数、添字が頂点番号
string FILENAME;//生成するファイルの名前
int NETWORKSIZE,USENETWORKDATA,SPREADCOUNT,EXISTDATA;
string DIRECTORYNEAME;

struct TransferInform//source:情報源,target:送り先
{ int source,target; };

struct NodeInform
{ int node,degree; };

bool degree_asc(NodeInform n1,NodeInform n2)//次数比較関数、昇順
{ return n1.degree<n2.degree; }

bool degree_desc(NodeInform n1,NodeInform n2)//次数比較関数、降順
{ return n1.degree>n2.degree; }

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

//inversecontorol用の重み計算
double totalInverseWeight(vector< set<int> > &edge,int source)
{
  double weight=0;
  set<int>::iterator vsit = edge[source].begin();
  while(vsit != edge[source].end()){
    weight+=1.0/network[*vsit].size();
    vsit++;
  }

  return weight;
}
//ランダムに情報の送り先を決める流し方
void noControlSpread(int source,int No)
{
  //ファイルを作り
  ofstream writeFile,noise;
  writeFile.open(fileName(No,write,"NC","./informspread/"+DIRECTORYNEAME).c_str());
  noise.open(fileName(No,write,"NC_noise","./informspread/"+DIRECTORYNEAME).c_str());
  if(writeFile.fail())return;
  int dead_v,collision;//行き場を失った頂点の数,すでに伝播し終えた頂点に送った回数
  dead_v=0;
  collision=0;

  vector<vector<int> > edge(NETWORKSIZE);
  vector<TransferInform> transferList;
  TransferInform transfer;
  mt19937_64 random_engine(No);
  int target,sumInformReceived=1,times=0;

  //情報獲得の確認用の配列を初期化
  informReceived = (bool*)calloc(NETWORKSIZE,sizeof(bool));

  //edgeの初期化
  for(int node=0;node<NETWORKSIZE;node++)
    copy(network[node].begin(),network[node].end(),back_inserter(edge[node]));

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
    	  if(!edge[source].size()) continue;
    	  double r = (double)random_engine()/(random_engine.max());
        int random = r * edge[source].size();
    	  target=edge[source][random];
    	  edge[source].erase(edge[source].begin() + random);

    	  transfer.source=source;
    	  transfer.target=target;
    	  transferList.push_back(transfer);

    	  if(edge[source].size()) sourceList.push(source);//行き場がある場合情報元はまた情報源となる
    	  else dead_v++;
    	  if(!informReceived[target])
    	  {
    	      sumInformReceived++;
    	      informReceived[target]=1;
    	      sourceList.push(target);
    	  }else{
            collision++;
        }
    	}

      //情報を送ってもらったnodeを再び送らないように

      vector<TransferInform>::iterator TIit = transferList.begin();
      while(TIit !=transferList.end())
    	{
          vector<int>::iterator vit = find(edge[TIit->target].begin(),edge[TIit->target].end(),TIit->source);
          if(vit != edge[TIit->target].end()){
            edge[TIit->target].erase(vit);
            if(!(edge[TIit->target].size())){
              dead_v++;
            }
          }
        TIit++;
    	}

      transferList.clear();

      writeFile<<times<<" "<<sumInformReceived<<endl;
      noise << times << " " << dead_v << " " << collision << endl;
  }
  free(informReceived);
  edge.clear();
  writeFile.close();
  noise.close();
}

//次数の逆数を重みとして送り先を乱択する流し方
void inverseControlSpread(int source,int No)
{
  //ファイルを作り
  ofstream writeFile,noise;
  writeFile.open(fileName(No,write,"IC","./informspread/"+DIRECTORYNEAME).c_str());
  noise.open(fileName(No,write,"IC_noise","./informspread/"+DIRECTORYNEAME).c_str());
  if(writeFile.fail())return;
  vector<set<int> > edge(NETWORKSIZE);
  vector<double> targetSelect;
  vector<TransferInform> transferList;
  TransferInform transfer;
  mt19937_64 random_engine(No);
  int collision,dead_v;
  dead_v = 0;
  collision = 0;

  int target,sumInformReceived=1,times=0;

  //情報獲得の確認用の配列を初期化
  informReceived = (bool*)calloc(NETWORKSIZE,sizeof(bool));

  //edgeの初期化
  for(int node=0;node<NETWORKSIZE;node++)
    copy(network[node].begin(),network[node].end(),inserter(edge[node],edge[node].begin()));

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
    	  if(!edge[source].size())
    	    continue;
    	  else if(edge[source].size()==1)
    	    {
    	      target=*(edge[source].begin());
    	      edge[source].clear();
    	      transfer.source=source;
    	      transfer.target=target;
    	      transferList.push_back(transfer);
            dead_v++;
    	    }
    	  else
    	    {
    	      double weight=totalInverseWeight(edge,source);
    	      set<int>::iterator nbit = edge[source].begin();
    	      for(set<int>::iterator nbit = edge[source].begin();nbit != edge[source].end();nbit++){
    	     	  targetSelect.push_back((1.0/network[*nbit].size())/weight);
            }
    	      double p=(double)random_engine()/(random_engine.max());
    	      weight=0;
            int neighbor =0;
    	      for(set<int>::iterator nbit = edge[source].begin();nbit != edge[source].end();nbit++,neighbor++)
        		{
        		  weight+=targetSelect[neighbor];

        		  if(weight>p)
        		  {
        		      target=*nbit;
        		      edge[source].erase(*nbit);
        		      targetSelect.clear();
        		      break;
        		  }
        		}
    	    }
      	  transfer.source=source;
      	  transfer.target=target;

      	  transferList.push_back(transfer);

      	  if(edge[source].size()) sourceList.push(source);

      	  if(!informReceived[target])
      	    {
      	      informReceived[target]=1;
      	      sumInformReceived++;
      	      sourceList.push(target);
      	    }else{
              collision++;
            }
        }
          //情報を送ってもらったnodeを再び送らないように
      vector<TransferInform>::iterator TIit = transferList.begin();
      while(TIit != transferList.end())
    	{
    	  set<int>::iterator v = edge[TIit->target].find(TIit->source);

    	  if(v!=edge[TIit->target].end()){
    	    edge[TIit->target].erase(v);
          if(!(edge[TIit->target].size())){
            dead_v++;
          }
        }
        TIit++;
    	}
      transferList.clear();

      writeFile << times <<" " << sumInformReceived<<endl;
      noise << times << " " << dead_v << " " << collision << endl;
    }
  free(informReceived);
  edge.clear();
  writeFile.close();
  noise.close();
}

//次数の大きい順、小さい順に情報の送り先を決めて流す関数
void degreeControlSpread(int source,int method,int No)
{
  //ファイルを作り
  ofstream writeFile,noise;
  if(method==desc){
    writeFile.open(fileName(No,write,"DC_desc","./informspread/"+DIRECTORYNEAME).c_str());
    noise.open(fileName(No,write,"DC_desc_noise","./informspread/"+DIRECTORYNEAME).c_str());
  }else{
    writeFile.open(fileName(No,write,"DC_asc","./informspread/"+DIRECTORYNEAME).c_str());
    noise.open(fileName(No,write,"DC_asc_noise","./informspread/"+DIRECTORYNEAME).c_str());
  }
  if(writeFile.fail())return;

  vector<vector<NodeInform> > edge(NETWORKSIZE);
  vector<TransferInform> transferList;
  TransferInform transfer;
  int target,neighbor,sumInformReceived=1,times=0;
  int collision,dead_v;
  //情報獲得の確認用の配列を初期化
  informReceived = (bool*)calloc(NETWORKSIZE,sizeof(bool));

  collision = 0;
  dead_v = 0;

  //edgeの初期化
  for(int i=0;i<NETWORKSIZE;i++)
  {
      edge[i].resize(network[i].size());
      set<int>::iterator stit;
      stit= network[i].begin();
      for(int j=0;stit != network[i].end();j++,stit++)
    	{
    	  edge[i][j].node=*stit;
    	  edge[i][j].degree=degree[*stit];
    	}
  }

  //descかascのソート
  if(method==desc)
  {
      for(int i=0;i<NETWORKSIZE;i++)
	    sort(edge[i].begin(),edge[i].end(),degree_desc);
  }
  else
  {
      for(int i=0;i<NETWORKSIZE;i++)
	    sort(edge[i].begin(),edge[i].end(),degree_asc);
  }

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

    	  if(!edge[source].size()) continue;

    	  for(neighbor=1;neighbor<edge[source].size() && edge[source][neighbor].degree==edge[source][neighbor-1].degree;neighbor++);

    	  int select=rand()%neighbor;
    	  target=edge[source][select].node;
    	  edge[source].erase(edge[source].begin()+select);

    	  transfer.source=source;
    	  transfer.target=target;
    	  transferList.push_back(transfer);

    	  if(edge[source].size()) sourceList.push(source);
        else dead_v++;

    	  if(!informReceived[target])
    	    {
    	      sumInformReceived++;
    	      informReceived[target]=1;
    	      sourceList.push(target);
    	    }else{
            collision++;
          }
    	}

          //情報を送ってもらったnodeを再び送らないように
      for(int i=0;i<transferList.size();i++)
    	{
    	  vector<NodeInform>::iterator p;
    	  for(int j = 0;j<edge[transferList[i].target].size();j++)
    	    if(edge[transferList[i].target][j].node==transferList[i].source)
    	   {
        		p=edge[transferList[i].target].begin();
        		edge[transferList[i].target].erase(p+j);
            if(!(edge[transferList[i].target].size())){
              dead_v++;
            }
        		break;
    	   }
    	}

      transferList.clear();

      writeFile<<times<<" "<<sumInformReceived<<endl;
      noise << times << " " << dead_v << " " << collision << endl;
    }

  free(informReceived);
  edge.clear();
  writeFile.close();
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
  //cout << argv[1] << " " << argv[2] << " " << argv[3] << " " << argv[4] << " " << argv[5] << " " << argv[6] << endl;
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
  pair< set<int>::iterator,bool > is;
  srand(time(NULL));
  set<int> firstSource,useData;
  int count=0;//実験回数
  ofstream writeFile;
  sourcelist = "./informspread/" + DIRECTORYNEAME + "/network and first source list.txt";
  writeFile.open(sourcelist.c_str());
  if(writeFile.fail()){cout << "error" << endl;return 0;}
  //使うnetworkのデータをランダムに選ぶ
  while(useData.size() < USENETWORKDATA){
    int number=(rand()%EXISTDATA);
    useData.insert(number);
  }
  set<int>::iterator udit = useData.begin();
  while(udit != useData.end())
  {
      firstSource.clear();
      rebuildNetwork(*udit);
      writeFile << *udit << endl; //使ったnetworkのデータを記録

      //最初のsourceをランダムで選び
      while(firstSource.size() < SPREADCOUNT){
        int node = rand()%NETWORKSIZE;
        is = firstSource.insert(node);
        if(is.second)writeFile << node << " ";
      }

      writeFile<<endl;
      set<int>::iterator fsit = firstSource.begin();
      while(fsit != firstSource.end())
    	{
    	  srand(count+1);
    	  noControlSpread(*fsit,count); //近傍nodeをrandomで選び
        cout << "NoControlSpread " << count << " finish" << endl;
    	  //srand(count+1);
    	  inverseControlSpread(*fsit,count); //確率で次数が小さい近傍nodeを選ぴ安いように
        cout << "InverseControlSpread "<< count << " finish" << endl;
    	  srand(count+1);
    	  degreeControlSpread(*fsit,desc,count); //近傍nodeの次数個数を降順で選び
        cout << "Degree_desc " << count << " finish" << endl;
    	  srand(count+1);
    	  degreeControlSpread(*fsit,asc,count); //近傍nodeの次数個数を昇順で選び
        cout << "Degree_desc " << count << " finish" << endl;
        count++;
        fsit++;
    	}
      udit++;
  }

  free(degree);
  writeFile.close();
}
