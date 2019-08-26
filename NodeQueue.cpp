#include "NodeQueue.h"



NodeQueue::NodeQueue()
{
}

void NodeQueue::AddNode(Node* pNodePtr)
{
	queue.push_front(pNodePtr);
}

//bubble sorts the queue
void NodeQueue::Sort()
{
	queue.sort([](Node* a, Node* b) {return a->getDistanceFromStart() < b->getDistanceFromStart(); });
}

Node * NodeQueue::ReturnAndPopFront()
{
	
	Node* shortestNode = queue.front();
	queue.pop_front();
	return shortestNode;
}

bool NodeQueue::CheckEmpty()
{
	return queue.empty();
}




NodeQueue::~NodeQueue()
{
}
