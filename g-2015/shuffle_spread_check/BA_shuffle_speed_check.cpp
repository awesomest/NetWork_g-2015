#include "../hpp/init.hpp"
#include "../hpp/file_operation.hpp"
#include "../hpp/check_feature.hpp"

int sh_count;
bool completed;
graph remake(graph shuffle_G, vector<int> degree) {
	//cout << sh_count << endl;
	if (++sh_count % 20 == 0) {
		completed = false;
		return shuffle_G;
	}

	random_shuffle(degree.begin(), degree.end());

	vector<int> left;
	for(int i = 0; i < degree.size(); i++){
		int a = degree[i];
		int b = degree[++i];
		if(a == b || shuffle_G.V[a].edge.count(b) > 0 || shuffle_G.V[b].edge.count(a) > 0) { //selfloop = true;
			left.push_back(a);
			left.push_back(b);
			continue;
		}
		shuffle_G.V[a].edge.insert(b);
		shuffle_G.V[b].edge.insert(a);
		shuffle_G.V[a].degree++;
		shuffle_G.V[b].degree++;
	}
	cout << "left size: " << left.size() << endl;
	if (left.size() == 0) return shuffle_G;
	//if(selfloop) return false;
	//if(is_multiple_edge(shuffle_G)) return false;

	return remake(shuffle_G, left);
}

int main(){
	int data_num = 10;
	int shuffle_num = 10;
	double p = 0;
	srand(time(NULL));//シード生成
	for(int data_i = 0; data_i < data_num; data_i++){
		graph G;
		string data_path = "BA_data/BA_10000";
		if(read_vertex(G,data_path,data_i,"dat")){//中身がtrueの場合実行
			//次数の数を数える
			vector<int> degree;
			for(int i = 0; i < G.vertex_num; i++)
				for(int j = 0; j < G.V[i].edge.size(); j++)
					degree.push_back(i);

			//組み替えが出来るまで試す
			for(int shuffle_i = 0; shuffle_i < shuffle_num; shuffle_i++){
				double tr = 0;
				do{
					//bool selfloop = false;
					tr++;
					//if((int)tr % 100 == 0)
						cout << "try: " << tr << endl;
					//シャッフルして隣の頂点をつなげる
					//random_shuffle(degree.begin(), degree.end());

					graph shuffle_G;
					shuffle_G.vertex_num = G.vertex_num;
					shuffle_G.edge_num = G.edge_num;
					for(int i = 0; i < shuffle_G.vertex_num; i++){
						vertex a;
						a.number=i;
						a.degree=0;
						shuffle_G.V.push_back(a);
					}
					/*

					for(int i = 0; i < degree.size(); i++){
						int a = degree[i];
						int b = degree[++i];
						if(a == b) selfloop = true;
						shuffle_G.V[a].edge.insert(b);
						shuffle_G.V[b].edge.insert(a);
						shuffle_G.V[a].degree++;
						shuffle_G.V[b].degree++;
					}
					if(selfloop)
						continue;

					if(is_multiple_edge(shuffle_G)) continue;
					*/
					sh_count = 0;
					completed = true;
					shuffle_G = remake(shuffle_G, degree);
					if (!completed) continue;

					//連結ならOK
					if(is_connected(shuffle_G)){
						cout << data_i << " " << shuffle_i << " Success!" << endl;
						//シャッフデータ保存
						ofstream ofs;
						string file_name = "shuffle_BA_10000_"+to_string(data_i)+"_"+to_string(shuffle_i)+".dat";
						ofs.open(file_name.c_str());
						if(ofs.fail())
							cout << "NG: shuffle data not open" << endl;
						ofs << shuffle_G.vertex_num << " " << shuffle_G.edge_num << endl;
						for(int i = 0; i < shuffle_G.vertex_num; i++){
							multiset<int>::iterator ite = shuffle_G.V[i].edge.begin();
							for(; ite != shuffle_G.V[i].edge.end(); ite++){
								if(i <= *ite) continue;
								ofs << i << " " << *ite << endl;
							}
						}
						ofs.close();

						break;
					}
				}while(true);

				p += tr;
				//試行回数記録
				ofstream ofs;
				string file_name = "shuffle_rty.dat";
				ofs.open(file_name.c_str());
				if(ofs.fail())
					cout << "NG: try data not open" << endl;
				ofs << tr << endl;
				ofs.close();
			}

		}else{
			cout << "Graph not open." << endl;
		}
		ofstream ofs;
		string file_name = "shuffle_rty.dat";
		ofs.open(file_name.c_str());
		if(ofs.fail())
			cout << "NG: try data not open" << endl;
		ofs << endl;
		ofs.close();
	}
	ofstream ofs;
	string file_name = "shuffle_rty.dat";
	ofs.open(file_name.c_str());
	if(ofs.fail())
		cout << "NG: try data not open" << endl;
	ofs << p << endl;
	p = data_num*shuffle_num / p;
	ofs << p << endl;
	ofs.close();
}
