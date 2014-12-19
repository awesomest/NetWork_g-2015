#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <random>
using namespace std;
//データファイルの名前を決める関数
string file_Name(string filename,string directory,int graph_num){
	ostringstream ost;
	ost << graph_num;
	string filenum = ost.str();
	cout << "./"+directory+"/"+filename+"_"+filenum+".dat" << endl;
	return "./"+directory+"/"+filename+ "_"+filenum+".dat";
}
//修正BAを作る関数
void make_rev_BA(int netsize,string filename,string directory,int graph_num){
	ofstream rev_BA;
	set< pair<int,int> > Edges;
	vector<int> degree; 
	rev_BA.open(file_Name(filename,directory,graph_num));
	if(rev_BA.fail()){cout << "error" << endl;return;}
	unsigned totaldegree = 2;
	random_device rd;
	mt19937_64 random_engine(rd());
	degree.push_back(0);
	degree.push_back(1);
	Edges.insert(make_pair(0,1));
	long long int E;
	for(unsigned i=2;i < netsize;i++){
		unsigned p;
		unsigned add_num = 0;
		unsigned cum_deg = 0;
		double re;
		re = (double)random_engine() / (random_engine.max()+1.0);
		p = re * totaldegree;
		degree.push_back(degree[p]);
		degree.push_back(i);
		Edges.insert(make_pair(degree[p],i));
		totaldegree += 2;
	}
	E = Edges.size();
	rev_BA << netsize << " " << E << endl;
	set< pair<int,int> >::iterator st = Edges.begin();
	while(st != Edges.end()){
		rev_BA << st->first << " " << st->second << endl;
		st++;
	}
}
/*
	コマンドライン引数
	1:ファイル名
	3:ディレクトリ名
	4:頂点数
	5:生成するグラフの数
	*/
int main(int argc,char *argv[]){
	string filename = argv[1];
	string directory = argv[2];
	int netsize = atoi(argv[3]);
	int graph = atoi(argv[4]);
	for(int i = 0;i <= graph;i++){
		make_rev_BA(netsize,filename,directory,i);
		cout << "graph_" << i << " finish" << endl;
	}
}