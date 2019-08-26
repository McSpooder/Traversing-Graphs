#pragma once
#include <string>
#include <vector>
#include "Edge.h"

using namespace std;

class Node
{
private:
	string name;
	const int reference;
	const double x;
	const double y;
	vector<Edge*> edges;

	//used for Dijkstras algorithm
	bool visited;
	double distanceFromStart;
	int previousNodeRef;

	//nodes are unique and shouldn't be copied
	Node* operator=(const Node& rhs);
	Node(const Node& rhs);
public:
	bool inQueue;

	Node(const string &pName, const int &pReference, const double &pX, const double &pY);
	void addEdge(Edge *pEdge);
	void visitNode();
	void resetVisited();
	void distanceFromStartZero();
	void setDistanceFromStart(const double &pDistance);
	void setPreviousNodeRef(const int& pPreviousNodeRef);

	int getPreviousNodeRef();
	double getX();
	double getY();
	int getReference();
	bool isVisited();
	double getDistanceFromStart() const;
	string getName();
	vector<Edge*> getEdges();

	~Node();
};

