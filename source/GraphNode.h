//
//  GraphNode.h
//  Lumia
//
//  Created by Olivia Li on 4/13/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef GraphNode_h
#define GraphNode_h

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
    bool operator==(const Node &other) const
    {
          return x==other.x && y == other.y;
    }
};

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
