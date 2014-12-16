#include "../hpp/init.hpp"
#include "../hpp/file_operation.hpp"

bool graph_shuffle(graph &G, string file_name, int num_file, string extension){
	//次数の数を数える
	vector<int> degree_count(G.vertex_num);
	for(int i = 0; i < degree_count.size(); i++){
		degree_count[i] = 0;
	}
	for(int i = 0; i < G.vertex_num; i++) degree_count[G.V[i].edge.size()]++;

	//1以外の次数ごとの数全てをシャッフル
	vector<int> add_vertex;
	for(int i = 2; i < degree_count.size(); i++){
		if(degree_count[i] == 0) continue;
		for(int j = 0; j < degree_count[i]; j++) add_vertex.push_back(i);
	}

	for(int shuffle_num = 0; shuffle_num < 100; shuffle_num++){
		random_shuffle(add_vertex.begin(), add_vertex.end());

		//頂点をつなげていく
		graph shuffle_G;
		shuffle_G.vertex_num = G.vertex_num;
		shuffle_G.edge_num = G.edge_num;
		for(int i=0; i<shuffle_G.vertex_num; i++){
			vertex a;
			a.number=i;
			a.degree=0;
			shuffle_G.V.push_back(a);
		}

		vector<int>  m_vertices;
		for(int i = 0; i < add_vertex[0]; i++) m_vertices.push_back(0);

		for(int i = 1; i < add_vertex.size(); i++){

			int t = my_rand(m_vertices.size());
			int v = m_vertices[t];
			shuffle_G.V[i].edge.insert(v);
			shuffle_G.V[v].edge.insert(i);
			shuffle_G.V[i].degree++;
			shuffle_G.V[v].degree++;

			m_vertices[t] = i; //つなげたところを消す

			for(int j = 0; j < add_vertex[i]-2; j++) m_vertices.push_back(i);	//残りの枝を入れる
		}

		for(int i = 0; i < m_vertices.size(); i++){	//次数１を足す
			int v = i + add_vertex.size();
			if(v >= shuffle_G.vertex_num) return false;
			int m = m_vertices[i];
			shuffle_G.V[m].edge.insert(v);
			shuffle_G.V[v].edge.insert(m);
			shuffle_G.V[m].degree++;
			shuffle_G.V[v].degree++;
		}

		//ファイル作成
		ofstream ofs;
		ostringstream ost;
		ost << num_file;
		string s = "shuffle_"+file_name+"_"+ost.str();
		ofs.open(file_fullname(shuffle_num,s,extension).c_str());
		if(ofs.fail()) return false;
		ofs << shuffle_G.vertex_num << " " << shuffle_G.edge_num << endl;
		for(int i = 0; i < shuffle_G.vertex_num; i++){
			multiset<int>::iterator ite = shuffle_G.V[i].edge.begin();
			for(; ite != shuffle_G.V[i].edge.end(); ite++){
				if(i <= *ite) continue;
				ofs << i << " " << *ite << endl;
			}
		}
		ofs.close();

	}

	return true;
}

/*コマンド引数で入力
1:読み取るファイルのテンプレ
2:読み取るファイルの拡張子
3:作成ファイルの数
*/
int main(int argc,char *argv[]){
	//for(int i=atoi(argv[3]);i<=atoi(argv[4]);i++){
	graph G;
	srand((unsigned)time(NULL));
	if(read_vertex(G,argv[1],atoi(argv[3]),argv[2])){//中身がtrueの場合実行
		graph_shuffle(G,argv[1],atoi(argv[3]),argv[2]);
	}
}