//【各グラフの直径・半径・平均値・平均値をプロットしやすいよう並び替えをするプログラム】2013.01/16,02:54,Yuho.Tanaka
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<sstream>

using namespace std;

//プロトタイプ宣言
bool read_file(string file_name,int file_number,vector<int>&diameter,vector<int>&radius,vector<double>&average);
void output_file(string file_name2,int file_number,vector<int>&diameter,vector<int>&radius,vector<double>&average);

/*コマンド引数で入力
 1:読み取るファイルの名前
 2:書きだすファイルの名前
 3:格納しているグラフの数
*/
int main(int argc,char *argv[]){
    vector<int>diameter;
    vector<int>radius;
    vector<double>average;
    if(read_file(argv[1],atoi(argv[3]),diameter,radius,average)){//中身がtrueの場合実行
        output_file(argv[2],atoi(argv[3]),diameter,radius,average);
    }
    return 0;
}
//読み取り
bool read_file(string file_name,int file_number,vector<int>&diameter,vector<int>&radius,vector<double>&average){
    ifstream graph;
    //ファイルの読み込み
    graph.open(file_name.c_str());
    //ファイルの読み込み（失敗）
    if(graph.fail()){
        cout<<"file open error"<<endl;
        return false;
    }
    //直径・半径・平均値の読み取り
    int d=0,r=0;
    double a=0;
    string LineData;
    for(int i=0;i<file_number;i++){
        graph>>LineData;
        graph>>LineData>>d;
        graph>>LineData>>LineData>>r;
        graph>>LineData>>LineData>>a;
        diameter.push_back(d);
        radius.push_back(r);
        average.push_back(a);
    }
    graph.close();
    return true;
}
//書き出し
void output_file(string file_name2,int file_number,vector<int>&diameter,vector<int>&radius,vector<double>&average){
    //グラフに格納
    ofstream graph;
    graph.open(file_name2.c_str(),ios::app);
    graph<<"#【各グラフの直径・半径・平均値・平均値】"<<endl;
    graph<<"#グラフの番号  "<<"直径  "<<"半径  "<<"平均値"<<endl;
    for(int i=0;i<file_number;i++){
        graph<<i+1<<"   "<<diameter[i]<<"   "<<radius[i]<<"   "<<average[i]<<endl;
    }
    graph.close();
    cout<<"finish"<<endl;
}
