//【直径・半径・平均値の個数を数えるプログラム】2013.01/30,12:44,Yuho.Tanaka
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<sstream>

using namespace std;

//プロトタイプ宣言
bool read_file(string file_name,int file_number,vector<int>&diameter,vector<int>&radius,vector<double>&average);
void count(int file_number,int max_diameter,int max_radius,int max_average,vector<int>&diameter,vector<int>&radius,vector<double>&average,int diameter_count[],int radius_count[],double average_count[]);
void diameter_output(string file_name2,int file_number,vector<int>&diameter,int diameter_count[]);
void radius_output(string file_name3,int file_number,vector<int>&radius,int radius_count[]);
void average_output(string file_name4,int file_number,vector<double>&average,double average_count[]);


/*コマンド引数で入力
 1:読み取るファイルの名前
 2:書きだすファイルの名前
 3:書きだすファイルの名前
 4:書きだすファイルの名前
 5:格納しているグラフの数
*/
int main(int argc,char *argv[]){
    
    vector<int>diameter;
    vector<int>radius;
    vector<double>average;
    
    if(read_file(argv[1],atoi(argv[5]),diameter,radius,average)){//中身がtrueの場合実行
	int max_diameter = *max_element(diameter.begin(), diameter.end());
	int max_radius = *max_element(radius.begin(), radius.end());		
	int max_average = *max_element(average.begin(), average.end());

    	int diameter_count[max_diameter];
    	int radius_count[max_radius];
    	double average_count[max_average];
      
	count(atoi(argv[5]),max_diameter,max_radius,max_average,diameter,radius,average,diameter_count,radius_count,average_count);
        diameter_output(argv[2],max_diameter,diameter,diameter_count);
        radius_output(argv[3],max_radius,radius,radius_count);
        average_output(argv[4],max_average,average,average_count);
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
    
   // graph>>LineData;
    graph>>LineData>>LineData>>LineData>>LineData;
    for(int i=0;i<file_number;i++){
        graph>>LineData>>d>>r>>a;
        diameter.push_back(d);
        radius.push_back(r);
        average.push_back(a);
    }
    graph.close();
  	
    return true;
}


//直径、半径、平均値の個数を数える
void count(int file_number,int max_diameter,int max_radius,int max_average,vector<int>&diameter,vector<int>&radius,vector<double>&average,int diameter_count[],int radius_count[],double average_count[]){
    //初期化
    for(int i=0;i<=max_diameter;i++){
        diameter_count[i]=0;
	}
    for(int i=0;i<=max_radius;i++){
        radius_count[i]=0;
	}
    for(int i=0;i<=max_average;i++){
        average_count[i]=0;
	}
    //個数を数える
    for(int i=0;i<file_number;i++){
        diameter_count[diameter[i]]++;
	radius_count[radius[i]]++;
	average_count[(int)average[i]]++;
    }

}
//直径の個数の書き出し
void diameter_output(string file_name2,int file_number,vector<int>&diameter,int diameter_count[]){
    //グラフに格納
    ofstream graph;
    graph.open(file_name2.c_str(),ios::app);
    graph<<"#【直径の個数】"<<endl;
    graph<<"#直径  個数"<<endl;
    for(int i=0;i<=file_number;i++){
        if(diameter_count[i]!=0){
            graph<<i<<"   "<<diameter_count[i]<<endl;
        }
    }
    graph.close();
}
//半径の個数の書き出し
void radius_output(string file_name3,int file_number,vector<int>&radius,int radius_count[]){
    //グラフに格納
    ofstream graph;
    graph.open(file_name3.c_str(),ios::app);
    graph<<"#【半径の個数】"<<endl;
    graph<<"#半径  個数"<<endl;
    for(int i=0;i<=file_number;i++){
        if(radius_count[i]!=0){
            graph<<i<<"   "<<radius_count[i]<<endl;
        }
    }
    graph.close();
}
//平均値の個数の書き出し
void average_output(string file_name4,int file_number,vector<double>&average,double average_count[]){
    //グラフに格納
    ofstream graph;
    graph.open(file_name4.c_str(),ios::app);
    graph<<"#【平均値の個数】"<<endl;
    graph<<"#平均値　個数"<<endl;
    for(int i=0;i<=file_number;i++){
        if(average_count[i]!=0){
            graph<<i<<"   "<<average_count[i]<<endl;
        }
    }
    graph.close();
}
