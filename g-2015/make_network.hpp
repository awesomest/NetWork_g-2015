#ifndef HEADER_MAKE_NETWORK
#define HEADER_MAKE_NETWORK

#include "init.hpp"
#include "file_operation.hpp"

//---------------------------------
//
//ネットワーク作成
//

//BAモデル作成
void make_BA(
	int num_file,					//ネットワークの番号
	int complete_graph_vertex_num,	//初めの完全グラフの頂点数
	int add_vertex_num,				//追加する頂点数
	int m,							//新しい頂点の枝数
	string file_name,				//出力ファイル名のテンプレ
	string extension				//出力ファイル名の拡張子
	)
{

	ofstream graph;
	graph.open(file_fullname(num_file,file_name,extension).c_str());
	int num_v = complete_graph_vertex_num+add_vertex_num,
			num_e = (complete_graph_vertex_num*(complete_graph_vertex_num-1))/2 + add_vertex_num*m;
	graph << num_v << " " << num_e << endl;

	vector<int> m_vertices;
	set<int> selected_vertices;

	for(int i=0;i<complete_graph_vertex_num;i++){//完全グラフの作成
		for(int j=i+1;j<complete_graph_vertex_num;j++){
			m_vertices.push_back(i);
			m_vertices.push_back(j);
			graph << i << " " << j << endl;
		}
	}

	for(int i=0;i<add_vertex_num;i++){//追加する頂点のループ
		int number=i+complete_graph_vertex_num;

		while(selected_vertices.size()<m){//追加する辺のループ
			int t =my_rand(m_vertices.size());
			if(!selected_vertices.count(m_vertices[t])){//選ばれていなかった
				selected_vertices.insert(m_vertices[t]);
			}
		}
		set<int>::iterator it = selected_vertices.begin();
		for ( ; it != selected_vertices.end(); it++) {
			m_vertices.push_back(*it);
			m_vertices.push_back(number);
			graph << *it << " " << number << endl;
		}
		selected_vertices.clear();
	}

	graph.close();
}

#endif