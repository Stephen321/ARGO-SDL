#include "GraphNode.h"
#include "GraphArc.h"

GraphNode::GraphNode() :
	m_prevNode(0),
	m_hCost(-1),
	m_gCost(-1) {

	_color = { 255,255,255,255 };
	/*
	m_shape = sf::CircleShape(RADIUS);
	m_shape.setOrigin(RADIUS, RADIUS);


	m_nameTxt = sf::Text("", font, 15);
	m_nameTxt.setOrigin(4, 8);
	m_hCostTxt = sf::Text("H(n)= ?", font, 11);
	m_hCostTxt.setOrigin(7, 7);
	m_gCostTxt = sf::Text("G(n)= ?", font, 11);
	m_gCostTxt.setOrigin(7, 7);*/
}



GraphArc* GraphNode::getArc(GraphNode* pNode) {

	list<GraphArc>::iterator iter = m_arcList.begin();
	list<GraphArc>::iterator endIter = m_arcList.end();
	GraphArc* pArc = 0;

	// find the arc that matches the node
	for (; iter != endIter && pArc == 0; ++iter) {
		if ((*iter).node() == pNode) {
			pArc = &((*iter));
		}
	}

	// returns null if not found
	return pArc;
}


void  GraphNode::reset() {
	m_marked = false;
	m_prevNode = 0;
	//m_shape.setFillColor(sf::Color::Blue);
	m_hCost = -1;
	m_gCost = -1;

	//m_hCostTxt.setString("H(n)= ?");
	//m_gCostTxt.setString("G(n)= ?");
}


void GraphNode::addArc(GraphNode* pNode, float weight) {
	// Create a new arc.
	GraphArc a;
	a.setNode(pNode);
	a.setWeight(weight);
	a.setLine(m_position, pNode->getPosition());
	// Add it to the arc list.
	m_arcList.push_back(a);
}


// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This finds an arc from this node to input node 
//                  and removes it.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

void GraphNode::removeArc(GraphNode* pNode) {
	list<GraphArc>::iterator iter = m_arcList.begin();
	list<GraphArc>::iterator endIter = m_arcList.end();

	int size = m_arcList.size();
	// find the arc that matches the node
	for (; iter != endIter && m_arcList.size() == size;
		++iter) {
		GraphNode* node = (*iter).node();
		/*
		if (node == pNode) {
		m_arcList.remove( (*iter) );
		}          */

	}
}



void GraphNode::drawArcs(SDL_Renderer* renderer, Camera* camera) const {
	int arcs = m_arcList.size();
	for (auto a : m_arcList)
		a.draw(renderer, camera);
}


void GraphNode::drawNodes(SDL_Renderer* renderer, Camera* camera) const {
	SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);
	Point point = { (float)(m_position.x - 10), (float)(m_position.y - 10) };
	point = camera->worldToScreen(point);

	SDL_Rect rect = { point.x,point.y, 20, 20 };
	SDL_RenderFillRect(renderer, &rect);
}