// TSP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <string>
#include <sstream>
#include <array>
#include <random>
#include <algorithm>
#include <time.h>

long double totalDistance(std::vector<std::pair<long double, long double>>& nodes, std::vector<int>& indices, int n)
{
	long double distance = 0;
	for (int x = 0; x < (indices.size() - 1); x++)
	{
		distance += std::sqrt((((nodes[indices[x]].first) - (nodes[indices[(x + 1) % n]].first)) * ((nodes[indices[x]].first) - (nodes[indices[(x + 1) % n]].first))) + (((nodes[indices[x]].second) - (nodes[indices[(x + 1) % n]].second)) * ((nodes[indices[x]].second) - (nodes[indices[(x + 1) % n]].second))));
	}
	int xx = indices.size() - 1;
	int x = 0;
	distance += std::sqrt((((nodes[indices[x]].first) - (nodes[indices[(xx)]].first)) * ((nodes[indices[x]].first) - (nodes[indices[(xx)]].first))) + (((nodes[indices[x]].second) - (nodes[indices[(xx)]].second)) * ((nodes[indices[x]].second) - (nodes[indices[(xx)]].second))));
	return distance;
}

long double calcDistance(long double x1, long double y1, long double x2, long double y2)
{
	long double distance = std::sqrt((((x1)-(x2)) * ((x1)-(x2))) + (((y1)-(y2)) * ((y1)-(y2))));
	return distance;
}



void anneal(std::vector<std::pair<long double, long double>>& nodes, std::vector<int>& best_solution, long double& bestDis) {
	std::vector<int> cur_solution = best_solution;
	long double currDis = bestDis;
	int N = best_solution.size();
	double T = sqrt(N);
	double stopping_temperature = 1e-10;
	double iteration = 0;
	double stopping_iter = 1000000;
	double alpha = 0.99998;
	srand(time(NULL));
	std::cout << "Starting annealing." << std::endl;
	while (T >= stopping_temperature && iteration < stopping_iter) {
		
		std::vector<int> candidate = cur_solution;
		int l = 2 + rand() % (N - 1);
		int i = rand() % (N - l + 1);
		std::reverse(candidate.begin() + i, candidate.begin() + i + l);
		long double candidateDis = totalDistance(nodes, candidate, N);
		if (candidateDis < currDis) {
			currDis = candidateDis;
			cur_solution = candidate;
			if (candidateDis < bestDis) {
				best_solution = candidate;
				bestDis = candidateDis;
			}
		}
		else {
			if (((double) rand() / RAND_MAX) < (exp(-std::abs(candidateDis - currDis) / T))) {
				currDis = candidateDis;
				cur_solution = candidate;
			}
		}
		T *= alpha;
		++iteration;
	}
}

void twoOpt(std::vector<std::pair<long double, long double>>& nodes, std::vector<int>& indices)
{
	bool foundImprovement = true;
	int n = indices.size();
	int totalRun = 0;
	while (foundImprovement) {
		if (n >= 33000) {
			if (totalRun == 1) {
				break;
			}
			totalRun += 1;
		}
		foundImprovement = false;
		for (int i = 0; i <= n - 1; i++) {
			for (int j = i + 1; j <= n - 1; j++) {
				long double currDistance = calcDistance(nodes[indices[i]].first, nodes[indices[i]].second, nodes[indices[(i + 1) % n]].first, nodes[indices[(i + 1) % n]].second);
				currDistance += calcDistance(nodes[indices[j]].first, nodes[indices[j]].second, nodes[indices[(j + 1) % n]].first, nodes[indices[(j + 1) % n]].second);

				long double afterSwapDistance = calcDistance(nodes[indices[i]].first, nodes[indices[i]].second, nodes[indices[j]].first, nodes[indices[j]].second);
				afterSwapDistance += calcDistance(nodes[indices[(i + 1) % n]].first, nodes[indices[(i + 1) % n]].second, nodes[indices[(j + 1) % n]].first, nodes[indices[(j + 1) % n]].second);

				if (afterSwapDistance < currDistance) {
					std::cout << "Swapping edges " << indices[i] << "---" << indices[(i + 1) % n] << "&" << indices[j] << "---" << indices[(j + 1) % n] << " <with> " << indices[i] << "---" << indices[j] << "&" << indices[(i + 1) % n] << "---" << indices[(j + 1) % n] << "\n";
					std::reverse((indices.begin() + ((i + 1))), (indices.begin() + ((j + 1))));
					foundImprovement = true;
					std::cout << "configuration after swap\n";
					for (std::vector<int>::const_iterator i = indices.begin(); i != indices.end(); ++i) {
						std::cout << *i << ' ';
					}
					std::cout << "\n New total Distance is ";
					std::cout << totalDistance(nodes, indices, n);
				}
			}
		}
	}
}

int TSP(std::string fileName)
{
	if (fileName.empty())
		return 1;

	// read the lines out of the file
	std::ifstream infile;
	infile.open("tmp.data", std::ifstream::in);

	int nodeCount;
	infile >> nodeCount;
	std::vector<std::pair<long double, long double>> nodes;
	std::vector<int> indices;
	//int *items = new int[numItems];
	//int *weights = new int[numItems];
	long double x, y = 0;
	int index = 0;
	while (infile >> x >> y)
	{
		nodes.push_back(std::make_pair(x, y));
		indices.push_back(index);
		index++;
	}

	// greedy
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, (nodeCount - 1));
	int i = 0, xx = dist6(rng);
	std::vector<int> visited;
	long double bestNodeDist = std::numeric_limits<long double>::max();
	int bestNodesIndex = -1;
	long double totalDis = 0;

	if (nodeCount < 33810)
	{
		while (i < nodeCount)
		{
			for (int j = 0; j < nodeCount; j++)
			{
				if ((xx != j) && ((std::find(visited.begin(), visited.end(), j) == visited.end())))
				{
					long double tempDist = calcDistance(nodes[xx].first, nodes[xx].second, nodes[j].first, nodes[j].second);
					if (tempDist < bestNodeDist)
					{
						bestNodeDist = tempDist;
						bestNodesIndex = j;
					}
				}
			}
			visited.push_back(xx);
			if (i != (nodeCount - 1))
			{
				totalDis += bestNodeDist;
			}
			xx = bestNodesIndex;
			bestNodeDist = std::numeric_limits<long double>::max();
			i++;
		}
	}
	else
	{
		std::random_shuffle(indices.begin(), indices.end());
		visited = indices;
	}

	/* while (i < nodeCount){
		visited.push_back(i);
		i += 1;
	} */



	long double bestDis = totalDistance(nodes, visited, visited.size());
	//twoOpt(nodes, visited);
	//bestDis = totalDistance(nodes, visited, visited.size());
	anneal(nodes, visited, bestDis);
	twoOpt(nodes, visited);
	bestDis = totalDistance(nodes, visited, visited.size());

	std::ostringstream dis;
	dis.precision(1);
	dis << std::fixed << bestDis;

	std::string outputStr = dis.str() + " " + std::to_string(0) + "\n";

	std::cout << dis.str() << " " << 0 << "\n";
	std::string str;
	for (int x = 0; x < visited.size(); x++)
	{

		str.append(std::to_string(visited[x]));
		str.append(" ");

	}
	std::cout << str.substr(0, str.size() - 1);
	outputStr.append(str.substr(0, str.size() - 1));
	std::ofstream out("output.txt");
	out << outputStr;
	out.close();
	return 0;
}


int main(int argc, char** argv)
{
	try
	{
		std::string fileName;
		fileName = "tmp.data";
		TSP(fileName);

	}
	catch (...)
	{
		return (1);
	}
}
