#include "Edge.h"
#include "Node.h"




Edge::Edge(Node* &pStartNode, Node* &pEndNode, const double &pWeight, const TransMode &pTransmode)
	:startNode(pStartNode),
	endNode(pEndNode),
	weight(pWeight),
	transMode(pTransmode)
{

}


Node * Edge::getStartNode()
{
	return startNode;
}

Node * Edge::getEndNode()
{
	return endNode;
}

double Edge::getWeight()
{
	return this->weight;
}

bool Edge::canBeTraversed(const TransMode & pTransMode)
{
	if (pTransMode == transMode) {
		return true;
	}
	else if (transMode == Rail) {
		if		(pTransMode == Bike) return true;
		else if (pTransMode == Foot) return true;

		return false;

	}
	else if (transMode == Ship) {

		if		(pTransMode == Bus) return true;
		else if (pTransMode == Car) return true;
		else if (pTransMode == Bike) return true;
		else if (pTransMode == Foot) return true;

		return false;
	}
	else if (transMode == Bus) {

		if		(pTransMode == Car)	 return true;
		else if (pTransMode == Bike) return true;
		else if (pTransMode == Foot) return true;

		return false;
	
	}
	else if (transMode == Car) {

		if		(pTransMode == Bike) return true;
		else if (pTransMode == Foot) return true;

		return false;
	}
	else if (transMode == Bike) {

		if (pTransMode == Foot) return true;
		
		return false;
	}

	return false;
}


Edge::~Edge()
{
}
