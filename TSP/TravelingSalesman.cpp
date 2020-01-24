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

long double totalDistance(std::vector<std::pair<long double, long double>> &nodes, std::vector<int> &indices)
{
	long double distance = 0;
	for (int x = 0; x < (indices.size()-1); x++)
	{
		distance += std::sqrt((((nodes[indices[x]].first) - (nodes[indices[(x+1)]].first)) * ((nodes[indices[x]].first) - (nodes[indices[(x+1)]].first))) + (((nodes[indices[x]].second) - (nodes[indices[(x+1)]].second)) * ((nodes[indices[x]].second) - (nodes[indices[(x+1)]].second))));
	}
	int xx = indices.size() - 1;
	int x = 0;
	distance += std::sqrt((((nodes[indices[x]].first) - (nodes[indices[(xx)]].first)) * ((nodes[indices[x]].first) - (nodes[indices[(xx)]].first))) + (((nodes[indices[x]].second) - (nodes[indices[(xx)]].second)) * ((nodes[indices[x]].second) - (nodes[indices[(xx)]].second))));
	return distance;
}

long double calcDistance(long double x1, long double y1, long double x2, long double y2)
{
	long double distance = std::sqrt((((x1) - (x2)) * ((x1) - (x2))) + (((y1) - (y2)) * ((y1) - (y2))));
	return distance;
}


bool configurationAlreadyPresent(std::vector<std::string> &configurations, std::string tempS)
{
	
	for (int i = 0; i < configurations.size(); i++)
	{
		std::string conf = configurations[i];
		conf = conf + conf;
		if (conf.find(tempS) != std::string::npos)
		{
			return true;
		}
	}
	return false;
}

/*
an edge to be broken can not be an edge previously joined,
same way an edge to be joined can not be an edge previously broken.
*/
bool NotRepeatedEdgeToBeBrokenJoined(std::vector<std::pair<int, int>> &joined_edges, std::vector<std::pair<int, int>> &broken_edges, int s, int e, int n, std::vector<int> &indices)
{
	//check if edge to be joined(indices[e] indices[n]) and (indices[s] indices[n-1])) is present in previously broken edges aka broken_edges
	int v1 = indices[e];
	int v2 = indices[n];

	int x1 = indices[s];
	int temp = (n == 0) ? (indices.size() - 1) : (n - 1);
	int x2 = indices[temp];

	for (int i = 0; i < broken_edges.size(); i++)
	{
		int y1 = broken_edges[i].first;
		int y2 = broken_edges[i].second;

		if ((v1 == y1 && v2 == y2) || (v1 == y2 && v2 == y1) || (x1 == y1 && x2 == y2) || (x1 == y2 && x2 == y1)) {
			return false;
		}
	}

	//check if edges to be broken((indices[s] indices[e]) and (indices[n] and indices[n-1])) is present in previously joined edges aka joined_edges
	/*
	v1 = indices[s];
	v2 = indices[e];

	x1 = indices[n];
	temp = (n == 0) ? (indices.size() - 1) : (n - 1);
	x2 = indices[temp];
	
	for (int i = 0; i < joined_edges.size(); i++)
	{
		int y1 = joined_edges[i].first;
		int y2 = joined_edges[i].second;
		if ((v1 == y1 && v2 == y2) || (v1 == y2 && v2 == y1) || (x1 == y1 && x2 == y2) || (x1 == y2 && x2 == y1)) {
			return false;
		}

	}
	*/
	return true;
}
bool notPresent(int i, std::vector<int> &indices, std::vector<int> &nToIgnore)
{
	for (int ii = 0; ii < nToIgnore.size(); ii++)
	{
		if (i == indices[nToIgnore[ii]])
		{
			return false;
		}
	}
	return true;
}

int searchNextN(long double currentDistance, std::vector<int> &indices, std::vector<std::pair<long double, long double>> &nodes, std::vector<int> &nToIgnore, int s, int e, int a, std::vector<std::pair<int, int>> &broken_edges, std::vector<std::pair<int, int>> &joined_edges)
{
	int n = -1;
	long double minDis = currentDistance;
	for (int i = 0; i < indices.size(); i++)
	{
		if (indices[i] != indices[s] && indices[i] != indices[e] && indices[i] != indices[a] && NotRepeatedEdgeToBeBrokenJoined(joined_edges, broken_edges, s, e, i, indices) && notPresent(indices[i], indices, nToIgnore))
		{

			long double temp = calcDistance(nodes[indices[e]].first, nodes[indices[e]].second, nodes[indices[i]].first, nodes[indices[i]].second);
			if (minDis > temp)
			{
				minDis = temp;
				n = i;
			}
		}
	}
	return n;
}

