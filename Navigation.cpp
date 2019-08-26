#include "Navigation.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "Node.h"
#include <list>
#include "Edge.h"
#include <vector>
#include "ACW_Wrapper.h"
#include "NodeQueue.h"


using namespace std;

// Converts latitude/longitude into eastings/northings
extern void LLtoUTM(const double Lat, const double Long, double &UTMNorthing, double &UTMEasting);

vector<Node*> nodes; //from my understanding its better than a list because it can be indexed. Its basicly a dynamic array.
vector<Edge*> edges;

Navigation::Navigation() : _outFile("Output.txt")
{
}

Navigation::~Navigation()
{
}

double Navigation::CalculateDistance(const float &pX1, const float &pY1, const float &pX2, const float &pY2) {

	float a = pX1 - pX2;
	float b = pY1 - pY2;
	return sqrt(a * a + b * b);
}

Node* Navigation::findNodeByRefference(int& refference) 
{
	int nodesSize = nodes.size();
	for (int i = 0; i < nodesSize; i++) {
		if (nodes[i]->getReference() == refference) {
			return nodes[i];
		}
	}
}

void Navigation::pushNextNodeToStartOfQ(list<Node*>& nodeQueue)
{
	int temp = 9999999999;
	int outputNodeIndex = 0;
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->getDistanceFromStart() < temp) {
			outputNodeIndex = i;
			temp = nodes[i]->getDistanceFromStart();
		}
	}

	if (nodes[0] != nodes[outputNodeIndex]) {
		Node* tempNode = nodes[0];
		nodes[0] = nodes[outputNodeIndex];
		nodes[outputNodeIndex] = nodes[0];
	}

}

Node * Navigation::findNextNodeToVisit()
{
	int temp = 9999999999;
	Node* outputNode = 0;
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->getDistanceFromStart() < temp && nodes[i]->isVisited() == false) {
			outputNode = nodes[i];
			temp = nodes[i]->getDistanceFromStart();
		}
	}

	return outputNode;
}


