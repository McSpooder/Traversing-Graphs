#include "Node.h"



Node::Node(const string &pName, const int &pReference, const double &pX, const double &pY)
	:name(pName),
	reference(pReference),
    x(pX),
	y(pY),
	visited(false),
	inQueue(false),
	distanceFromStart(9999999999) //ideally should be infinite but in this case just a large number
{

}


string Node::getName() 
{
	return this->name;
}

vector<Edge*> Node::getEdges()
{
	return this->edges;
}

int Node::getPreviousNodeRef()
{
	return previousNodeRef;
}

double Node::getX()
{
	return this->x;
}


double Node::getY()
{
	return this->y;
}

int Node::getReference()
{
	return this->reference;
}

bool Node::isVisited()
{
	return this->visited;
}

double Node::getDistanceFromStart() const
{
	return distanceFromStart;
}

void Node::addEdge(Edge *pEdge)
{
	edges.push_back(pEdge);
}

void Node::visitNode()
{
	visited = true;
}

void Node::resetVisited()
{
	visited = false;
}

void Node::distanceFromStartZero()
{
	distanceFromStart = 0;
}

void Node::setDistanceFromStart(const double &pDistance)
{
	distanceFromStart = pDistance;
}

void Node::setPreviousNodeRef(const int & pPreviousNodeRef)
{
	previousNodeRef = pPreviousNodeRef;
}

Node::~Node()
{
}
