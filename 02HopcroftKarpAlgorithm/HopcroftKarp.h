//https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm
#pragma once
#include <vector>
#include <queue>
#include <iterator>
#define MAX 1001
#define NIL 0
#define INF (1<<28)

using namespace std;


	class HopCropCraft
	{
	public:
		vector< int > Adj[MAX];
		vector< int > U;
		vector< int > V;
		int  Pair_U[MAX]= {0}, Pair_V[MAX]={0}, dist[MAX]= {0}, matching=0;
		
		bool bfs();
		bool dfs(int);
		void run();
		HopCropCraft(vector< int > &U, vector< int > &V, vector< int > (&Adj)[MAX]);


	};
