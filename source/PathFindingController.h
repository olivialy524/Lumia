//
//  PathFindingController.h
//  Lumia
//
//  Created by Olivia Li on 4/6/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef PathFindingController_h
#define PathFindingController_h
#include "EnemyModel.h"
#include "LumiaModel.h"
#include "GraphNode.h"

class PathFindingController {
    
    
#pragma mark Attributes and Constants
protected:
    
    std::unordered_map<Node, NodeState> _graph;
    
    int _ticks;
    
    const int NUM_TICKS;
    
    const int CHASE_DIST;
    
#pragma mark -
#pragma mark Constructors
    
public:
    /**
     * Creates a new  path finding controller
     *
     * This constructor does NOT do any initialzation.  It simply allocates the
     * object. This makes it safe to use this class without a pointer.
     */
    PathFindingController(); // Don't initialize.  Allow stack based
    
    /**
     * Disposes of this  path finding controller
     */
    ~PathFindingController() { dispose(); }
    
    /**
     * Deactivates this path finding controller
     *
     * This method will not dispose of the  path finding controller. It can be reused
     * once it is reinitialized.
     */
    void dispose();
    
    /**
     * Inititialize the graph used in path finding
     *
     * @return true if the controller was initialized successfully
     */
    bool init(const float xBound, const float yBound);
    
#pragma mark -
#pragma mark Change Path Finding States
protected:
    /**
     * true if the node is in the graph
     */
    bool isValid(Node n);
    /**
     * true if the node is walkable
     */
    bool isVoid(Node n);
    /**
     * A* search pathfinding for enemy used to move toward the target Lumia
     */
    void findPath(std::shared_ptr<EnemyModel> e);
    /**
     * set the target Lumia for the enemy
     */
    void setEnemyTarget(std::shared_ptr<EnemyModel> e, std::list<std::shared_ptr<LumiaModel>>& lumiaList);
    /**
     * change the state of the enemy
     */
    void changeStateIfApplicable(std::shared_ptr<EnemyModel> e, std::list<std::shared_ptr<LumiaModel>>& lumiaList);
    
public:
    /**
     * change a node in the graph
     */
    void changeGraphNode(float x, float y, NodeState node);
    
    void changeGraphNode(Vec2 pos, NodeState node){
        changeGraphNode(pos.x, pos.y, node);
    }
    
    void update(float dt, std::list<std::shared_ptr<EnemyModel>>& enemyList, std::list<std::shared_ptr<LumiaModel>>& lumiaList);

    /**
     * Clears any graph states so that we may start fresh.
     */
    void clear();
};
    
#endif /* PathFindingController_h */
