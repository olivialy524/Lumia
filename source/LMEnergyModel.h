//
//  LMEnergyModel.h
//  Lumia
//
//  Created by Ben Dacek on 3/17/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//
#include <cugl/cugl.h>
#include <cugl/physics2/CUBoxObstacle.h>
#ifndef LMEnergyModel_h
#define LMEnergyModel_h
class EnergyModel : public cugl::physics2::BoxObstacle {
private:
    /** This macro disables the copy constructor (not allowed on physics objects) */
    CU_DISALLOW_COPY_AND_ASSIGN(EnergyModel);
protected:
    float _scale;
    std::shared_ptr<cugl::scene2::SceneNode> _sceneNode;
    std::shared_ptr<cugl::scene2::PolygonNode> _node;
public:
    EnergyModel() : cugl::physics2::BoxObstacle() { }

    virtual ~EnergyModel(void) { dispose(); }

    void dispose();

    static std::shared_ptr<EnergyModel> alloc() {
        std::shared_ptr<EnergyModel> result = std::make_shared<EnergyModel>();
        return (result->init() ? result : nullptr);
    }

    static std::shared_ptr<EnergyModel> alloc(const cugl::Vec2& pos) {
        std::shared_ptr<EnergyModel> result = std::make_shared<EnergyModel>();
        return (result->init(pos) ? result : nullptr);
    }

    static std::shared_ptr<EnergyModel> alloc(const cugl::Vec2& pos, cugl::Size size) {
        std::shared_ptr<EnergyModel> result = std::make_shared<EnergyModel>();
        return (result->init(pos, size) ? result : nullptr);
    }
    
    std::shared_ptr<cugl::scene2::PolygonNode> getNode() {
        return _node;
    }
    void setNode(std::shared_ptr<cugl::scene2::PolygonNode> n) {
        _node = n;
    }
};
#endif /* LMEnergyModel_h */
