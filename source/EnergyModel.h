//
//  LMEnergyModel.h
//  Lumia
//
//  Created by Ben Dacek on 3/17/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//
#ifndef EnergyModel_h
#define EnergyModel_h
#include <cugl/cugl.h>
#include <cugl/physics2/CUBoxObstacle.h>

class EnergyModel : public cugl::physics2::BoxObstacle {
private:
    /** This macro disables the copy constructor (not allowed on physics objects) */
    CU_DISALLOW_COPY_AND_ASSIGN(EnergyModel);
protected:
    float _scale;
    /* Whether or not this energy item is due to be or has been removed */
    bool _removed;
    std::shared_ptr<cugl::scene2::SceneNode> _sceneNode;
    std::shared_ptr<cugl::scene2::PolygonNode> _node;
public:
    EnergyModel() : cugl::physics2::BoxObstacle() { }

    virtual ~EnergyModel(void) { dispose(); }

    void dispose();

    virtual bool init() override { return init(cugl::Vec2::ZERO, cugl::Size::ZERO); }

    virtual bool init(const cugl::Vec2 pos) override { return init(pos, cugl::Size::ZERO); }

    virtual bool init(const cugl::Vec2 pos, cugl::Size size);

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

    void setRemoved(bool value) { _removed = value; }

    /* Returns whether or not this energy item is due to be or has been removed */
    bool getRemoved() { return _removed; }
};

#endif /* __LM_ENERGY_MODEL_H__ */
