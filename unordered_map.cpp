//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
using namespace std;
//+++++++++++++++++++++++++++プロトタイプ宣言+++++++++++++++++++++++++++
class VERTEX;
class NETWORK;
class LessVertex;	
bool cheak_Clasta_Edge(vector<VERTEX*> &vers,VERTEX &v);
bool checkEdge(vector<VERTEX*> &ver,int v);
bool line_Edge(vector<VERTEX> &ver,int vMoto,int vSaki);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*+++++++++++++++++++++++++頂点のデータクラス++++++++++++++++++++++++++++*/
//*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class VERTEX
{
public:
	int vNum;//頂点番号
	double clasta; //クラスタ係数
	int triangle;//この頂点を中心としてできる三角形の数
	int degree; //次数
	int D_centrality; //次数中心性
	//++++++++++++++++++未実装+++++++++++++++
	// double N_centrality; //近接中心性
	// double M_centrality; //媒介中心性
	//+++++++++++++++++++++++++++++++++
	unordered_map<int,int> neighber;//隣接頂点集合(重さ)
	VERTEX();//コンストラクタ
	VERTEX(int verNum);//コンストラクタのオーバーロード
	void calcDegree();//次数を求める関数
	void show_Degree();//次数の表示
	void show_neighber();//隣接頂点を表示する関数
	void show_clasta();//クラスタ係数の表示
	void calc_alldata();//データの一括計算
	void show_alldata();//データの一括表示
};


//コンストラクタでメンバ変数の値を初期化する
inline VERTEX::VERTEX()
{
	vNum = 0;
	clasta = 0;
	triangle = 0;
	degree = 0;
	D_centrality = 0;
	//N_centrality = 0;
	//M_centrality = 0;
}


//頂点番号を指定して生成する
inline VERTEX::VERTEX(int verNum)
{
	vNum = verNum;
	clasta = 0;
	triangle = 0;
	degree = 0;
	D_centrality = 0;
	//N_centrality = 0;
	//M_centrality = 0;
}


//クラスタ係数の表示
inline void VERTEX::show_clasta()
{
	cout << "頂点v" << vNum << "のクラスター係数は" << clasta << "です" << endl;
}

//次数計算
inline void VERTEX::calcDegree()
{
	degree = (int)neighber.size();
	D_centrality = degree;
}

//次数表示
inline void VERTEX::show_Degree()
{
	cout << "頂点v" << vNum << "の次数及び次数中心性は" << degree << "です" << endl;
}


//隣接頂点の表示
inline void VERTEX::show_neighber()
{
	cout << "頂点v" << vNum << "と隣接している点は" << endl;
	unordered_map<int,int>::iterator pneighber = neighber.begin();
	while(pneighber != neighber.end())
	{
		cout << "v" << pneighber->first << ", ";
		pneighber++;
	}
	cout << "です" << endl;
}

//計算できるデータを全部計算
inline void VERTEX::calc_alldata()
{
	calcDegree();
}


