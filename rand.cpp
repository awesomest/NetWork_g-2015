#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <vector>
using namespace std;

int main(int argc,char **argv){
	int N;
	int edge = 0;
	double p;
	// printf("N = %s\n",argv[1]);
	// printf("p = %s\n",argv[2]);
	N = atoi(argv[1]);
	p = atof(argv[2]);
	// printf("N = %d\n",N);
	
	// printf("p = %f\n",p);
	//printf("N = ");
	// scanf("%d", &N);
	// //printf("p = ");
	// scanf("%lf", &p);
	vector<vector<bool> > network(N, vector<bool>(N, false));
	srand(N/*(unsigned)time( NULL )*/);
	for( int i = 0; i < N; i++ ){
		for( int j = i+1; j < N; j++ ){
			double prob = (double)rand() / RAND_MAX;
			//cout << prob << " " << RAND_MAX << endl;
			if( prob < p ){
				network[i][j] = true;
				network[j][i] = true;
				edge++;
			}
		}
	}
	/*
	for( int i = 0; i < N; i++ ){
		for( int j = 0; j < N; j++ ){
			cout << network[i][j] << " ";
		}
		cout << endl;
	}
	*/
	cout << N << " " << edge << endl;
	for( int i = 0; i < N; i++ ){
		for( int j = i + 1; j < N; j++ ){
			if( network[i][j] ) cout << i << " " << j << endl;
		}
	}
}