int searchNextN_SandBox(long double currentDistance, std::vector<int> &indices, std::vector<std::pair<long double, long double>> &nodes, int s, int e, int a, std::vector<std::pair<int, int>> &broken_edges, std::vector<std::pair<int, int>> &joined_edges)
{
	int n = -1;
	long double minDis = currentDistance;
	for (int i = 0; i < indices.size(); i++)
	{
		if (indices[i] != indices[s] && indices[i] != indices[e] && indices[i] != indices[a] && NotRepeatedEdgeToBeBrokenJoined(joined_edges, broken_edges, s, e, i, indices))
		{

			long double temp = calcDistance(nodes[indices[e]].first, nodes[indices[e]].second, nodes[indices[i]].first, nodes[indices[i]].second);
			if (minDis > temp)
			{
				minDis = temp;
				n = i;
			}
		}
	}
	return n;
}

void generateConfiguration(std::vector<int> &indices, int e, int n, int s, int a, int &nextS)
{
	int tempE = e, tempN = n, tempStore = 0;
	int flagS = 0;
	if ((tempN + 1) == indices.size())
	{
		tempStore = indices[0];
		indices[0] = indices[tempN];
		tempN = 0;
	}
	else
	{
		tempStore = indices[tempN + 1];
		indices[tempN + 1] = indices[tempN];
		tempN += 1;
	}
	if (tempN == s)
	{
		flagS = 1;
	}
	while (tempN != tempE)
	{
		if ((tempN + 1) == indices.size())
		{
			int tempVar = indices[0];
			indices[0] = tempStore;
			tempStore = tempVar;
		}
		else
		{
			int tempVar = indices[tempN + 1];
			indices[tempN + 1] = tempStore;
			tempStore = tempVar;
		}
		tempN = ((tempN + 1) == indices.size()) ? 0 : (tempN + 1);
		if (tempN == s)
		{
			flagS = 1;
		}
	}
	indices[n] = tempStore;
	if (flagS)
	{
		if ((s + 1) == indices.size())
		{
			nextS = 0;
		}
		else
		{
			nextS = s + 1;
		}
		flagS = 0;
	}
	// now we have to reverse the direction of rest of the vertexes
	// that start from a and end at b = if n==0 then indices.size() else n-1
	// first count the number of nodes we have to reverse
	int count = 1;
	int b = (n == 0) ? ((indices.size() - 1)) : (n - 1);
	int tempA = a;
	while (tempA != b)
	{
		count += 1;
		if ((tempA + 1) == indices.size())
		{
			tempA = 0;
		}
		else
		{
			tempA += 1;
		}
	}
	for (int ii = 0; ii < (count / 2); ii++)
	{
		int swap = indices[(ii + a) % (indices.size())];
		indices[(ii + a) % (indices.size())] = indices[((ii + a + ((count - 1) - (2 * ii))) % (indices.size()))];
		indices[((ii + a + ((count - 1) - (2 * ii))) % (indices.size()))] = swap;
	}
}

