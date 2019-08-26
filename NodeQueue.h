#pragma once
#include <forward_list>
#include <list>
#include "Node.h"
class NodeQueue
{
private:
	forward_list<Node*> queue;
public:
	NodeQueue();
	void AddNode(Node* pNodePtr);
	void Sort();
	Node* ReturnAndPopFront();
	bool CheckEmpty();
	~NodeQueue();
};

