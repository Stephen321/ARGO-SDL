#ifndef GRAPHARC_H
#define GRAPHARC_H

#include "GraphNode.h"
#include <iostream>

#include "SDL.h" //for debuging

using namespace helper;

template<class DataType, class NodeType, class ArcType>
class GraphArc {
private:

	GraphNode<DataType, NodeType, ArcType>* m_pNode;

    ArcType m_weight;

	SDL_Point points[2];

public:

	
	virtual void draw(SDL_Renderer* renderer) const{
		SDL_RenderDrawLines(renderer, points, 2);
	}

	void setLine(helper::Vector2 start, helper::Vector2 end){
		points[0] = { (int)start.x, (int)start.y };
		points[1] = { (int)end.x, (int)end.y };
	}
	
    // Accessor functions
    GraphNode<DataType, NodeType, ArcType>* node() const {
        return m_pNode;
    }
                              
    ArcType weight() const {
        return m_weight;
    }
    
    // Manipulator functions
    void setNode(GraphNode<DataType, NodeType, ArcType>* pNode) {
		m_pNode = pNode;
    }
    
    void setWeight(ArcType weight) {
       m_weight = weight;
    }
    
};

#endif
