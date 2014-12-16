#include <iostream>
#include <vector>
#include <set>
#include <time.h>
#include <random>
#include <fstream>
#include <sstream>
using namespace std;
#define mk make_pair
typedef vector<bool> vb;
typedef vector<vb> vbb;
typedef pair<int, int> pii;

string fileName(int number,string name="",string directory="")
{
  ostringstream oss;
  string No;
  oss << number;
  No = oss.str();
  //cout << directory +"/"+ name + "/" +name +"_"+FILENAME+ "_time" + No + ".dat" << endl;
  return "./"+directory +"/" + name + "_" + No + ".dat";
}
// 拡張サイクル
void makeCycle(set<pii> &E, int N, int K) {
	for (int n=0; n<N; n++) {
		for (int k=n+1; k<=n+K/2; k++) {
			E.insert(mk(min(n,k%N), max(n,k%N)));
		}
	}
}

// WSモデル
void makeWS(set<pii> &E, int N, double p) {
	int e = E.size();
	mt19937_64 random_engine((unsigned)time(NULL));
	e = e * (1.0+p);
	//cout << e << endl;
	while (E.size() < e) {
		int v1 = (int)(((double)random_engine()/(random_engine.max()+1.0))*N), v2 = (int)(( (double)random_engine() / (random_engine.max()+1.0))*N);
		if (v1 == v2) continue;
		E.insert(mk(min(v1,v2), max(v1,v2)));
	}
}

/*
	コマンドライン引数
	1:頂点数
	2:拡張サイクルの平均次数
	3:枝を追加する確率確率(0.0〜1.0)
	4:作成するファイルの数
	5:WSモデル保存用ディレクトリ名
    6:WSモデルのファイル名のテンプレ
*/

int main(int argc,char* argv[]) {
	int N, K ,make_file_count;
	double p;
	N = atoi(argv[1]);
	K = atoi(argv[2]);
	p = atof(argv[3]);
	cout << N << " " << K << " " << p <<  " " << argv[4] << " " << argv[5] << " " << argv[6] << endl;
	make_file_count = atoi(argv[4]);
	ofstream WS;
	//cin >> N >> K >> p;
	for(int i = 0;i < make_file_count;i++ ){
		set<pii> E;
		int e = N*K/2;
		WS.open(fileName(i,argv[6],argv[5]));
		if(WS.fail()){
			cout << "error" << endl;
			return 0;
		}
		makeCycle(E,N,K);
		makeWS(E,N,p);
		// output
		//cout << N << " " << E.size() << endl;
		for (set<pii>::iterator it=E.begin(); it!=E.end(); it++) {
			WS << it->first << " " << it->second << endl;
		}
		E.clear();
		WS.close();
	}
}