bool Navigation::ProcessCommand(const string& commandString)
{
	istringstream inString(commandString);
	string command;
	inString >> command;
	

	ofstream file;

	if (command == "MaxDist") {

		try {
			float max = 0;
			float temp;

			Node* a = nodes[1];
			Node* b = nodes[0];
			
			int lastIndex = nodes.size() - 1;
			Node* tempNode;
			
			while (lastIndex > 0) {


				for (int i = 0; i <= lastIndex; i++) {

					temp = CalculateDistance(nodes[0]->getX(), nodes[0]->getY(), nodes[i]->getX(), nodes[i]->getY());

					if (temp > max) {
						max = temp;
						a = nodes[0];
						b = nodes[i];
					}
				}

				tempNode = nodes[0];
				nodes[0] = nodes[lastIndex];
				nodes[lastIndex] = tempNode;
				--lastIndex;

			}
			
			file.open("Output.txt", ios::app);
			file << "MaxDist" << endl;
			file << a -> getName() << "," << b -> getName() << "," << max << endl << endl;
			file.close();

			return true;

		}
		catch(exception e){
			return false;
		}

	}

	else if (command == "MaxLink") {

		try {
			float max = 0;
			float temp;
			Edge* e = edges[0];
			int startNodeRef;
			int endNodeRef;

			int edgesSize = edges.size() - 1;

			for (int i = 0; i < edgesSize; i++) {

				//find distance between them.
				temp = edges[i]->getWeight();

				if (temp > max) {
					max = temp;
					e = edges[i];
				}
			}

			file.open("Output.txt", ios::app);
			file << "MaxLink" << endl;
			file << e->getStartNode()->getReference() << "," << e->getEndNode()->getReference() << "," << max << endl << endl;

			return true;
		}
		catch (exception e)
		{
			return false;
		}
		
	}

	else if (command == "FindDist") {

		try {
			string reference1;
			string reference2;
			inString >> reference1;
			inString >> reference2;
			int reference1No = std::stoi(reference1);
			int reference2No = std::stoi(reference2);
			Node* node1 = findNodeByRefference(reference1No);
			Node* node2 = findNodeByRefference(reference2No);

			int distance = CalculateDistance(node1->getX(), node1->getY(), node2->getX(), node2->getY());

			file.open("Output.txt", ios::app);
			file << "FindDist " << reference1 << " " << reference2 << endl;
			file << node1->getName() << "," << node2->getName() << "," << distance << endl << endl << endl;
			file.close();

			cout << "distance between the nodes is: " << distance << endl;
			cout << "Between node: " + node1->getName() + " and node: " + node2->getName() << endl;
			return true;
		}
		catch (exception e)
		{
			return false;
		}
		

	}

	else if (command == "FindNeighbour") {

		try {
			string nodeReference;
			inString >> nodeReference;
			int nodeReferenceNo = stoi(nodeReference);

			Node* node1 = findNodeByRefference(nodeReferenceNo);
			file.open("output.txt", ios::app);
			file << "Find Neighbour " << nodeReference << endl;


			vector<Edge*> LocalEdges = node1->getEdges();
			for (int i = 0; i < LocalEdges.size(); i++) {
				file << LocalEdges[i]->getEndNode()->getReference() << endl;
			}


			file << endl;
			file.close();
			return true;

		}
		catch (exception e) {
			return false;
		}

	}

	else if (command == "Check") {
		inString >> command;
		TransMode transmode;
		if (command == "Rail") {
			transmode = Rail;
		}
		else if (command == "Ship") {
			transmode = Ship;
		}
		else {
			return false;
		}

		file.open("output.txt", ios::app);
		file << "Check " + command;
		vector<string> nodeReferences;
		while (inString) {
			inString >> command;
			file << command + " ";
			nodeReferences.push_back(command);
		}

		file << endl;

		bool valid = true;


		int k = 0;

		int reference = stoi(nodeReferences[k]);
		Node* n = findNodeByRefference(reference);
		vector<Edge*> edges1 = n->getEdges();

		while (valid) {
			valid = false;

			reference = stoi(nodeReferences[k + 1]);

			for (int i = 0; i < edges.size(); i++) {
				if (reference == edges[i]->getEndNode()->getReference() && edges[i]->canBeTraversed(transmode)) {
					valid = true;
					file << nodeReferences[k] + "," + nodeReferences[k + 1] + ",PASS" << endl;
				}
			}

			if (!valid) {
				file << nodeReferences[k] + "," + nodeReferences[k + 1] + ",FAIL" << endl;
			}
			else {
				n = findNodeByRefference(reference);
				edges = n->getEdges();
			}

			k++;
		}
		file << endl;


		return true;
	}

	else if (command == "FindRoute") {

		inString >> command;
		TransMode transmode;

		if (command == "Foot") {
			transmode = Foot;
		}
		else if (command == "Bike") {
			transmode = Bike;
		}
		else if (command == "Car") {
			transmode = Car;
		}
		else if (command == "Bus") {
			transmode = Bus;
		}
		else if (command == "Rail") {
			transmode = Rail;
		}
		else if (command == "Ship") {
			transmode = Ship;
		}
		else {
			return false;
		}
		
		ofstream file;
		file.open("Output.txt", ios::app);
		file << "FindRoute " << command << " ";

		inString >> command; //gets the starting node reference in string form.
		file << command + " ";
		int reference = stoi(command);
		Node* n1 = findNodeByRefference(reference);
		inString >> command; //gets the destination node reference in string form.
		file << command << endl;
		reference = stoi(command);
		Node* n2 = findNodeByRefference(reference);

		bool found = false;
		vector<int> nodeReferences;
		findRoute(n1, n2, found, transmode, nodeReferences);
		if (nodeReferences.size() == 0) {
			//route not found
			file << "FAIL" << endl;
		}
		else {// route is found

			for (int i = nodeReferences.size() - 1; i > 0; i--) {
				file << nodeReferences[i] << endl;
			}
			file << n2->getReference() << endl;
		}

		file << endl;

		//reset the node visited status. Little inefficient because some of these nodes dont require for the visited to be reset.
		for (int i = 0; i < nodes.size(); i++) {
			nodes[i]->resetVisited();
		}


		return true;

	}
	
	else if (command == "FindShortestRoute") {

		inString >> command;
		TransMode transmode;

		if (command == "Foot") {
			transmode = Foot;
		}
		else if (command == "Bike") {
			transmode = Bike;
		}
		else if (command == "Car") {
			transmode = Car;
		}
		else if (command == "Bus") {
			transmode = Bus;
		}
		else if (command == "Rail") {
			transmode = Rail;
		}
		else if (command == "Ship") {
			transmode = Ship;
		}
		else {
			return false;
		}

		ofstream file;
		file.open("Output.txt", ios::app);
		file << "FindShortestRoute " << command << " ";

		inString >> command; //gets the starting node reference in string form.
		file << command + " ";
		int reference = stoi(command);
		Node* n1 = findNodeByRefference(reference);
		inString >> command; //gets the destination node reference in string form.
		file << command << endl;
		reference = stoi(command);
		Node* n2 = findNodeByRefference(reference);

		//Initialize dijkstras components
		n1->distanceFromStartZero(); //starting node is always zero.
		vector<Edge*> edges;
		double temp;
		Node* tempNode;
		Node* currentNode = n1;
		bool valid = false;
		NodeQueue nodeQueue;
		nodeQueue.AddNode(currentNode);

		while (!nodeQueue.CheckEmpty()) {

			currentNode = nodeQueue.ReturnAndPopFront();

			if (currentNode == n2) {
				valid = true;
				break;
			}

			edges = currentNode->getEdges();

			for (int i = 0; i < edges.size(); i++) {

				tempNode = edges[i]->getEndNode();

				if (edges[i]->canBeTraversed(transmode)) {
					temp = currentNode->getDistanceFromStart() + edges[i]->getWeight();
					if (temp < tempNode->getDistanceFromStart()) {
						tempNode->setDistanceFromStart(temp);
						tempNode->setPreviousNodeRef(currentNode->getReference());
						
						//check if temp is in the queue, if not add it
						if (tempNode->inQueue == false) {
							nodeQueue.AddNode(tempNode);
							nodeQueue.Sort();
							tempNode->inQueue = true;
						}


					}
				}

			}

		}

		if (valid) {
			//adds all the backwards path to the destination on to the list.
			list<int> nodeRefs;
			nodeRefs.push_front(n2->getReference());
			while (currentNode != n1) {
				int nodeRef = currentNode->getPreviousNodeRef();
				nodeRefs.push_front(nodeRef);
				currentNode = findNodeByRefference(nodeRef);
			}

			//outputs the path to a file
			for (auto nodeRef : nodeRefs) {
				file << nodeRef << endl;
			}

			file << endl;
		}
		else {
			file << "FAIL" << endl << endl;
		}


		//reset all the nodes.
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->setDistanceFromStart(9999999999);
			nodes[i]->setPreviousNodeRef(0);
			nodes[i]->resetVisited();
		}

		return true;
	}

	return false;
}