//頂点のデータを全件表示
inline void VERTEX::show_alldata()
{
	cout << endl << "+++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "v" << vNum << endl;
	show_neighber();
	show_Degree();
	show_clasta();
	cout << endl << "+++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++" << endl;
}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++頂点比較用のオペレータ++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//頂点集合のアドレスをソートするために変更する
class LessVertex
{
public:
	bool operator()(const VERTEX &v1,const VERTEX &v2)const
	{
		return v1.vNum < v2.vNum;
	}
};



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++ネットワーク全体に関するクラス++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class NETWORK
{
private:
	int Edge;//ネットワークの辺の数
	int Vertex;//ネットワークの頂点の数
	double Clasta;//クラスター係数
public:
	int totalDegree;//総次数
	vector<VERTEX> vertex;//ネットワークの頂点集合
public:
	//++++++++++引数付コンストラクタ++++++++++++
	NETWORK(int v,int e){
		Edge = e;
		Vertex = v;
		for(int i = 0;i < v;i++){
			VERTEX ver;
			ver.vNum = i;
			vertex.push_back(ver);
		}
		totalDegree = 2*Edge;//握手定理より
	}
	//++++++++++++++++++++++++++++++++++++++++++
	//そこまで使わないと思う
	void clac_Edge()
	{
		totalDegree = 0;
		//もしもの時に
		for(register int i = 0;i < vertex.size();i++){
			totalDegree += vertex[i].degree;
		}
		Edge = totalDegree / 2;
	}
	//+++++++++++++++++使わないかも++++++++++++++
	void getEdge(int *e)
	{
		*e = Edge;
	}
	void setVertex()
	{
		Vertex = vertex.size();
	}
	void getVertex(int *v)
	{
		*v = Vertex;
	}
	//+++++++++++++++++++++++++++++++++++++++++
	void calcClasta()
	{
		Clasta = 0;
		int eff_ver = 0; //クラスタ係数の計算に有効な頂点の数

		for(register int i = 0;i < vertex.size();i++)
		{
			if(vertex[i].degree > 1)
			{
				//計算
				int triangle = 0;
				unordered_map<int,int>::iterator pm1 = vertex[i].neighber.begin();
				for(;pm1 != vertex[i].neighber.end();pm1++){
					unordered_map<int,int>::iterator pm2 = pm1;
					for(++pm2;pm2 != vertex[i].neighber.end();pm2++){
						if(vertex[pm1->first].neighber.count(pm2->first)){
							triangle++;
						}
					}
				}
				vertex[i].clasta = (double)triangle / (vertex[i].degree * (vertex[i].degree - 1) / 2);
				Clasta += vertex[i].clasta;
				eff_ver++;
			}
			if(i % 100 == 0){
				cout << i << endl;
			}
		}
		Clasta = Clasta / eff_ver;
	}
	void show_clasta()
	{
		cout << "このネットワークのクラスター係数は" << Clasta << "です" << endl;		
	}
	void show_vertex()
	{
		cout << endl << endl;
		cout << "このネットワークの頂点集合は" << endl;
		for(register int i = 0;i < vertex.size();i++){
			cout << "v" << vertex[i].vNum << "  ";
		}
		cout << endl << "です" << endl << endl;
	}
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++メイン関数++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int main(void){
	int v,e;
	cout << "頂点数と辺の数を入力してください(m_n)" << endl;
	cin >> v >> e;
	NETWORK net(v,e);//頂点数v,枝数eのネットワークを生成
	//net.show_vertex();
	//++++++++++++++++++++++++++情報入力及び枝張り++++++++++++++++++++++++++++++
	cout << "隣接する頂点対を入力してください(v1_v2)" << endl;
	while(e > 0)
	{
		int v1,v2;
		cin >> v1 >> v2;
		if(v1 == v2)
		{
			cout << "自己ループになっています 別の組み合わせを入力してください" << endl;
			continue;
		}	
		net.vertex[v1].neighber[v2] = 1;
		net.vertex[v2].neighber[v1] = 1;
		e--;
	}

	//sort(net.vertex.begin(),net.vertex.end(),LessVertex());//まだいらない

	for(int i = 0;i < 3 ;i++)
	{
		cout << "+++++++++++++++++++++++++++++++++++++++++" << endl;
	}
	/*方針を変更するまでコメントアウト
	net.setVertex();
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//+++++++++++++++++++++++++++++++++++++++++++++++++表示+++++++++++++++++++++++++++++++++++++++++++++++++
	//net.show_vertex();//頂点集合を表示
	//++++++++++++全頂点の情報表示++++++++++++++

	vector<VERTEX>::iterator pv;
	for(pv = net.vertex.begin();pv != net.vertex.end();pv++)
	{
		pv->calc_alldata();
		//pv->show_alldata();		
	}
	cout << endl;
	//+++++++++++++++++++++++++++++++++++++++
	//++++++++ネットワークのクラスター係数を表示+++++++++++++++++++++++++++
	net.calcClasta();
	/*for(pv = net.vertex.begin();pv != net.vertex.end();pv++){
		pv->show_alldata();
	}*/
	net.show_clasta();
	cout << endl;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++関数+++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//選んだ2頂点間に枝が張っているかチェックする関数
bool cheak_Clasta_Edge(vector<VERTEX*> &vers,VERTEX &v)
{
	vector<VERTEX*>::iterator pv;
	pv = vers.begin();
	while(pv != vers.end())
	{
		if((*pv)->vNum == v.vNum)
		{
			return true;
		}
		pv++;
	}
	return false;
}

