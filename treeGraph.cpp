#include <iostream>
#include <cmath>
#include <queue>
using namespace std;

int main(){
	int N = 1, edge = 0, k, l;
	cin >> k >> l;
	if(l > 0){
		for(int i = 0; i <= l-1; i++){
			N += k * pow(k-1.0, i);
		}
		edge = N-1;
	}
	cout << N << " " << edge << endl;

	queue<int> q;
	int Ni = 0;
	int Nj = 1;
	int newN = k;
	if(l > 0){
		for(int j = 0; j < k; j++){
			cout << Ni << " " << Nj << endl;
			q.push(Nj);
			Nj++;
		}

		for(int i = 1; i < l; i++){
			for(int j = 0; j < newN; j++){
				Ni = q.front();
				q.pop();
				for(int m = 0; m < k-1; m++){
					cout << Ni << " " << Nj << endl;
					q.push(Nj);
					Nj++;
				}
			}
			newN *= k-1;
		}
	}

}
