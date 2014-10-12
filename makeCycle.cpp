#include <iostream>
#include <vector>
#include <set>
#include <time.h>
using namespace std;
#define mk make_pair
typedef vector<bool> vb;
typedef vector<vb> vbb;
typedef pair<int, int> pii;

// 拡張サイクル
void makeCycle(set<pii> &E, int N, int K) {
	for (int n=0; n<N; n++) {
		for (int k=n+1; k<=n+K/2; k++) {
			E.insert(mk(min(n,k%N), max(n,k%N)));
			//E[n][k%N] = true;
			//E[k%N][n] = true;
		}
	}
}

// WSモデル
void makeWS(set<pii> &E, int N, double p) {
	int e = E.size();
	srand((unsigned)time(NULL));
	while (E.size() < e*(1+p)) {
		int v1 = rand()%N, v2 = rand()%N;
		if (v1 == v2) continue;
		E.insert(mk(min(v1,v2), max(v1,v2)));
	}
}

int main() {
	int N, K;
	double p;
	cin >> N >> K >> p;
	//vbb E(N, vb(N, false));
	set<pii> E;
	int e = N*K/2;
	makeCycle(E,N,K);
	makeWS(E,N,p);
	
	// output
	cout << N << " " << E.size() << endl;
	set<pii>::iterator it;
	for (it=E.begin(); it!=E.end(); it++) {
		cout << it->first << " " << it->second << endl;
	}
	/*
	for (int i=0; i<N; i++) {
		for (int j=i+1; j<N; j++) {
			if (E[i][j]) cout << i << " " << j << endl;
		}
	}
	*/
}
