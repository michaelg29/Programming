#include <iostream>
#include <limits.h>
#include <vector>
#include <map>

#define V 7

template<typename T>
bool contains(std::vector<T> v, T x) {
	return std::find(v.begin(), v.end(), x) != v.end();
}

typedef std::vector<std::pair<int, double>> MinPQ;

int findMinOnPQ(MinPQ minPQ) {
	double min = minPQ.at(0).second;
	int idx = minPQ.at(0).first;
	
	for (int i = 1, size = minPQ.size(); i < size; i++) {
		if (minPQ.at(i).second < min) {
			min = minPQ.at(i).second;
			idx = minPQ.at(i).first;
		}
	}

	return idx;
}

int inMinPQ(MinPQ minPQ, int element) {
	for (int i = 0, size = minPQ.size(); i < size; i++) {
		if (minPQ.at(i).first == element) {
			return i;
		}
	}

	return -1;
}

double calculateDistance(double weight[V], int prevNode[V], int element) {
	if (prevNode[element] == element) {
		return 0;
	}

	return weight[element] + calculateDistance(weight, prevNode, prevNode[element]);
}

double** dijkstra(char nodes[V], double graph[V][V], int src) {
	double weight[V] = { 0 };
	int prevNode[V] = { 0 };

	for (int i = 0; i < V; i++) {
		weight[i] = INT_MAX;
	}

	// distance from src to src is 0
	weight[src] = 0;
	prevNode[src] = src;

	// visited log
	std::vector<int> visited;

	// minimum distance priority queue
	MinPQ minPQ;

	minPQ.push_back(std::pair<int, double>(src, 0));

	// find shortest path for all vertices
	while (minPQ.size() != 0) {
		// get smallst val
		int smallest = findMinOnPQ(minPQ);

		// add to visited list and erase from queue
		visited.push_back(smallest);
		minPQ.erase(minPQ.begin() + inMinPQ(minPQ, smallest));

		// iterate over neighbours
		for (int i = 0; i < V; i++) {
			auto loc = inMinPQ(minPQ, i);

			if (graph[smallest][i] == 0) {
				// doesn't exist
				continue;
			}
			else if (contains<int>(visited, i)) {
				// already visited
				continue;
			}
			else if (loc != -1) {
				// in queue
				double currentDistance = calculateDistance(weight, prevNode, i);

				double newDistance = calculateDistance(weight, prevNode, smallest) + graph[smallest][i];

				if (newDistance < currentDistance) {
					// set new distance
					minPQ.erase(minPQ.begin() + loc);
					minPQ.push_back(std::pair<int, double>(i, newDistance));
					weight[i] = graph[smallest][i];
					prevNode[i] = smallest;
				}
			}
			else {
				// add to queue
				weight[i] = graph[smallest][i];
				prevNode[i] = smallest;

				minPQ.push_back(std::pair<int, double>(i, calculateDistance(weight, prevNode, i)));
			}
		}
	}

	// generate shortestPathsTree
	double** shortestPathsTree = 0;
	shortestPathsTree = new double*[V];

	for (int r = 0; r < V; r++) {
		shortestPathsTree[r] = new double[V];
		for (int c = 0; c < V; c++) {
			shortestPathsTree[r][c] = 0;
		}
	}

	// input final values into return array
	for (int i = 0; i < V; i++) {
		shortestPathsTree[prevNode[i]][i] = weight[i];
	}

	return shortestPathsTree;
}

void printArray(double graph[V][V]) {
	for (int r = 0; r < V; r++) {
		for (int c = 0; c < V; c++) {
			std::cout << graph[r][c] << ", ";
		}
		std::cout << std::endl;
	}
}

void printArray(double** graph) {
	for (int r = 0; r < V; r++) {
		for (int c = 0; c < V; c++) {
			std::cout << graph[r][c] << ", ";
		}
		std::cout << std::endl;
	}
}

double calculateDistance(double** shortestPathsTree, int src, int goal) {
	if (src == goal) {
		return 0;
	}

	// find row with goal as a neighbour
	for (int r = 0; r < V; r++) {
		if (shortestPathsTree[r][goal] != 0) {
			return shortestPathsTree[r][goal] + calculateDistance(shortestPathsTree, src, r);
		}
	}
}

int main() {
	std::cout << "Hello, Dijkstra!" << std::endl;

	char nodes[V] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G' };

	double graph[V][V] = {
		//   A, B, C, D, E, F, G
			{0, 5, 3, 0, 0, 0, 0},
			{0, 0, 2, 0, 3, 0, 1},
			{0, 0, 0, 7, 7, 0, 0},
			{2, 0, 0, 0, 0, 6, 0},
			{0, 0, 0, 2, 0, 1, 0},
			{0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 1, 0, 0}
	};

	printArray(graph);
	std::cout << std::endl << std::endl;

	double **shortestPathsTree = dijkstra(nodes, graph, 0);

	printArray(shortestPathsTree);

	int start = 0;
	int goal = 3;

	std::cout << std::endl << nodes[start] << "->" << nodes[goal] << ": " << calculateDistance(shortestPathsTree, start, goal) << std::endl;

	return 0;
}