void Kopt(std::vector<std::pair<long double, long double>> &nodes, std::vector<int> &indices, std::vector<int> &bestIndices, long double &bestDis, std::vector<std::string> &configurations)
{
	std::vector<std::pair<int/*start vertex*/, int/*end vertex*/>> broken_edges;
	std::vector<std::pair<int, int>> joined_edges;
	std::string tempS = "";
	for (int x : indices)
	{
		tempS.append(std::to_string(x));
		tempS.append(" ");
	}
	configurations.push_back(tempS);

	// above we have got randomly generated indices

	std::vector<int> nToIgnore; // ignore all those n that lead to old configurations symmetry breaking
	

	long double maxDis = 0;
	int s = 0, e = 0, a = 0, nextS=0;
	//iterate over the current path and select the longest edge
	// start with the this edge and do kopt
	for (int x = 0; x < (indices.size() - 1); x++)
	{
		long double temp = calcDistance(nodes[indices[x]].first, nodes[indices[x]].second, nodes[indices[(x + 1)]].first, nodes[indices[(x + 1)]].second);
		if (maxDis < temp)
		{
			maxDis = temp;
			s = x;
			e = x + 1;
		}
	}
	int xx = indices.size() - 1;
	int x = 0;
	long double temp = calcDistance(nodes[indices[x]].first, nodes[indices[x]].second, nodes[indices[(xx)]].first, nodes[indices[(xx)]].second);
	if (maxDis < temp)
	{
		maxDis = temp;
		s = xx;
		e = x;
	}
	nextS = s; // if swaps swap s then check what is new s aka start

	//find adjacent nodes to e aka end
	if ((e + 1) == (indices.size()))
	{
		a = 0;
	}
	else
	{
		a = e + 1;
	}

	long double currentDistance = calcDistance(nodes[indices[s]].first, nodes[indices[s]].second, nodes[indices[(e)]].first, nodes[indices[(e)]].second);
	//find the indice whose distance is shortest from the e except s, a
	//------------------------------------------
	int n = searchNextN(currentDistance, indices, nodes, nToIgnore, s, e, a, broken_edges, joined_edges); // here I have to pass the joined_edges and broken_edges meaning don't join edges previously broken and don't break edges previosuly joined.
	//---------------------------------------------------------
	int noOfOpt = 0;
	while (n != -1 && noOfOpt < 10) // meaning there is actually a next n
	{
		std::vector<int> tempIndices = indices;
		generateConfiguration(tempIndices, e, n, s, a, nextS);
		joined_edges.push_back(std::make_pair(indices[e], indices[n]));
		int nMinus1 = (n == 0) ? (indices.size() - 1) : (n - 1);
		joined_edges.push_back(std::make_pair(indices[s], indices[nMinus1]));
		broken_edges.push_back(std::make_pair(indices[s], indices[e]));
		broken_edges.push_back(std::make_pair(indices[n], indices[nMinus1]));
		//check if this configuration is already present
		tempS = "";
		for (int x : tempIndices)
		{
			tempS.append(std::to_string(x));
			tempS.append(" ");
		}
		/*
		since string is present in circular format 
		i have to check by rotating the string each time and comparing
		*/
		if(configurationAlreadyPresent(configurations, tempS))
		{
			//this configuration is already present
			nToIgnore.push_back(n);
			currentDistance = calcDistance(nodes[indices[s]].first, nodes[indices[s]].second, nodes[indices[(e)]].first, nodes[indices[(e)]].second);
			n = searchNextN(currentDistance, indices, nodes, nToIgnore, s, e, a, broken_edges, joined_edges);
		}
		else
		{
			indices = tempIndices;
			configurations.push_back(tempS);
			//check if this is best configuration
			long double currDis = totalDistance(nodes, indices);
			if (currDis < bestDis)
			{
				bestDis = currDis;
				bestIndices = indices;
			}

			// now a is new e so
			e = a;
			if (nextS != s)
			{
				s = nextS;
			}
			//find adjacent nodes to e
			if ((e + 1) == (indices.size()))
			{
				a = 0;
			}
			else
			{
				a = e + 1;
			}

			int oldN = (n == (indices.size() - 1)) ? 0 : (n + 1);
			nToIgnore.clear();
			//nToIgnore.push_back(oldN);
			currentDistance = calcDistance(nodes[indices[s]].first, nodes[indices[s]].second, nodes[indices[(e)]].first, nodes[indices[(e)]].second);
			n = searchNextN(currentDistance, indices, nodes, nToIgnore, s, e, a, broken_edges, joined_edges);
		}
		noOfOpt++;
	}
	
}

