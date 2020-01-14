#pragma once
#include <list>
#include <vector>

class PNode {
private:
	std::pair<float, float> pos;
	bool traversable;
	float cost;
public:
	PNode(float x, float y, bool traversableIn) {
		pos.first = x;
		pos.second = y;
		traversable = traversableIn;
	}

	bool isTraversableI() {
		return traversable;
	}

	std::pair<float, float> getPos(){
		return pos;
	}
};

class Pathfinding {
private:
	std::list<PNode> openList;
	std::list<PNode> closedList;

	std::vector<std::vector<PNode>> vNodes;

	PNode* startingNode;
	PNode* targetNode;
	PNode* currentNode;

public:

	void initialiseNodes() {
		//ADD NODES TO 2 dimensional Vector
	}

	void getPath()
	{
		openList.clear();
		closedList.clear();

		currentNode = new PNode(0, 0, true); //TO BE REPLACED WITH LIVE FIGURES

		startingNode = currentNode;


		closedList.emplace_back(*startingNode);

		//add adjacent squares to open list 

		for (int i = 0; i < openList.size(); i++) {
			
		}

	}

	float getCost(PNode* a, PNode* b)
	{
		float gCost,hCost,tCost;

		//gCost = 
		//CALCULATE COST

		return tCost;
	}

	void addToOpenList(PNode a)
	{
		openList.emplace_back(a);
	}

	void addToClosedList(PNode a)
	{
		closedList.emplace_back(a);
	}



};