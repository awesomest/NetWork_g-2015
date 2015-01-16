#ifndef HEADER_INIT
#define HEADER_INIT

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <set>
#include <map>
#include <time.h>
#include <queue>
#include <random>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <sstream>
#include <cstdlib>
using namespace std;

struct vertex{
	int number,degree;
	multiset<int> edge;
};

struct graph{
	int vertex_num,edge_num;
	vector<vertex> V;
};

//ランダム
//0~x-1までを返す
int my_rand(int x){
	return (int)(((double)(rand())/(RAND_MAX+1.0))*(x));
}

#endif