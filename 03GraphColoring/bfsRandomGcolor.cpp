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
	   
	    
public:
	int V; // No. of vertices 
	std::list<int> *adj; // A dynamic array of adjacency lists 
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

int nearestC(int s, std::vector<int> &availableC, std::vector<Node> &nodes, Graph &graph)
{
	//visit all nodes in bfs from this node pass all available colors also 
	//and return which ever first color found which is in available colors
	
	// Mark all the vertices as not visited 
	bool *visited = new bool[graph.V];
	for (int i = 0; i < graph.V; i++)
	{
		visited[i] = false;
	}
	// Create a queue for BFS 
	std::list<int> queue;
	// Mark the current node as visited and enqueue it 
	visited[s] = true;
	queue.push_back(s);
	// 'i' will be used to get all adjacent 
	// vertices of a vertex 
	std::list<int>::iterator i;
	int first = 1;
	while (!queue.empty())
	{
		// Dequeue a vertex from queue and check it's color if not -1 and is present in availableC
		if (first)
		{
			s = queue.front();
			queue.pop_front();
			first = 0;
			//do nothing
		}
		else
		{
			s = queue.front();
			queue.pop_front();
			//check it's color if not -1 and is present in availableC
			if (((nodes[s].color) != -1) && (std::find(availableC.begin(), availableC.end(), (nodes[s].color)) != availableC.end()))
			{
				return (nodes[s].color);
			}
		}
		

		// Get all adjacent vertices of the dequeued 
		// vertex s. If a adjacent has not been visited,  
		// then mark it visited and enqueue it 
		for (i = graph.adj[s].begin(); i != graph.adj[s].end(); ++i)
		{
			if (!visited[*i])
			{
				visited[*i] = true;
				queue.push_back(*i);
			}
		}
	}
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

int colorGraph(std::vector<Node> &nodes, Graph &graph, std::vector<int> &indices)
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
				nodes[i].color = nearestC(nodes[i].value ,availableC, nodes, graph);
				//dummy functionality 
				//nodes[i].color = availableC[0];
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

	std::vector<std::vector<int>> indiceGroup;
	int j = 0, i= 0;
	while (j < tempNodes.size())
	{
		if (tempNodes[i].degree == tempNodes[j].degree )
		{
			j++;
		}
		else
		{
			std::vector<int> indice;
			for (int x = i; x < j; x++)
			{
				indice.push_back(tempNodes[x].value);
			}
			indiceGroup.push_back(indice);
			i = j;
		}
		if (j == tempNodes.size())
		{
			std::vector<int> indice;
			for (int x = i; x < j; x++)
			{
				indice.push_back(tempNodes[x].value);
			}
			indiceGroup.push_back(indice);
			i = j;
		}
	}

	for (int i = 0; i < tempNodes.size(); i++)
	{
		indices.push_back(tempNodes[i].value);
	}
	int globalResult = colorGraph(nodes, graph, indices);
	std::vector<Node> globalNodes = nodes;
	for (int i = 0; i < 50; i++)
	{
		std::vector<int> shuffledIndice;
		for (int x = 0; x < indiceGroup.size(); x++)
		{
			std::random_shuffle(indiceGroup[x].begin(), indiceGroup[x].end());
		}
		for (int x = 0; x < indiceGroup.size(); x++)
		{
			for (int xx = 0; xx < indiceGroup[x].size(); xx++)
			{
				shuffledIndice.push_back(indiceGroup[x][xx]);
			}
		}
		int localResult = colorGraph(nodes, graph, shuffledIndice);
		if (localResult < globalResult)
		{
			globalNodes = nodes;
			globalResult = localResult;
		}

	}
	std::string outputStr = std::to_string(globalResult) + " " + std::to_string(0) + "\n";

	std::cout << globalResult << " " << 0 << "\n";
	std::string str;
	for (int x = 0; x < globalNodes.size(); x++)
	{

			str.append(std::to_string(globalNodes[x].color));
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