#include "../hpp/init.hpp"
#include "../hpp/file_operation.hpp"
#include "../hpp/check_feature.hpp"

int main(){
	int data_num = 10;
	int shuffle_num = 10;
	double p = 0;
	srand(time(NULL));//シード生成
	for(int data_i = 0; data_i < data_num; data_i++){
		graph G;
		string data_path = "tree_data/modified_BA_10000";
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
					bool selfloop = false;
					tr++;
					if((int)tr % 1000 == 0)
						cout << tr << endl;
					//シャッフルして隣の頂点をつなげる
					random_shuffle(degree.begin(), degree.end());

					graph shuffle_G;
					shuffle_G.vertex_num = G.vertex_num;
					shuffle_G.edge_num = G.edge_num;
					for(int i = 0; i < shuffle_G.vertex_num; i++){
						vertex a;
						a.number=i;
						a.degree=0;
						shuffle_G.V.push_back(a);
					}

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
					//連結ならOK
					if(is_connected(shuffle_G)){
						cout << data_i << " " << shuffle_i << " Success!" << endl;
						//シャッフデータ保存
						ofstream ofs;
						string file_name = "modified_BA_10000_"+to_string(data_i)+"_"+to_string(shuffle_i)+".dat";
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
	p /= double(data_num*shuffle_num);
	ofs << p << endl;
	ofs.close();
}