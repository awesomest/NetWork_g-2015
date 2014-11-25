#ifndef HEADER_CHECK_FEATURE
#define HEADER_CHECK_FEATURE

#include "init.hpp"
#include "file_operation.hpp"

//------------------------------------
//
//ネットワークの特徴
//

//グラフが自己ループか判定するプログラム
bool is_selfloop(graph &G){
	for(int i=0;i<G.V.size();i++){
		if (G.V[i].edge.count(i)) return true;
	}
	return false;
}

//構造体からグラフが多重辺を持つか判定する
bool is_multiple_edge(graph &G){
	for(int i=0;i<G.vertex_num;i++){
		if (G.V[i].edge.empty()) continue;
		set<int>::iterator it = G.V[i].edge.begin();
		while (1) {
			// 頂点数１のときここでエラー？
			set<int>::iterator it_prev = it++;
			if (it == G.V[i].edge.end()) break;
			if (*it_prev == *it) return true;
		}
	}
	return false;
}

//非連結があるか
//あるときにtrueを返す
bool is_connected(graph &G){
	set<int>visited;
	queue<int> fringe;
	set<int>::iterator it;

	fringe.push(0);
	visited.insert(0);

	while(!fringe.empty()){
		int pos = fringe.front();
		fringe.pop();

		set<int>::iterator it = G.V[pos].edge.begin();
		while (it != G.V[pos].edge.end()) {
			if (!visited.count(*it)){
				fringe.push(*it);
				visited.insert(*it);
			}
			it++;
		}
	}

	if(visited.size()==G.vertex_num) return true;
	else return false;
}

//平均距離
//リファレンスで定義したものに半径、直径、平均距離を代入する
void average_distance(
	graph &G, 				//ネットワーク
	unsigned int &radius, 	//meinで表示するために
	unsigned int &diameter, //
	double &ave_dis			//
	)
{
	radius = INT_MAX;
	diameter = 0;
	ave_dis = 0;
	for(int i = 0; i < G.vertex_num; i++){
		vector<bool> visited(G.vertex_num);
		for(int j = 0; j < G.vertex_num; j++){
			visited[j] = false;
		}
		visited[i] = true;
		unsigned int dis = 0;
		queue<int> queue;
		queue.push(i);
		unsigned int v_longest = 0;
		while(!queue.empty()){
			dis++;
			int step = queue.size();
			while(step--){
				int pop = queue.front();
				queue.pop();
				multiset<int>::iterator ite;
				for(ite = G.V[pop].edge.begin(); ite != G.V[pop].edge.end(); ite++){
					if(!visited[*ite]){
						visited[*ite] = true;
						ave_dis += dis;
						queue.push(*ite);
						v_longest = dis;
					}
				}
			}
		}
		radius = min(radius, v_longest);
		diameter = max(diameter, v_longest);
	}
	ave_dis /= (G.vertex_num * (G.vertex_num-1.0));
}

//次数分布をプロットする用のファイル作成
//成功したらtrueを返す
bool degree_distributions(
	graph& G,			//ネットワーク
	string file_name, 	//つくるファイルのテンプレ
	int number, 		//ファイルの番号
	string extension	//ファイルの拡張子
	)
{

	vector<int> degree_count(G.vertex_num);
	for(int i = 0; i < degree_count.size(); i++){
		degree_count[i] = 0;
	}

	for(int i = 0; i < G.vertex_num; i++) degree_count[G.V[i].edge.size()]++;

	ofstream graph;
	graph.open(file_fullname(number, file_name+"_DDplot", extension).c_str());
	//ファイルの読み込み（失敗）
	if(graph.fail()){
		cout<<"file open error"<<endl;
		return false;
	}

	for(int i = 0; i < degree_count.size(); i++){
		if(degree_count[i] != 0){
			graph << i << " " << degree_count[i]/(G.edge_num*2.0) << endl;
		}
	}
	graph.close();

	return true;
}

#endif