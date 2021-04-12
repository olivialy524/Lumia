//
//  PathFindingController.h
//  Lumia
//
//  Created by Olivia Li on 4/6/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef PathFindingController_h
#define PathFindingController_h
#include "GraphNode.h"
#include "EnemyModel.h"
#include "LumiaModel.h"

class PathFindingController {
protected:
    float xBound;
    
    float yBound;
    
    std::unordered_map<Node, NodeState> _graph;
    
    int ticks;
    
    const int NUM_TICKS;
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new input controller.
     *
     * This constructor does NOT do any initialzation.  It simply allocates the
     * object. This makes it safe to use this class without a pointer.
     */
    PathFindingController(); // Don't initialize.  Allow stack based
    
    /**
     * Disposes of this input controller, releasing all listeners.
     */
    ~PathFindingController() { dispose(); }
    
    /**
     * Deactivates this input controller, releasing all listeners.
     *
     * This method will not dispose of the input controller. It can be reused
     * once it is reinitialized.
     */
    void dispose();
    
    /**
     * Initializes the input control for the given bounds
     *
     * The bounds are the bounds of the scene graph.  This is necessary because
     * the bounds of the scene graph do not match the bounds of the display.
     * This allows the input device to do the proper conversion for us.
     *
     * @param bounds    the scene graph bounds
     *
     * @return true if the controller was initialized successfully
     */
    bool init(const float xBound, const float yBound);
    
#pragma mark -
#pragma mark Change Path Finding States
    
    void changeGraphNode(float x, float y, NodeState node);
    
    void findPath(std::shared_ptr<EnemyModel> e);
    
    void changeStateIfApplicable(std::shared_ptr<EnemyModel> e);
    
    void update(float dt, std::list<std::shared_ptr<EnemyModel>>& _enemyList, std::list<std::shared_ptr<LumiaModel>>& _lumiaList);

    /**
     * Clears any buffered inputs so that we may start fresh.
     */
    void clear();
    
#endif /* PathFindingController_h */
