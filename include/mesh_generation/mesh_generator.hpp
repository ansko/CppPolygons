#pragma once


#include <memory>
#include <vector>

#include "../geometries/point.hpp"
#include "../geometries/polygon.hpp"
#include "../geometries/polygonal_cylinder.hpp"
#include "mesh.hpp"


class MeshGenerator
{
public:
    MeshGenerator(std::vector<std::shared_ptr<PolygonalCylinder> > pc_ptrs);
    void generateMesh();
    Mesh getMesh();
protected:
private:
    std::vector<std::shared_ptr<PolygonalCylinder> > __pc_ptrs;
    Mesh mesh;
};
