//
//  CollisionController.cpp
//  Lumia
//
//  Created by Olivia Li on 4/12/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "CollisionController.h"

CollisionController::CollisionController(){}

void CollisionController::clearStates(){
    _lumiasToRemove.clear();
    _lumiasToCreate.clear();
    _doorsToOpen.clear();
    _enemiesToRemove.clear();
    _energiesToRemove.clear();
    _lumiasToStick.clear();
    _lumiasToUnstick.clear();
    _didLightup = false;
    _didAbsorbEnergy = false;
}

void CollisionController::processPlantLumiaCollision(int newSize, const std::shared_ptr<LumiaModel> lumia, bool isAvatar) {
    _didLightup = true;
    // Lumia body remains if above min size, otherwise kill Lumia body
    if (lumia->getSizeLevel() != newSize) {
        float radiusDiff = LumiaModel::sizeLevels[lumia->getSizeLevel()].radius - LumiaModel::sizeLevels[newSize].radius;
        Vec2 newPosition = Vec2(lumia->getPosition().x, lumia->getPosition().y - radiusDiff);
        struct LumiaBody lumiaNew = {
            newPosition,
            newSize,
            isAvatar,
            lumia->getLinearVelocity(),
            lumia->getAngularVelocity()
        };

        _lumiasToRemove.push_back(lumia);
        lumia->setRemoved(true);
        _lumiasToCreate.push_back(lumiaNew);
    } else {
        _lumiasToRemove.push_back(lumia);
        lumia->setRemoved(true);
        lumia->setDying(true);
    }
}

void CollisionController::processSpikeLumiaCollision(int newSize, const std::shared_ptr<LumiaModel> lumia, bool isAvatar) {
    // Lumia body remains if above min size, otherwise kill Lumia body
    if (lumia->getSizeLevel() != newSize) {
        float radiusDiff = LumiaModel::sizeLevels[lumia->getSizeLevel()].radius - LumiaModel::sizeLevels[newSize].radius;
        Vec2 newPosition = Vec2(lumia->getPosition().x, lumia->getPosition().y - radiusDiff);
        struct LumiaBody lumiaNew = {
            newPosition,
            newSize,
            isAvatar,
            lumia->getLinearVelocity(),
            lumia->getAngularVelocity()
        };

        _lumiasToRemove.push_back(lumia);
        lumia->setRemoved(true);
        _lumiasToCreate.push_back(lumiaNew);
    } else {
        _lumiasToRemove.push_back(lumia);
        lumia->setRemoved(true);
        lumia->setDying(true);
    }
}

void CollisionController::processEnemyLumiaCollision(const std::shared_ptr<EnemyModel> enemy, const std::shared_ptr<LumiaModel> lumia, bool isAvatar) {
    enemy->setInCoolDown(true);
    bool destroyEnemy = lumia->getSizeLevel() > enemy->getSizeLevel();
    enemy->setVelocity(Vec2::ZERO);

    if (destroyEnemy) {
        _enemiesToRemove.push_back(enemy);
        enemy->setRemoved(true);
    }

    int newSize = destroyEnemy ? lumia->getBiggerSizeLevel() : lumia->getSmallerSizeLevel();

    // Resize Lumia only if increasing/decreasing changed size level
    if (lumia->getSizeLevel() != newSize) {
        float diff = LumiaModel::sizeLevels[newSize].radius - LumiaModel::sizeLevels[lumia->getSizeLevel()].radius;
        Vec2 newPosition = Vec2(lumia->getPosition().x, lumia->getPosition().y + diff);
        struct LumiaBody lumiaNew = {
            newPosition,
            newSize,
            isAvatar,
            lumia->getLinearVelocity(),
            lumia->getAngularVelocity()
        };

        _lumiasToRemove.push_back(lumia);
        lumia->setRemoved(true);
        _lumiasToCreate.push_back(lumiaNew);
    } else if (lumia->getSizeLevel() == 0 && newSize == 0) {
        _lumiasToRemove.push_back(lumia);
        lumia->setRemoved(true);
        lumia->setDying(true);
    }
}

