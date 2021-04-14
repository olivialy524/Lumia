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
NUM_TICKS(50),
CHASE_DIST(50)
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

bool PathFindingController::isValid(Node n){
    bool b = _graph.find(n) != _graph.end();
    return b;
}

bool PathFindingController::isVoid(Node n){
    bool b =_graph[n] == NodeState::Void || _graph[n] == NodeState::Lumia;
    return b;
}

void PathFindingController::changeGraphNode(float x, float y, NodeState node){
    _graph[getNodeFromPos(x, y)] = node;
}

void PathFindingController::findPath(std::shared_ptr<EnemyModel> e){
// A* search algorithm for enemy
    std::priority_queue<Node, vector<Node>, Node_Compare> frontier;
    Vec2 targetPos = e -> getTarget() -> getPosition();
    Vec2 curPos = e->getPosition();
    Node goal = getNodeFromPos(targetPos);
    Node start = getNodeFromPos(curPos);
    Node cur;

    frontier.push(start);
    start.cost = heuristic(start, goal);

    came_from[start] = start;
    cost_so_far[start] = 0;
    
    while (!frontier.empty()) {
        cur = frontier.top();
        frontier.pop();
        if (_graph[cur]==NodeState::Lumia){
            break;
        }
        
        for (Node dir : directions){
            Node next = cur + dir;
            if (!(isValid(next) &&isVoid(next))){
                continue;
            }
            float new_cost = cost_so_far[cur] + 1.0f;
            if (cost_so_far.find(next) == cost_so_far.end() // not visited
                || new_cost < cost_so_far[next]) { // closer
                cost_so_far[next] = new_cost;
                float priority = new_cost + heuristic(next, goal);
                next.cost = priority;
                frontier.push(next);
                if (cur == start){
                    came_from[next] = next;
                }else{
                    came_from[next] = came_from[cur];
                }
            }
      }
    }
    Vec2 nextStep = getPosFromNode(came_from[cur]);
    Vec2 dir = nextStep-curPos;
    e->setVelocity(dir.normalize() * 2.0f);
    came_from.clear();
    cost_so_far.clear();
    
}

void PathFindingController::setEnemyTarget(std::shared_ptr<EnemyModel> enemy, std::list<std::shared_ptr<LumiaModel> > &lumiaList){
    std::shared_ptr<LumiaModel> closestLumia;
    Vec2 enemyPos = enemy->getPosition();
    float dist = numeric_limits<float>::infinity();
    for (auto & lumia : lumiaList){
        if (lumia->isRemoved()){
            continue;
        }
        Vec2 lumiaPos = lumia->getPosition();
        if (enemyPos.distanceSquared(lumiaPos) < dist){
            dist = enemyPos.distanceSquared(lumiaPos);
            closestLumia = lumia;
        }
    }
    enemy->setTarget(closestLumia);
  
}

void PathFindingController::changeStateIfApplicable(std::shared_ptr<EnemyModel> e, std::list<std::shared_ptr<LumiaModel>>& lumiaList){
    std::shared_ptr<LumiaModel> closestLumia = e -> getTarget();
    if (closestLumia == nullptr || closestLumia -> isRemoved()){
        e->setState(EnemyModel::Wander);
        return;
    }
    Vec2 lumiaPos = closestLumia->getPosition();
    Vec2 enemyPos = e->getPosition();
    float dist = enemyPos.distanceSquared(lumiaPos);
    Vec2 distance = lumiaPos-enemyPos;
    
    switch (e->getState()){
        case EnemyModel::EnemyState::Wander:{
            CULog("wander");
            e->setVelocity(Vec2::ZERO);
            if (dist <= CHASE_DIST){

                e->setState(closestLumia->getSizeLevel() <= e-> getSizeLevel() ? EnemyModel::Chasing : EnemyModel::Fleeing);
            }
            break;
        }
        case EnemyModel::EnemyState::Chasing:{
            CULog("chase");
            if (dist > CHASE_DIST){
                e->setState(EnemyModel::Wander);
            } else if(closestLumia->getSizeLevel() > e-> getSizeLevel()){
                e->setState(EnemyModel::Fleeing);
            } else{
                findPath(e);
            }
            break;
        }
        case EnemyModel::EnemyState::Fleeing:{
            CULog("flee");
            if (dist > CHASE_DIST){
                e->setState(EnemyModel::Wander);
            } else if(closestLumia->getSizeLevel() > e-> getSizeLevel()){
                e->setVelocity(-distance.normalize() * 2.0f);
            } else {
                e->setState(EnemyModel::Chasing);
            }
            break;
        }
    }
}

void PathFindingController::update(float dt, std::list<std::shared_ptr<EnemyModel>>& enemyList, std::list<std::shared_ptr<LumiaModel>>& lumiaList){
    _ticks++;
    
    for (auto & lumia : lumiaList){
        if (lumia->isRemoved()){
            continue;
        }
        Vec2 lastPos = lumia->getLastPosition();
        changeGraphNode(lastPos, NodeState::Void);
        Vec2 curPos = lumia->getPosition();
        changeGraphNode(curPos, NodeState::Lumia);
    }
    for (auto & enemy : enemyList){
        Vec2 lastPos = enemy->getLastPosition();
        changeGraphNode(lastPos, NodeState::Void);
        
        if (enemy -> getRemoved()){
            continue;
        }
        Vec2 curPos = enemy->getPosition();
        changeGraphNode(curPos, NodeState::Enemy);
        auto target =enemy->getTarget();
        if (target==nullptr || target ->isRemoved() || _ticks % 100 == 0){
            setEnemyTarget(enemy, lumiaList);
        }
        
        if (_ticks % 50 == 0){
            changeStateIfApplicable(enemy, lumiaList);
            enemy->setInCoolDown(false);
            
        }
    }
}

/**
 * Clears the graph so that we may start fresh.
 */
void PathFindingController::clear(){
    _graph.clear();
    came_from.clear();
    cost_so_far.clear();
}
