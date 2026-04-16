/*
ὄνομα: quadtree
αἰών: 4/13/2026
βούλημα: game
*/

#include "quadtree.h"
#include "gameobject.h"

bool AABB::contains(Vec<float> point) const {
    Vec<float> displacement = point - center;
    return std::abs(displacement.x) < half_dimension.x && std::abs(displacement.y) < half_dimension.y;
}

bool AABB::intersects(AABB other) const {
    Vec<float> displacement = other.center - center;
    return std::abs(displacement.x) < (half_dimension.x - other.half_dimension.x) &&
        std::abs(displacement.y) < (half_dimension.y - other.half_dimension.y);
}

Quadtree::Quadtree(AABB boundary):boundary{boundary} {

}

void Quadtree::clear() {
    objects.clear();
    n_e = nullptr;
    n_w = nullptr;
    s_e = nullptr;
    s_w = nullptr;
}

bool Quadtree::insert(GameObject *obj) {
    // ignore objects not in this node
    if (!boundary.contains(obj->get_bounding_box().center)) {
        return false;
    }
    //if space, add it
    if (objects.size() < NODE_CAPACITY && n_w == nullptr) {
        objects.push_back(obj);
        return true;
    }

    if (n_w == nullptr) {
        subdivide();
    }

    return n_w->insert(obj) || n_e->insert(obj) || s_w->insert(obj) || s_e->insert(obj);
}

std::vector<GameObject *> Quadtree::query_range(AABB range) const {
    if (!boundary.intersects(range)) {
        return {};
    }

    // handle leaf node
    std::vector<GameObject*> results;
    if (n_w == nullptr) {
        std::copy_if(std::begin(objects), std::end(objects), std::back_inserter(results),
                [&](GameObject* obj) {return range.intersects(obj->get_bounding_box());}
        );
        return results;
    }

    // add objects from children
    auto pts = n_w->query_range(range);
    results.insert(std::end(results), std::begin(pts), std::end(pts));
    pts = n_e->query_range(range);
    results.insert(std::end(results), std::begin(pts), std::end(pts));
    pts = s_w->query_range(range);
    results.insert(std::end(results), std::begin(pts), std::end(pts));
    pts = s_e->query_range(range);
    results.insert(std::end(results), std::begin(pts), std::end(pts));

    return results;
}

void Quadtree::subdivide() {
    Vec<float> half = boundary.half_dimension / 2.0f;

    s_w = std::make_shared<Quadtree>(AABB{{boundary.center.x - half.x, boundary.center.y - half.y}, half});
    s_e = std::make_shared<Quadtree>(AABB{{boundary.center.x + half.x, boundary.center.y - half.y}, half});
    n_w = std::make_shared<Quadtree>(AABB{{boundary.center.x - half.x, boundary.center.y + half.y}, half});
    n_e = std::make_shared<Quadtree>(AABB{{boundary.center.x + half.x, boundary.center.y + half.y}, half});

    for (auto obj : objects) {
        insert(obj);
    }
    objects.clear();
}
