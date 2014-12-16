#include <cstdio>
#include <algorithm>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <set>
using namespace std;

class vertex
{
	public:
		int number;		// 頂点番号
		int degree;		// 次数
		set<int> neighbor;		// 隣接頂点

		// 隣接頂点を追加する
		void insertNeighbor( int v ){
			neighbor.insert(v);
		}
};

class network
{
	public:
		int n_vertex;		// 頂点数
		int sum_degree;		// 総次数
		double avg_dist;		// 平均距離
		vector<vertex> neighbors;		// 全ての頂点に対する隣接頂点

		// 頂点v1と頂点v2を連結する
		void insertNeighbors( int v1, int v2 ){
			neighbors[v1].insertNeighbor(v2);
			neighbors[v2].insertNeighbor(v1);
		}

		/*int calcDistance(){
		}*/

};

int main(){
	int n;		// n : ネットワーク数
	printf("ネットワーク数 : ");
	scanf("%d", &n);
	vector<network> networks(n);	// ネットワークの配列
	for( int i=0; i<n; i++ ){
		int v;		// v : 頂点数
		printf("頂点数 : ");
		scanf("%d", &v);
		networks[i].n_vertex = v;
		networks[i].neighbors.resize(v);
		//vector<vertex> vertexes(n);
		
		/****** 隣接行列で入力(0 or 1) *******/
		cout << "    ";
		for( int j=0; j<v; j++ ) cout << j << " ";
		cout << endl;
		cout << "  --";
		for( int j=0; j<v; j++ ) cout << "--";
		cout << endl;
		for( int j=0; j<v; j++ ){
			networks[i].neighbors[j].number = j;
			cout << j << " | ";
			for( int k=0; k<v; k++ ){
				int neighbors;
				scanf("%d", &neighbors);
				if( neighbors == 1 ) networks[i].neighbors[j].insertNeighbor(k);
			}
			networks[i].neighbors[j].degree = networks[i].neighbors[j].neighbor.size();
		}
		cout << "  --";
		for( int j=0; j<v; j++ ) printf("--");
		cout << endl; 
		/*************************************/

		/********** 出力 *********************/
		for( int j=0; j<v; j++ ){
			cout << "-------------------" << endl;
			cout << "■ 頂点番号 " << j << endl;
			cout << "                   " << endl;
		
			cout << "次数     : " << networks[i].neighbors[j].degree << " " << endl;
			cout << "隣接頂点 : ";
			set<int>::iterator it = networks[i].neighbors[j].neighbor.begin();
			for( ; it != networks[i].neighbors[j].neighbor.end(); it++ ){
				 cout << *it << " ";
			}
			cout << endl << endl;
		}
		/*************************************/
	}
}
