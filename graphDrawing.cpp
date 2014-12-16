/*ネットワークを描画する*/
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main(){
	int N, edge;
	cin >> N >> edge;

	ofstream ofs("network.dot");
	ofs << "graph sample {" << endl;

	for(int i = 0; i < edge; i++){
		int a, b;
		cin >> a >> b;
		ofs << a << "--" << b << ";" << endl;
	}

	ofs << "}";
}
