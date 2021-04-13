//
//  PathFindingController.cpp
//  Lumia
//
//  Created by Olivia Li on 4/6/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "PathFindingController.h"

#pragma mark -
#pragma mark Constructors
/**
 * Creates a new input controller.
 *
 * This constructor does NOT do any initialzation.  It simply allocates the
 * object. This makes it safe to use this class without a pointer.
 */
PathFindingController::PathFindingController():
_ticks(0),
NUM_TICKS(100)
{}

/**
 * Deactivates this path finding controller
 *
 */
void PathFindingController::dispose(){
    clear();
    _ticks = 0;
}

/**
 * Initializes the graph with the given bounds
 *
 * @param bounds    the graph bounds in world coordinates
 *
 * @return true if the controller was initialized successfully
 */
bool PathFindingController::init(const float xBound, const float yBound){
    for (float i = 0.0f; i < xBound; i+=1.0f){
        for (float j = 0.0f; j < yBound; j+=1.0f){
            _graph[{i, j}] = NodeState::Void;
        }
    }
    return true;
}

#pragma mark -
#pragma mark Change Path Finding States

void PathFindingController::changeGraphNode(float x, float y, NodeState node){
    _graph[{floor(x), floor(y)}] = node;
}

void PathFindingController::findPath(std::shared_ptr<EnemyModel> e){
    
}

void PathFindingController::changeStateIfApplicable(std::shared_ptr<EnemyModel> e){
    
}

void PathFindingController::update(float dt, std::list<std::shared_ptr<EnemyModel>>& enemyList, std::list<std::shared_ptr<LumiaModel>>& lumiaList){
    _ticks = (_ticks + 1) % NUM_TICKS;
    
    for (auto & lumia : lumiaList){
        Vec2 lastPos = lumia->getLastPosition();
        changeGraphNode(lastPos, NodeState::Void);
        Vec2 curPos = lumia->getPosition();
        changeGraphNode(curPos, NodeState::Lumia);
    }
    for (auto & enemy : enemyList){
        Vec2 lastPos = enemy->getLastPosition();
        changeGraphNode(lastPos, NodeState::Void);
        Vec2 curPos = enemy->getPosition();
        changeGraphNode(curPos, NodeState::Enemy);
    }
    if (_ticks != 0){
        return;
    }
    for (auto & e : enemyList){
        changeStateIfApplicable(e);
    }
}

/**
 * Clears the graph so that we may start fresh.
 */
void PathFindingController::clear(){
    _graph.clear();
}
