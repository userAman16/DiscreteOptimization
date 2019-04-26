#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <queue>
#include <string>

/*
knapsackSolver 

*/
typedef struct Item
{
	float weight;
	int value;
	int Index;
} Item;

// Node structure to store information of decision 
// tree 
typedef struct Node
{
	// level  --> Level of node in decision tree (or index 
	//             in arr[] 
	// profit --> Profit of nodes on path from root to this 
	//            node (including this node) 
	// bound ---> Upper bound of maximum profit in subtree 
	//            of this node/ 
	int level, profit, bound;
	float weight;
	std::vector<int> items;
} Node;

bool cmp(Item a, Item b)
{
	double r1 = (double)a.value / a.weight;
	double r2 = (double)b.value / b.weight;
	return r1 > r2;
}

// Returns bound of profit in subtree rooted with u. 
// This function mainly uses Greedy solution to find 
// an upper bound on maximum profit. 
int bound(Node u, int n, int W, Item arr[])
{
	// if weight overcomes the knapsack capacity, return 
	// 0 as expected bound 
	if (u.weight >= W)
		return 0;

	// initialize bound on profit by current profit 
	int profit_bound = u.profit;

	// start including items from index 1 more to current 
	// item index 
	int j = u.level + 1;
	int totweight = u.weight;

	// checking index condition and knapsack capacity 
	// condition 
	while ((j < n) && (totweight + arr[j].weight <= W))
	{
		totweight += arr[j].weight;
		profit_bound += arr[j].value;
		j++;
	}

	// If k is not n, include last item partially for 
	// upper bound on profit 
	if (j < n)
		profit_bound += (W - totweight) * arr[j].value /
		arr[j].weight;

	return profit_bound;
}

// Returns maximum profit we can get with capacity W 
Node BranchAndBound(int W, Item arr[], int n)
{
	// sorting Item on basis of value per unit 
	// weight. 
	std::sort(arr, arr + n, cmp);

	// make a queue for traversing the node 
	std::queue<Node> Q;
	Node u, v, selectedNode;

	// dummy node at starting 
	u.level = -1;
	u.profit = u.weight = 0;
	Q.push(u);

	// One by one extract an item from decision tree 
	// compute profit of all children of extracted item 
	// and keep saving maxProfit 
	int maxProfit = 0;
	while (!Q.empty())
	{
		// Dequeue a node 
		u = Q.front();
		Q.pop();

		// If it is starting node, assign level 0 
		if (u.level == -1)
			v.level = 0;

		// If there is nothing on next level 
		if (u.level == n - 1)
			continue;

		// Else if not last node, then increment level, 
		// and compute profit of children nodes. 
		v.level = u.level + 1;

		// Taking current level's item add current 
		// level's weight and value to node u's 
		// weight and value 
		v.weight = u.weight + arr[v.level].weight;
		v.profit = u.profit + arr[v.level].value;
		v.items = u.items;
		if (v.weight <= W)
		{
			v.items.push_back(arr[v.level].Index);
		}
		// If cumulated weight is less than W and 
		// profit is greater than previous profit, 
		// update maxprofit 
		if (v.weight <= W && v.profit > maxProfit)
		{
			maxProfit = v.profit;
			selectedNode = v;
		}


		// Get the upper bound on profit to decide 
		// whether to add v to Q or not. 
		v.bound = bound(v, n, W, arr);

		// If bound value is greater than profit, 
		// then only push into queue for further 
		// consideration 
		if (v.bound > maxProfit)
			Q.push(v);

		// Do the same thing,  but Without taking 
		// the item in knapsack
		Node temp;
		temp.level = v.level;
		temp.weight = u.weight;
		temp.profit = u.profit;
		temp.items = u.items;
		temp.bound = bound(temp, n, W, arr);
		if (temp.bound > maxProfit)
			Q.push(temp);
	}

	return selectedNode;
}

int knapsackSolver(std::string fileName)
{
	if (fileName.empty())
		return 1;

	// read the lines out of the file
	std::ifstream infile;
	infile.open("tmp.data", std::ifstream::in);

	int numItems, capacity;
	infile >> numItems >> capacity;
	Item *arr = new Item[numItems];
	//int *items = new int[numItems];
	//int *weights = new int[numItems];
	int eachItem, eachCapacity = 0;
	int i = 0;
	while (infile >> eachItem >> eachCapacity)
	{
		Item tempItem = { eachCapacity, eachItem , i };
		arr[i] = (tempItem);
		i++;
	}


	Node result = BranchAndBound(capacity, arr, numItems);
	std::string outputStr = std::to_string(result.profit) + " " + std::to_string(0) + "\n";

	std::cout << result.profit << " " << 0 << "\n";
	std::string str;
	for (int x = 0; x < numItems; x++)
	{
		if (std::find(result.items.begin(), result.items.end(), x) != result.items.end())
		{
			str.append(std::to_string(1));
			str.append(" ");
		}
		else
		{
			str.append(std::to_string(0));
			str.append(" ");
		}
	}
	std::cout << str.substr(0, str.size() - 1);
	outputStr.append(str.substr(0, str.size() - 1));
	std::ofstream out("output.txt");
	out << outputStr;
	out.close();
	delete arr;
	return 0;
}

/*
knapsack solver
*/


int main(int argc, char **argv)
{
	try
	{
		std::string fileName;
		for (int ii = 1; ii < argc; ii++)
		{
			if (0 == strcmp(argv[ii], "-knapsack")) {
				fileName = argv[ii + 1];
				knapsackSolver(fileName);
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