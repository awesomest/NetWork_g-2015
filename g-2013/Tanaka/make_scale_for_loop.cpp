//【指定したファイル数、スケールフリーグラフを制作するプログラム】2013.01/19,00:39 Yuho.Tanaka
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<sstream>
#include<time.h>

using namespace std;

struct vertex{
    int number;
    vector<vertex*>edge;
};

//プロトタイプ宣言
void make_scale(int number,string file_name,string extension,int vertex_max);
string file_fullname(int number,string file_name,string extension);

/*コマンド引数で入力
 1:生成するファイルの名前
 2:生成するファイルの拡張子
 3:最大頂点数
 4:作成ファイル数の始まり
 5 作成ファイル数の終わり
*/
int main(int argc,char *argv[]){
    srand((unsigned int)time(NULL));
    
    for(int i=atoi(argv[4]);i<=atoi(argv[5]);i++){
        make_scale(i,argv[1],argv[2],atoi(argv[3]));
        cout<<"finish graph "<<i<<endl;
    }
    return 0;
}

//ファイルに出力
void make_scale(int number,string file_name,string extension,int vertex_max){
    
    vector<int> array;
    ofstream graph;
    
    //ファイルの読み込み
    graph.open(file_fullname(number,file_name,extension).c_str());
    
    graph<<vertex_max<<" "<<vertex_max-1<<endl;
    array.push_back(0);
    array.push_back(1);
    graph<<0<<" "<<1<<endl;
    
    for(int i=1;i<vertex_max-1;i++){ //i=1(?)
        int t;
        t=rand()%(2*i);//問題箇所
        array.push_back(array[t]);
        array.push_back(i+1);
        graph<<array[t]<<" "<<i+1<<endl;
    }
    graph.close();
}
//ファイルを指定
string file_fullname(int number,string file_name,string extension){
    ostringstream ost;
    string file_number;
    ost << number;
    file_number = ost.str();
    return file_name+"_"+file_number+"."+extension;
}