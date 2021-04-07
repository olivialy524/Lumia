//
//  GraphNode.h
//  Lumia
//
//  Created by Olivia Li on 4/5/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef GraphNode_h
#define GraphNode_h
#include <cugl/cugl.h>
using namespace cugl;
using namespace std;
enum NodeState {
    /**When the node is empty or an object that the enemy can walk through*/
    Void,
    /**When the node is not walkable, such as a tile or a spike*/
    Obstacle,
    /**When the node is a Lumia*/
    Lumia
};

struct Node
{
    Vec2 position;
    
    bool operator==(const Node &other) const
      {
          Vec2 diff = other.position - position;
          return Vec2::ZERO <= diff && diff < Vec2::ONE;
      }
};

template<> struct hash<Node>
{
std::size_t operator()(const Node& n) const
{
  hash<float> hash_float;
  return (hash_float(n.position.x)
           ^ hash_float(n.position.y) << 1);
}
};

#endif /* GraphNode_h */
