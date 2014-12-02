#ifndef HEADER_FILE_OPERATION
#define HEADER_FILE_OPERATION

#include "init.hpp"

//---------------------------------
//
//ファイル操作
//

//ファイルを指定
string file_fullname(int number,string file_name,string extension){
	ostringstream ost;
	string file_number;
	ost << number;
	file_number = ost.str();
	return file_name+"_"+file_number+"."+extension;
}

//ファイルの読み込み
bool read_vertex(graph &G, string file_name, int number, string extension){
	ifstream graph;

	graph.open(file_fullname(number,file_name,extension).c_str());
	//ファイルの読み込み（失敗）
	if(graph.fail()){
		cout<<"file open error"<<endl;
		return false;
	}

	//ファイルの読み込み
	//graph.open(file_fullname(number,file_name,extension).c_str());
	int node,edge;
	graph>>node>>edge;
	G.vertex_num=node;
	G.edge_num=edge;
	for(int i=0;i<node;i++){
		vertex a;
		a.number=i;
		a.degree=0;
		G.V.push_back(a);
	}

	//ファイルの格納(頂点：頂点からの接続先)
	for(int i=0;i<edge;i++){
		int t, s;
		graph>>t>>s;
		G.V[t].edge.insert(s);
		G.V[s].edge.insert(t);
		G.V[t].degree++;
		G.V[s].degree++;
	}
	graph.close();

	return true;
}

#endif