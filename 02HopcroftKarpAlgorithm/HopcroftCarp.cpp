//https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm

#include "pch.h"
#include <iostream>

#define MAX 100001
#define NIL 0
#define INF (1<<28)
#include <vector>
#include <queue>
using namespace std;

vector< int > Adj[MAX];
vector< int > U;
vector< int > V;
int  Pair_U[MAX], Pair_V[MAX], dist[MAX];
// n: number of nodes on left side, nodes are numbered 1 to n
// m: number of nodes on right side, nodes are numbered n+1 to n+m
// Adj = NIL[0] ∪ G1[Adj[1---n]] ∪ G2[Adj[n+1---n+m]]

bool bfs() 
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

bool dfs(int u)
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

int hopcroft_karp() {
	int matching = 0;
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
	return matching;
}

int main() {
	U = { 1,2,3,4 };
	V = {5,6,7,8};
	for(int u : U)
	{
		Pair_U[u] = NIL;
	}
	for (int v : V)
	{
		Pair_V[v] = NIL;
	}
	Adj[1].push_back(6);
	Adj[1].push_back(7);
	Adj[2].push_back(8);
	Adj[3].push_back(5);
	Adj[3].push_back(7);
	Adj[4].push_back(5);

	printf("%d\n", hopcroft_karp());
	return 0;
}