void Kopt_SandBox(std::vector<std::pair<long double, long double>> &nodes, std::vector<int> &bestIndices, long double &bestDis)
{
	std::vector<std::pair<int/*start vertex*/, int/*end vertex*/>> broken_edges;
	std::vector<std::pair<int, int>> joined_edges;
	std::vector<int> indices = bestIndices;
	
	// above we have got randomly generated indices

	long double maxDis = 0;
	int s = 0, e = 0, a = 0, nextS = 0;
	//iterate over the current path and select the longest edge
	// start with the this edge and do kopt
	for (int x = 0; x < (indices.size() - 1); x++)
	{
		long double temp = calcDistance(nodes[indices[x]].first, nodes[indices[x]].second, nodes[indices[(x + 1)]].first, nodes[indices[(x + 1)]].second);
		if (maxDis < temp)
		{
			maxDis = temp;
			s = x;
			e = x + 1;
		}
	}
	int xx = indices.size() - 1;
	int x = 0;
	long double temp = calcDistance(nodes[indices[x]].first, nodes[indices[x]].second, nodes[indices[(xx)]].first, nodes[indices[(xx)]].second);
	if (maxDis < temp)
	{
		maxDis = temp;
		s = xx;
		e = x;
	}
	nextS = s; // if swaps swap s then check what is new s aka start

	//find adjacent nodes to e aka end
	if ((e + 1) == (indices.size()))
	{
		a = 0;
	}
	else
	{
		a = e + 1;
	}

	long double currentDistance = calcDistance(nodes[indices[s]].first, nodes[indices[s]].second, nodes[indices[(e)]].first, nodes[indices[(e)]].second);
	//find the indice whose distance is shortest from the e except s, a
	//------------------------------------------
	int n = searchNextN_SandBox(currentDistance, indices, nodes, s, e, a, broken_edges, joined_edges); // here I have to pass the joined_edges and broken_edges meaning don't join edges previously broken and don't break edges previosuly joined.
	//---------------------------------------------------------
	int noOfOpt = 0;
	while (n != -1 && noOfOpt < 10) // meaning there is actually a next n
	{
		std::vector<int> tempIndices = indices;
		generateConfiguration(tempIndices, e, n, s, a, nextS);
		joined_edges.push_back(std::make_pair(indices[e], indices[n]));
		joined_edges.push_back(std::make_pair(indices[s], indices[n - 1]));
		broken_edges.push_back(std::make_pair(indices[s], indices[e]));
		broken_edges.push_back(std::make_pair(indices[n], indices[n - 1]));
		//check if this configuration is already present
		{
			indices = tempIndices;
			//check if this is best configuration
			long double currDis = totalDistance(nodes, indices);
			if (currDis < bestDis)
			{
				bestDis = currDis;
				bestIndices = indices;
			}

			// now a is new e so
			e = a;
			if (nextS != s)
			{
				s = nextS;
			}
			//find adjacent nodes to e
			if ((e + 1) == (indices.size()))
			{
				a = 0;
			}
			else
			{
				a = e + 1;
			}

			int oldN = (n == (indices.size() - 1)) ? 0 : (n + 1);
			//nToIgnore.push_back(oldN);
			currentDistance = calcDistance(nodes[indices[s]].first, nodes[indices[s]].second, nodes[indices[(e)]].first, nodes[indices[(e)]].second);
			n = searchNextN_SandBox(currentDistance, indices, nodes, s, e, a, broken_edges, joined_edges);
		}
		noOfOpt++;
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

	std::vector<int> bestIndices = visited;
	long double bestDis = totalDistance(nodes, bestIndices);
	std::vector<std::string> configurations; //store the generated configurations we do not want to repeat
	std::vector<int> currBestIndices = bestIndices;
	long double currBestDis = totalDistance(nodes, currBestIndices);
	for (int i = 0; i < 100; i++) 
	{
		Kopt(nodes, visited, bestIndices, bestDis, configurations);
		if (bestDis < currBestDis) {
			visited = bestIndices;
			currBestIndices = bestIndices;
			currBestDis = bestDis;
		}
		else
		{
			break;
		}
	}
	
	bool noRandomness = false;
	for (int i = 0; i < 10; i++) 
	{
		visited = indices; // again set it to do randomness from some where
		for (int ii = 0; ii < 100; ii++) 
		{
			std::random_shuffle(visited.begin(), visited.end());
			std::string tempS = "";
			for (int x : visited)
			{
				tempS.append(std::to_string(x));
				tempS.append(" ");
			}
			if (configurationAlreadyPresent(configurations, tempS)) {
				noRandomness = true;
			}
			else
			{
				noRandomness = false;
				long double currDis = totalDistance(nodes, visited);
				if (currDis < bestDis)
				{
					bestDis = currDis;
					bestIndices = visited;
				}
				break;
			}
		}
		if (!noRandomness) 
		{
			currBestIndices = bestIndices;
			currBestDis = bestDis;
			for (int ii = 0; ii < 100; ii++) {
				Kopt(nodes, visited, bestIndices, bestDis, configurations);
				if (bestDis < currBestDis) {
					visited = bestIndices;
					currBestIndices = bestIndices;
					currBestDis = bestDis;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}

	}
	std::ostringstream dis;
	dis.precision(1);
	dis << std::fixed << bestDis;

	std::string outputStr = dis.str() + " " + std::to_string(0) + "\n";

	std::cout << dis.str() << " " << 0 << "\n";
	std::string str;
	for (int x = 0; x < bestIndices.size(); x++)
	{

		str.append(std::to_string(bestIndices[x]));
		str.append(" ");

	}
	std::cout << str.substr(0, str.size() - 1);
	outputStr.append(str.substr(0, str.size() - 1));
	std::ofstream out("output.txt");
	out << outputStr;
	out.close();
	return 0;
}

int TSP_justKopt(std::string fileName)
{
	//lets see how randomization performs
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

	//lets just take randomly generated indices
	//we try to move to local minima from this

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, (nodeCount - 1));
	int i = 0, xx = dist6(rng);
	std::vector<int> visited;
	std::random_shuffle(indices.begin(), indices.end());
	visited = indices;
	std::vector<int> bestIndices = visited;
	long double bestDis = totalDistance(nodes, bestIndices);
	Kopt_SandBox(nodes, bestIndices, bestDis);

}

int main(int argc, char **argv)
{
	try
	{
		std::string fileName;
		fileName = "tmp.data";
		TSP(fileName);
		//TSP_justKopt(fileName);

	}
	catch (...)
	{
		return (1);
	}
}
