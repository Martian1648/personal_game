/*
ὄνομα: quadtree
αἰών: 4/13/2026
βούλημα: game
*/

#pragma once
#include <memory>
#include <vector>
#include "vec.h"

class GameObject;

//axis aligned bounding box
struct AABB {
    Vec<float> center;
    Vec<float> half_dimension;

    bool contains(Vec<float> point) const;
    bool intersects(AABB other) const;
};

class Quadtree {
public:
    Quadtree(AABB boundary);
    void clear();

    bool insert(GameObject* obj);
    std::vector<GameObject*> query_range(AABB range) const;
    void subdivide();

    // how many elements can be stored
    static constexpr std::size_t NODE_CAPACITY = 4;

    // boundry of this node
    AABB boundary;

    // objects in this node
    std::vector<GameObject*> objects;

    //children
    std::shared_ptr<Quadtree> n_w, n_e, s_w, s_e;

};