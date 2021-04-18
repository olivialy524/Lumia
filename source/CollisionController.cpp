//
//  CollisionController.cpp
//  Lumia
//
//  Created by Olivia Li on 4/12/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "CollisionController.h"

CollisionController::CollisionController():
_switchLumia(false)
{}

void CollisionController::clearStates(){
    _lumiasToRemove.clear();
    _lumiasToCreate.clear();
    _doorsToOpen.clear();
    _enemiesToRemove.clear();
    _energiesToRemove.clear();
    _switchLumia = false;
}

void CollisionController::processPlantLumiaCollision(int newSize, const std::shared_ptr<LumiaModel> lumia, bool isAvatar) {
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
        // if avatar is killed, player is given control of nearest Lumia body
        if (isAvatar) {
            _switchLumia = true;
        }
        _lumiasToRemove.push_back(lumia);
        lumia->setRemoved(true);
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
        // if avatar is killed, player is given control of nearest Lumia body
        if (isAvatar) {
            _switchLumia = true;
        }
        _lumiasToRemove.push_back(lumia);
        lumia->setRemoved(true);
    }
}

void CollisionController::processEnergyLumiaCollision(const std::shared_ptr<EnergyModel> energy, const std::shared_ptr<LumiaModel> lumia, bool isAvatar) {
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
            struct LumiaBody lumiaNew = {
                newPosition,
                newSize,
                false,
                (lumia->getLinearVelocity() + lumia2->getLinearVelocity()) / 2,
                lumia->getAngularVelocity()
            };

            _lumiasToCreate.push_back(lumiaNew);
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
    button->getDoor()->setOpening(true);
    button->getDoor()->setClosing(false);
}

void CollisionController::processButtonLumiaEnding(const std::shared_ptr<LumiaModel> lumia, const std::shared_ptr<Button> button) {
    button->getDoor()->setOpening(false);
    button->getDoor()->setClosing(true);
}

void CollisionController::dispose(){
    clearStates();
}

bool CollisionController::init(){
    _switchLumia = false;
    
    // TODO: add listeners
    return true;
}
