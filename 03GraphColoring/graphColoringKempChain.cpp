#include <iostream>
#include <vector>
#include <fstream>
#include <list> 
#include <string>
#include <algorithm>

typedef struct Node
{
	int color;
	int value;
	int degree;
} Node;

class Graph
{
	int V;    // No. of vertices 
	    // A dynamic array of adjacency lists 
public:
	std::list<int> *adj;
	// Constructor and destructor 
	Graph(int V) { this->V = V; adj = new std::list<int>[V]; }
	~Graph() { delete[] adj; }

	// function to add an edge to graph 
	void addEdge(int v, int w);
};

void Graph::addEdge(int v, int w)
{
	adj[v].push_back(w);
	adj[w].push_back(v);  // Note: the graph is undirected 
}

bool cmp(Node a, Node b)
{
	return((a.degree) > (b.degree));
}

void getNeighboringC(int x, std::vector<int> & neighboringC, std::vector<Node> &nodes, Graph &graph)
{
	std::list<int> neighbors = graph.adj[x];
	std::list<int>::iterator it;
	for (it = neighbors.begin(); it != neighbors.end(); it++)
	{
		if ((nodes[(*it)].color) != -1)
		{
			neighboringC.push_back(nodes[(*it)].color);
		}
	}
}

int colorGraph(std::vector<Node> &nodes, Graph &graph, std::vector<int> indices)
{
	/*
	global first color gColor = 0
	store all Nodes in descending order of degree in array A
	where Nodes is an struct of value and color
	for all nodes in A
		if this is first Node apply color gColor
		else
		applyColor(node, nodes, graph)
		{
			search all adjacent color to this node and store in array adjColor[]
			now search nearest color to this node which is not in adjColor[]
			if none apply gColor+1 else apply that color
		}
	*/
	int gColor = 0, first = 1;
	
	std::vector<int> colors;
	for (int i : indices)
	{
		if (first)
		{
			nodes[i].color = gColor;
			colors.push_back(gColor);
			gColor += 1;
			first = 0;
		}
		else
		{
			//store the color of its neighbors in neighboringC
			//then store all those colors not in
			//neighboringC(if none then take the last color increment it and apply)
			//in vector availableColors then do search which color in availableColors is nearest and apply
			std::vector<int> neighboringC;
			getNeighboringC(nodes[i].value, neighboringC, nodes, graph);
			std::vector<int> availableC;
			for (int ii : colors)
			{
				if (std::find(neighboringC.begin(), neighboringC.end(), ii) != neighboringC.end())
				{
					continue;	
				}
				else
				{
					availableC.push_back(ii);
				}
			}
			if (availableC.size() == 0)
			{
				nodes[i].color = gColor;
				colors.push_back(gColor);
				gColor += 1;
			}
			else if (availableC.size() == 1)
			{
				nodes[i].color = availableC[0];
			}
			else
			{
				//nearestC(availableC) // which color in availableColors is nearest and apply
				//dummy functionality 
				nodes[i].color = availableC[0];
				//dummy functionality 
			}
			
		}
	}
	return gColor;

}

int graphColoring(std::string fileName)
{
	if (fileName.empty())
		return 1;

	// read the lines out of the file
	std::ifstream infile;
	infile.open("tmp.data", std::ifstream::in);

	int nodeCount, edgeCount;
	infile >> nodeCount >> edgeCount;
	std::vector<Node> nodes;
	
	for (int i = 0; i < nodeCount; i++)
	{
		Node node;
		node.value = i;
		node.color = -1;
		node.degree = 0;
		nodes.push_back(node);
	}

	Graph graph(nodeCount);
	//int *items = new int[numItems];
	//int *weights = new int[numItems];
	int node1, node2 = 0;
	while (infile >> node1 >> node2)
	{
		graph.addEdge(node1, node2);
		nodes[node1].degree += 1;
		nodes[node2].degree += 1;
	}

	std::vector<Node> tempNodes = nodes;
	std::sort(tempNodes.begin(), tempNodes.end(), cmp);
	std::vector<int> indices;
	for (int i = 0; i < tempNodes.size(); i++)
	{
		indices.push_back(tempNodes[i].value);
	}
	tempNodes = nodes;
	std::vector<Node> bestNodes;
	int result = colorGraph(tempNodes, graph, indices);
	/*group nodes by color 2d vector of color and nodes in them*/
	/*sort each vector of nodes by their degree*/
	int iterations = 0;
	if (nodeCount > 501)
	{
		iterations = 100;
	}
	else
	{
		iterations = 5000;
	}
	for (int x = 0; x < iterations; x++)
	{
		
		std::vector<std::vector<Node>> colorChain;
		for (int y = 0; y < result; y++) //iterate for each color c
		{
			
			std::vector<Node> sameColorNodes;
			for (int xy = 0; xy < tempNodes.size(); xy++) // iterate for each node
			{
				

				if (tempNodes[xy].color == y) // if this nodes color is c
				{
					sameColorNodes.push_back(tempNodes[xy]); // set of nodes that have color c
				}
			}
			colorChain.push_back(sameColorNodes); // 2d array of color and corresponding nodes
		}
		std::random_shuffle(colorChain.begin(), colorChain.end());
		std::vector<int> tempIndices;
		//create indices
		for (int y = 0; y < colorChain.size(); y++)
		{
			std::vector<Node> sameColorNodes = colorChain[y];
			std::sort(sameColorNodes.begin(), sameColorNodes.end(), cmp);
			for (int i = 0; i < sameColorNodes.size(); i++)
			{
				tempIndices.push_back(sameColorNodes[i].value);
			}
		}

		tempNodes = nodes;
		result = colorGraph(tempNodes, graph, tempIndices);
		bestNodes = tempNodes;
	}

	std::string outputStr = std::to_string(result) + " " + std::to_string(0) + "\n";

	std::cout << result << " " << 0 << "\n";
	std::string str;
	for (int x = 0; x < bestNodes.size(); x++)
	{

			str.append(std::to_string(bestNodes[x].color));
			str.append(" ");

	}
	std::cout << str.substr(0, str.size() - 1);
	outputStr.append(str.substr(0, str.size() - 1));
	std::ofstream out("output.txt");
	out << outputStr;
	out.close();
	return 0;
}


int main(int argc, char **argv)
{
	try
	{
		std::string fileName;
		for (int ii = 1; ii < argc; ii++)
		{
			if (0 == strcmp(argv[ii], "-gcoloring")) {
				fileName = argv[ii + 1];
				graphColoring(fileName);
			}
			else
			{
				std::cout << "wrong command\n" << argv[ii];
			}
		}
	}
	catch (...)
	{
		return (1);
	}
}
