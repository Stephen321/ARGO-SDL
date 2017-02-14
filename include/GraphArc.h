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
	SDL_Color _color;
public:

	GraphArc() {
		_color = SDL_Color{ 255,255,255,255 };
	}

	virtual void draw(SDL_Renderer* renderer, Camera* camera) const {
		SDL_SetRenderDrawColor(renderer, _color.r,_color.g,_color.b,_color.a);
		SDL_Point drawPoint[2];
		for (int i = 0; i < 2; i++)
		{
			Point point = { (float)points[i].x, (float)points[i].y };
			point = camera->worldToScreen(point);
			drawPoint[i].x = point.x;
			drawPoint[i].y= point.y;
		}
		SDL_RenderDrawLines(renderer, drawPoint, 2);
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
