//
//  GraphNode.h
//  Lumia
//
//  Created by Olivia Li on 4/13/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef GraphNode_h
#define GraphNode_h
#include <cugl/cugl.h>

enum NodeState {
    /**When the node is empty or an object that the enemy can walk through*/
    Void,
    /**When the node is not walkable, such as a tile or a spike*/
    Obstacle,
    /**When the node is a Lumia*/
    Lumia,
    /**When the node is an enemy*/
    Enemy
};

struct Node{
    float x;
    float y;
    float cost = numeric_limits<float>::infinity();
    bool operator==(const Node &other) const
    {
          return x==other.x && y == other.y;
    }
    
    Node operator+(const Node &b) const
    {
        return {x + b.x, y + b.y};
    }
};

struct Node_Compare{
    // ensures that the smallest roll_no is at the top of the priority queue
    bool operator()(const Node &a, const Node &b) const
    {
        return a.cost > b.cost;
    }
    
};

/**
 Eight directions to traverse the graph from current node
 */
inline Node directions[8] =
    {{-1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f},
        {-1.0f, 0.0f}, {1.0f, 0.0f},
        {-1.0f, 1.0f}, {0.0f, -1.0f}, {1.0f, -1.0f}
    };

/**
 Heuristic cost used in A* search
 */
inline float heuristic(Node a, Node b) {
  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

inline Vec2 getPosFromNode(Node n){
    return Vec2(n.x, n.y);
}

inline Node getNodeFromPos(float x, float y){
    return {floor(x), floor(y)};
}

inline Node getNodeFromPos(Vec2 pos){
    return getNodeFromPos(pos.x, pos.y);
}

namespace std{
    template<> struct hash<Node>
    {
    size_t operator()(const Node& n) const
    {
      hash<float> hash_float;
      return (hash_float(n.x)
               ^ (hash_float(n.y) << 1));
    }
    };
}


#endif /* GraphNode_h */
