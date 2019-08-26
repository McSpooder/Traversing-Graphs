#pragma once

#include <fstream>
#include <string>
#include "Node.h"
#include <list>

class Navigation
{
	std::ofstream _outFile;

	// Add your code here

public:
	Navigation();
	~Navigation();

	double CalculateDistance(const float &pX1, const float &pY1, const float &pX2, const float &pY2);

	void findRoute(Node* pTraversalNode, Node* pDestinationNode, bool& pFound, const TransMode& pTransMode, vector<int>& pReferences);
	bool BuildNetwork(const std::string& fileNamePlaces, const std::string& fileNameLinks);
	bool ProcessCommand(const std::string& commandString);
	Node* findNodeByRefference( int& reference);
	void pushNextNodeToStartOfQ(list<Node*>& nodeQueue);
	Node* findNextNodeToVisit();

	// Add your code here
};