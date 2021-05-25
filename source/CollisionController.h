//
//  CollisionController.h
//  Lumia
//
//  Created by Olivia Li on 4/12/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef CollisionController_h
#define CollisionController_h
#include "LumiaModel.h"
#include "EnemyModel.h"
#include "Plant.h"
#include "EnergyModel.h"
#include "Button.h"
#include "SlidingDoor.h"
#include "StickyWallModel.h"

class CollisionController {
public:

#pragma mark -
#pragma mark Enums
    /** Information representing a Lumia to create */
    struct LumiaBody {
        /** The position to spawn the Lumia body at */
        cugl::Vec2 position;
        /** The size level of the Lumia body */
        int sizeLevel;
        /** Whether or not the Lumia body should be controlled by the player */
        bool isAvatar;
        /** The velocity to spawn the Lumia body with */
        cugl::Vec2 vel;
        /** The angular velocity to spawn the Lumia body with */
        float angularVel;
    };

protected:

    /** List of Lumia bodies to remove in next update step */
    std::list<std::shared_ptr<LumiaModel>> _lumiasToRemove;
    /** List of Lumia bodies to create in next update step */
    std::list<LumiaBody> _lumiasToCreate;
    /** List of Lumia bodies to stick in next update step */
    std::list<std::shared_ptr<LumiaModel>> _lumiasToStick;
    /** List of Lumia bodies to Unstick in next update step */
    std::list<std::shared_ptr<LumiaModel>> _lumiasToUnstick;
    /** List of energy item sto remove in next update step */
    std::list<std::shared_ptr<EnergyModel>> _energiesToRemove;
    
    std::list<std::shared_ptr<SlidingDoor>> _doorsToOpen;

    /** List of Lumia bodies to remove in next update step */
    std::list<std::shared_ptr<EnemyModel>> _enemiesToRemove;
    
    bool _didLightup;
    
    bool _didAbsorbEnergy;
    
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new collision controller.
     *
     * This constructor does NOT do any initialzation.  It simply allocates the
     * object. This makes it safe to use this class without a pointer.
     */
    CollisionController(); // Don't initialize.  Allow stack based
    
    /**
     * Disposes of this input controller, releasing all listeners.
     */
    ~CollisionController() { dispose(); }
    
    /**
     * Deactivates this collision controller, releasing all listeners.
     *
     * This method will not dispose of the input controller. It can be reused
     * once it is reinitialized.
     */
    void dispose();
    
    /**
     * @return true if the controller was initialized successfully
     */
    bool init();
    
 
#pragma mark -
#pragma mark Collision Handling
    /** Processes a collision between Lumia and a magical plant */
    void processPlantLumiaCollision(int newSize, const std::shared_ptr<LumiaModel> lumia, bool isAvatar);

    /** Processes a collision between Lumia and a spike */
    void processSpikeLumiaCollision(int newSize, const std::shared_ptr<LumiaModel> lumia, bool isAvatar);

    /** Processes a collision between Lumia and a magical plant */
    void processEnemyLumiaCollision(const std::shared_ptr<EnemyModel> enemy,const std::shared_ptr<LumiaModel> lumia, bool isAvatar);

    /** Processes a collision between Lumia and an energy item */
    void processEnergyLumiaCollision(const std::shared_ptr<EnergyModel> energy, const std::shared_ptr<LumiaModel> lumia, bool isAvatar);

    /** Processes a collision between Lumia and another Lumia */
    void processLumiaLumiaCollision(const std::shared_ptr<LumiaModel> lumia, const std::shared_ptr<LumiaModel> lumia2, bool isAvatar);

    void processButtonLumiaCollision(const std::shared_ptr<LumiaModel> lumia, const std::shared_ptr<Button> button);

    void processButtonLumiaEnding(const std::shared_ptr<LumiaModel> lumia, const std::shared_ptr<Button> button);
    
    void processStickyWallLumiaCollision(const std::shared_ptr<LumiaModel> lumia, const StickyWallModel* stickyWall);
    
    void processStickyWallLumiaEnding(const std::shared_ptr<LumiaModel> lumia);
    
    void clearStates();
    
    void addLumiaToRemove(std::shared_ptr<LumiaModel> lumia){
        _lumiasToRemove.push_back(lumia);
    }

#pragma mark -
#pragma mark Attributes
    std::list<std::shared_ptr<LumiaModel>> getLumiasToRemove(){
        return _lumiasToRemove;
    }
    
    std::list<LumiaBody> getLumiasToCreate(){
        return _lumiasToCreate;
    }
    
    std::list<std::shared_ptr<LumiaModel>> getLumiasToStick(){
        return _lumiasToStick;
    }

    std::list<std::shared_ptr<LumiaModel>> getLumiasToUnstick(){
        return _lumiasToUnstick;
    }
    
    std::list<std::shared_ptr<EnergyModel>> getEnergiesToRemove(){
        return _energiesToRemove;
    }
    
    std::list<std::shared_ptr<EnemyModel>> getEnemiesToRemove(){
        return _enemiesToRemove;
    }
    
    std::list<std::shared_ptr<SlidingDoor>> getDoorsToOpen(){
        return _doorsToOpen;
    }
    
    bool didLightup(){
        return _didLightup;
    }
    
    bool didAbsorbEnergy(){
        return _didAbsorbEnergy;
    }
};

#endif /* CollisionController_h */
