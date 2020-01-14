#pragma once

#include <list>
#include "AIStructs.h"

class AINode {
public:

	virtual bool run(AIData* a) = 0;

};

class CompositeNode : public AINode {
private:

	std::list<AINode*> children;

public:

	const std::list<AINode*>& getChildren() const {
		return children;
	}

	void addChild(AINode* child) {
		children.emplace_back(child);
	}

};

class Selector : public CompositeNode {
public : 

	//if at least 1 child is true return true
	bool run(AIData* a) override {
		for (AINode* child : getChildren()) {
			if (child->run(a)) {
				return true;
			}
		}

		return false;
	}
};

class Sequence : public CompositeNode {
public:
	//if at least 1 child fails then return false
	bool run(AIData* a) override {
		for (AINode* child : getChildren()) {
			if (!child->run(a)) {
				return false;
			}
		}

		return true;
	}

};