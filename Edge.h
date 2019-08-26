#pragma once
#include <string>

using namespace std;

enum TransMode {Foot, Bike, Car, Bus, Rail, Ship };

class Node;

class Edge
{
private:

	Node* startNode;
	Node* endNode;
	double weight;
	TransMode transMode;

public:
	Edge(Node* &pStartNode, Node* &pEndNode, const double &pWeight, const TransMode &pVehicleType);
	Node* getStartNode();
	Node* getEndNode();
	double getWeight();
	bool canBeTraversed(const TransMode &pTransMode);
	~Edge();
};

