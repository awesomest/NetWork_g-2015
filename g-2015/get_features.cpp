#include "hpp/init.hpp"
#include "hpp/file_operation.hpp"
#include "hpp/check_feature.hpp"

bool graph_shuffle(graph &G, string temp_name, string file_path, int loop_num){
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

	for(int shuffle_num = 0; shuffle_num < loop_num; shuffle_num++){
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

		// 組み換えデータ作成
		ofstream ofs;
		string file_name = file_path+"shuffle/"+temp_name+"_"+to_string(shuffle_num)+".dat";
		ofs.open(file_name.c_str());
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

bool degreeCorrelation(graph &G, string temp_name, string file_path) {
	vector<int> deg_neighbor(G.vertex_num,0), degrees(G.vertex_num,0);
	for (int i = 0; i < G.vertex_num; i++) {
		int k = G.V[i].degree;
		degrees[k]++;
		multiset<int>::iterator it = G.V[i].edge.begin();
		for ( ; it != G.V[i].edge.end(); it++) {
			deg_neighbor[k] += G.V[*it].degree;
		}
	}

	string file_name = file_path+"original_degree_correlation.dat";
	ofstream ofs;
	ofs.open(file_name.c_str());
	if(!ofs.fail()) {
		for (int i = 0; i < G.vertex_num; i++) {
			if (deg_neighbor[i] == 0) continue;
			double deg_cor = (double)deg_neighbor[i] / (i*degrees[i]);
			ofs << i << " " << deg_cor << endl;
		}
		ofs.close();
	} else {
		return false;
	}
	return true;
}
/*コマンド引数で入力
1:ファイルのテンプレ
2:頂点数
3:元データ数
4:組み換え回数
*/
int main(int argc,char *argv[]){
	//for(int i=atoi(argv[3]);i<=atoi(argv[4]);i++){
	string temp_name = argv[1];
	graph G;
	srand((unsigned)time(NULL));
	for (int i = 0; i < stoi(argv[3]); i++) {
		string origin_name = temp_name+"_data/"+temp_name;
		string file_path = "uncorrelated/"+temp_name+"_data/"+temp_name+"_"+to_string(i)+"/";
		if(read_vertex(G,origin_name,i,"dat")){//中身がtrueの場合実行
			string file_name = file_path+"original_distance_feature.dat";
			ofstream ofs;
			ofs.open(file_name.c_str());
			if(!ofs.fail()) {
				unsigned int radius, diameter;
				double ave_dis;
				average_distance(G, radius, diameter, ave_dis);

				ofs << radius   << endl;
				ofs << diameter << endl;
				ofs << ave_dis  << endl;
				ofs.close();
			}
			degreeCorrelation(G,temp_name,file_path);
			graph_shuffle(G,temp_name,file_path,stoi(argv[4]));
		}
	}
}