//recursive depth first search for the destination node
void Navigation::findRoute(Node* pTraversalNode, Node* pDestinationNode, bool& pFound, const TransMode& pTransMode, vector<int>& pReferences) 
{
	pTraversalNode->visitNode();
	vector<Edge*> edges = pTraversalNode->getEdges();

	for (int i = 0; i < edges.size(); i++) {
		if (edges[i]->canBeTraversed(pTransMode)) {
			Node* n1 = edges[i]->getEndNode();
			if (n1 == pDestinationNode) {
				pFound = true;
				pReferences.push_back(n1->getReference());
			}
			else if (n1->isVisited() == false && pFound == false) {
				findRoute(n1, pDestinationNode, pFound, pTransMode, pReferences);
			}

		}

	}
	if (pFound == true) {
		pReferences.push_back(pTraversalNode->getReference());
	}

}

bool Navigation::BuildNetwork(const string &fileNamePlaces, const string &fileNameLinks)
{


	fstream finPlaces(fileNamePlaces);
	fstream finLinks(fileNameLinks);

	std::string name;
	std::string reference;
	std::string longitude;
	std::string latitude;
	int referenceNo;
	float longitudeNo;
	float latitudeNo;
	double Northing;
	double Easting;

	while (!finPlaces.eof()) {
		getline(finPlaces, name, ',');
		getline(finPlaces, reference, ',');
		getline(finPlaces, latitude, ',');
		getline(finPlaces, longitude, '\n');

		referenceNo = std::stoi(reference);
		latitudeNo = std::stof(latitude);
		longitudeNo = std::stof(longitude);
		
		LLtoUTM(latitudeNo, longitudeNo, Northing, Easting);

		nodes.push_back(new Node(name, referenceNo, Northing, Easting));
	}

	string startNodeRef;
	string endNodeRef;
	string vehicleType;
	int sNodeRefNo;
	int eNodeRefNo;

	while (!finLinks.eof()) {

		getline(finLinks, startNodeRef, ',');
		getline(finLinks, endNodeRef, ',');
		getline(finLinks, vehicleType, '\n');

		sNodeRefNo = std::stoi(startNodeRef);
		eNodeRefNo = std::stoi(endNodeRef);

		// calculate edge weight
		Node* node1 = findNodeByRefference(sNodeRefNo);
		Node* node2 = findNodeByRefference(eNodeRefNo);
		double edgeWeight = CalculateDistance(node1->getX(), node1->getY(), node2->getX(), node2->getY());

		

		TransMode transmode;

		if (vehicleType == "Foot") {
			transmode = Foot;
		}
		else if (vehicleType == "Bike") {
			transmode = Bike;
		}
		else if (vehicleType == "Car") {
			transmode = Car;
		}
		else if (vehicleType == "Bus") {
			transmode = Bus;
		}
		else if (vehicleType == "Rail") {
			transmode = Rail;
		}
		else if (vehicleType == "Ship") {
			transmode = Ship;
		}
        

		//create first version of the edge.
		Edge* e;
		e = new Edge(node1, node2, edgeWeight, transmode);
		edges.push_back(e);
		node1 -> addEdge(e);
		//create the second version of the edge.
		e = new Edge(node2, node1, edgeWeight, transmode);
		edges.push_back(e);
		node2->addEdge(e);

	}

	if (finPlaces.fail() || finLinks.fail()) { 
		return false;
	}
	else {
		return true;
	}
}

// Add your code here


