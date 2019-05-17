//https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm
#include "HopcroftKarp.h"
#include<iostream>
using namespace std;


	bool HopCropCraft::bfs()
	{
		queue< int > Q;
		for (int u : U)
		{
			if (Pair_U[u] == NIL)
			{
				dist[u] = 0;
				Q.push(u);
			}
			else
			{
				dist[u] = INF;
			}
		}
		dist[NIL] = INF;
		while (!Q.empty())
		{
			int u = Q.front(); Q.pop();
			if (dist[u] < dist[NIL])
			{
				for (int v : Adj[u])
				{
					if (dist[Pair_V[v]] == INF)
					{
						dist[Pair_V[v]] = dist[u] + 1;
						Q.push(Pair_V[v]);
					}
				}
			}
		}
		return (dist[NIL] != INF);
	}

	bool HopCropCraft::dfs(int u)
	{
		if (u != NIL)
		{
			for (int v : Adj[u])
			{
				if (dist[Pair_V[v]] == dist[u] + 1)
				{
					if (dfs(Pair_V[v]) == true)
					{
						Pair_V[v] = u;
						Pair_U[u] = v;
						return true;
					}
				}
			}
			dist[u] = INF;
			return false;
		}
		return true;
	}

	HopCropCraft::HopCropCraft(std::vector< int > &U, std::vector< int > &V, std::vector< int > (&Adj)[MAX])
	{
		this->U = U;
		this->V = V;
		std::copy(std::begin(Adj), std::end(Adj), std::begin(this->Adj));
	}

	void HopCropCraft::run()
	{
		while (bfs())
		{
			for (int u : U)
			{
				if (Pair_U[u] == NIL)
				{
					if (dfs(u) == true)
					{
						matching = matching + 1;
					}
				}
			}
		}
		int k = matching;
		return;
	}
	
	int main() {
	vector< int > Adj[MAX];
	static const int arr1[] =  { 1,2,3,4 };
	vector< int > U (arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]) );
	static const int arr2[] =  {5,6,7,8};
	vector< int > V (arr2, arr2 + sizeof(arr2) / sizeof(arr2[0]) );
	Adj[1].push_back(6);
	Adj[1].push_back(7);
	Adj[2].push_back(8);
	Adj[3].push_back(5);
	Adj[3].push_back(7);
	Adj[4].push_back(5);
	HopCropCraft hopCropCraft(U, V, Adj);
	hopCropCraft.run();
	std::cout << hopCropCraft.matching;
	return 0;
}