void CollisionController::processEnergyLumiaCollision(const std::shared_ptr<EnergyModel> energy, const std::shared_ptr<LumiaModel> lumia, bool isAvatar) {
    _didAbsorbEnergy = true;
    CULog("here");
    if (lumia->getSizeLevel() < LumiaModel::sizeLevels.size() - 1) {
        _energiesToRemove.push_back(energy);
        energy->setRemoved(true);

        int newSize = lumia->getBiggerSizeLevel();
        float radiusDiff = LumiaModel::sizeLevels[newSize].radius - LumiaModel::sizeLevels[lumia->getSizeLevel()].radius;
        Vec2 newPosition = Vec2(lumia->getPosition().x, lumia->getPosition().y + radiusDiff);
        struct LumiaBody lumiaNew = {
            newPosition,
            newSize,
            isAvatar,
            lumia->getLinearVelocity(),
            lumia->getAngularVelocity()
        };

        _lumiasToRemove.push_back(lumia);
        lumia->setRemoved(true);
        _lumiasToCreate.push_back(lumiaNew);
    }
}

void CollisionController::processLumiaLumiaCollision(const std::shared_ptr<LumiaModel> lumia, const std::shared_ptr<LumiaModel> lumia2, bool isAvatar) {
    // only merge Lumia bodies together if both of them aren't already at max size
    int maxSizeLevel = LumiaModel::sizeLevels.size() - 1;
    if (lumia->getSizeLevel() != maxSizeLevel && lumia2->getSizeLevel() != maxSizeLevel) {
        int newSize = lumia->getSizeLevel() + lumia2->getSizeLevel() + 1;

        if (newSize > maxSizeLevel) {
            int newSize2 = newSize - (maxSizeLevel + 1);
            newSize = maxSizeLevel;

            float newX = ((lumia->getPosition().x + lumia2->getPosition().x) / 2) + 0.5 + LumiaModel::sizeLevels[newSize2].radius;
            float newY = lumia->getPosition().y + (LumiaModel::sizeLevels[newSize2].radius - LumiaModel::sizeLevels[lumia->getSizeLevel()].radius);
            Vec2 newPosition = Vec2(newX, newY);
            struct LumiaBody lumiaNew2 = {
                newPosition,
                newSize2,
                false,
                (lumia->getLinearVelocity() + lumia2->getLinearVelocity()) / 2,
                lumia->getAngularVelocity()
            };

            _lumiasToCreate.push_back(lumiaNew2);
        }

        float newX = (lumia->getPosition().x + lumia2->getPosition().x) / 2;
        float newY = lumia->getPosition().y + (LumiaModel::sizeLevels[newSize].radius - LumiaModel::sizeLevels[lumia->getSizeLevel()].radius);
        Vec2 newPosition = Vec2(newX, newY);
        struct LumiaBody lumiaNew = {
            newPosition,
            newSize,
            isAvatar,
            (lumia->getLinearVelocity() + lumia2->getLinearVelocity()) / 2,
            lumia->getAngularVelocity()
        };
        
        _lumiasToRemove.push_back(lumia);
        lumia->setRemoved(true);
        _lumiasToRemove.push_back(lumia2);
        lumia2->setRemoved(true);

        _lumiasToCreate.push_back(lumiaNew);
    } 
}

void CollisionController::processButtonLumiaCollision(const std::shared_ptr<LumiaModel> lumia, const std::shared_ptr<Button> button) {
    button->setPushingDown(true);
    lumia->setOnButton(true);
    button->setLumia(lumia);
}

void CollisionController::processButtonLumiaEnding(const std::shared_ptr<LumiaModel> lumia, const std::shared_ptr<Button> button) {
    if (button -> getIsSlidingDoor()){
        button->getSlidingDoor()->setOpening(false);
        button->getSlidingDoor()->setClosing(true);
    }else{
        button->getShrinkingDoor()->setOpening(false);
        button->getShrinkingDoor()->setClosing(true);
    }
    button->setPushingDown(false);
    button->resetCD();
    if (lumia == button->getLumia()){
        lumia->setOnButton(false);
    }
}

void CollisionController::processStickyWallLumiaCollision(const std::shared_ptr<LumiaModel> lumia, const StickyWallModel* stickyWall){
    if (!lumia->getRemoved() && !lumia->isOnStickyWall()){
        lumia->setStickDirection(-stickyWall->getSurfaceNorm());
        _lumiasToStick.push_back(lumia);
    }
}


void CollisionController::processStickyWallLumiaEnding(const std::shared_ptr<LumiaModel> lumia){
    if (!lumia->getRemoved() && lumia->isOnStickyWall()){
        _lumiasToUnstick.push_back(lumia);
    }
}

void CollisionController::dispose(){
    clearStates();
}

bool CollisionController::init(){
    return true;
}
