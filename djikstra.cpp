//Created by Brandon Barnes

//Requires use of at least c++11

//displayMatrix design modified from: https://www.tutorialspoint.com/cplusplus-program-to-implement-adjacency-matrix
//Graph sizing based on design from: https://stackoverflow.com/questions/12375591/vector-of-vectors-to-create-matrix

//Implementation does not adjust graph to have no single edge loops or to take min of parallel edges

#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>

using namespace std;

class Node;

class Edge{
	int rowPosition;
	int columnPosition;
	int weight;
	
	public:
	Edge(){
		
	};
	Edge(int r, int c, int w){
		this->rowPosition = r;
		this->columnPosition = c;
		this->weight = w;
	}
	int getRow(){
		return this->rowPosition;
	}
	int getColumn(){
		return this->columnPosition;
	}
	int getWeight(){
		return this->weight;
	}
};

class Node{
	int value;
	vector<Edge> edges;
	int distance;
	int traversed;
	Node* parent;
	
	public:
	Node(){
		this->value = 0;
		this->distance = INT_MAX;
	}
	
	Node(int startingValue){
		this->value = startingValue;
		this->distance = INT_MAX;
	}
	
	void addEdge(int rowPosition, int columnPosition, int weight){
		edges.push_back(Edge(rowPosition, columnPosition, weight));
	}
	vector<Edge> getEdges(){
		return this->edges;
	}
	int getValue(){
		return this->value;
	}
	Edge getEdge(int position){
		return edges[position];
	}
	void setTraversed(int toSet){
		this->traversed = toSet;
	}
	void setDistance(int toSet){
		this->distance = toSet;
	}
	int getTraversed(){
		return this->traversed;
	}
	int getDistance(){
		return this->distance;
	}
	void setParent(Node* newParent){
		this->parent=newParent;
	}
	Node* getParent(){
		return this->parent;
	}
};

void displayMatrix(vector<vector <Node> > adjMatrix){
	for(int i = 0; i<adjMatrix.size(); i++){
		for(int j = 0; j<adjMatrix[i].size(); j++){
			cout << adjMatrix[i][j].getValue() << ',' << adjMatrix[i][j].getDistance() << ' ';
		}
		cout << endl;
	}
}

vector<vector <Node> > genBasicGraph(int rowSize, int columnSize){
	
	Node* temp = new Node(1);
	vector<vector <Node> > graph(rowSize, vector<Node>(columnSize));
	
	
	graph[0][0] = *temp;
	
	for(int i = 1; i<rowSize && i<columnSize; i++){
		
		temp = new Node(i+1);
		graph[i][i] = *temp;
		temp->addEdge(i-1,i-1,i);
		graph[i-1][i-1].addEdge(i,i,i);
	}
	
	
	return graph;
	
}

vector<vector <Node> > initializeGraph(vector<vector <Node> > graph){
	for(int i = 0; i<graph.size();i++){
		for(int j = 0; j<graph[i].size(); j++){
			graph[i][j].setDistance(INT_MAX); //Currently unreached/unreachable
			graph[i][j].setParent(NULL);
			graph[i][j].setTraversed(0);
		}
	}
	return graph;
}


vector<vector <Node> > djikstra(vector<vector <Node> > &graph, int row, int column){
	auto comparator =  [](Node* a, Node* b){return a->getDistance()>b->getDistance();};
	vector<Edge> edges;
	Node* workingNode;
	int tempRow;
	int tempColumn;
	int distanceHolder;
	
	
	graph = initializeGraph(graph);
	
	priority_queue<Node*, vector<Node*>, decltype(comparator)> pQueue(comparator);
	
	
	graph[row][column].setDistance(0);
	graph[row][column].setTraversed(1);
	pQueue.push(&(graph[row][column]));
	
	
	while(!(pQueue.empty())){
		
		workingNode = pQueue.top();
		
		workingNode->setTraversed(2);
		
		
		edges = workingNode->getEdges();
		
		Edge temp;
		
		//iterate through all edges adding nodes if untraversed
			//add via stored row/column locations
			//base distance of nodes off of current node distance+weight
			//Update any node encountered as necessary
				//Update by saving top in a temp location popping it off and adding again
					//should be able to use working node for this
		
		for(int i = 0; i<edges.size(); i++){
			
			temp = edges[i];
			tempRow = temp.getRow();
			tempColumn = temp.getColumn();
			distanceHolder = workingNode->getDistance();
			
			if(graph[tempRow][tempColumn].getTraversed()==0){
				graph[tempRow][tempColumn].setTraversed(1);
				graph[tempRow][tempColumn].setDistance(distanceHolder+temp.getWeight());
				graph[tempRow][tempColumn].setParent(workingNode);
				pQueue.push(&(graph[tempRow][tempColumn]));
			}
			else if(graph[tempRow][tempColumn].getTraversed()==1){
				if(graph[tempRow][tempColumn].getDistance()>(distanceHolder+temp.getWeight())){
					graph[tempRow][tempColumn].setDistance(distanceHolder+temp.getWeight());
					graph[tempRow][tempColumn].setParent(workingNode);
				}
			}
			else{
				if(graph[tempRow][tempColumn].getDistance()>(distanceHolder+temp.getWeight())){
					graph[tempRow][tempColumn].setDistance(distanceHolder+temp.getWeight());
					graph[tempRow][tempColumn].setParent(workingNode);
				}
			}
		}
		
		pQueue.pop(); 
		
	}
	
	return graph;
}


int main(){
	int rowSize = 10;
	int columnSize = 10;
	Node* temp;
	
	
	vector<vector<Node> > graph = genBasicGraph(rowSize, columnSize);
	
	
	
	temp = new Node(1000);
	graph[0][9] = *temp;
	graph[0][9].addEdge(1,1,12);
	graph[1][1].addEdge(0,9,12);
	
	temp = new Node(-2000);
	graph[9][0] = *temp;
	graph[9][0].addEdge(4,4,15);
	graph[4][4].addEdge(9,0,15);
	graph[9][0].addEdge(9,9,15);
	graph[9][9].addEdge(9,0,15);
	
	
	
	
	graph = djikstra(graph, 0, 0);
	displayMatrix(graph);
	
	cout << "exits" << endl;
}