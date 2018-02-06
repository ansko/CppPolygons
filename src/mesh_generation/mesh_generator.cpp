#include "../../include/mesh_generation/mesh_generator.hpp"


MeshGenerator::MeshGenerator(
    std::vector<std::shared_ptr<PolygonalCylinder> > pc_ptrs
)
{
    __pc_ptrs = pc_ptrs;
}

void MeshGenerator::generateMesh()
{
  /*
     Generates system of points that after will be vertices of simplexes.
  */
    std::vector<std::shared_ptr<Point> > pt_sur_ptrs;
    std::vector<std::shared_ptr<Point> > pt_vol_ptrs;

    for (uint i = 0; i < __pc_ptrs.size(); ++i) {
        uint tag = 0;
        std::pair<int, std::shared_ptr<Point> > pr;
        float r = __pc_ptrs[i]->getR();
        std::shared_ptr<Polygon> top_ptr = std::make_shared<Polygon>(
            __pc_ptrs[i]->topFacet());
        std::shared_ptr<Polygon> bot_ptr = std::make_shared<Polygon>(
            __pc_ptrs[i]->bottomFacet());
        for (auto pt : top_ptr->vertices())
            pt_sur_ptrs.push_back(std::make_shared<Point>(pt));
        for (auto pt : bot_ptr->vertices())
            pt_sur_ptrs.push_back(std::make_shared<Point>(pt));
    }

    for (auto pt_ptr : pt_sur_ptrs)
        std::cout << pt_ptr->x() << " "
                  << pt_ptr->y() << " "
                  << pt_ptr->z() << std::endl;
}

Mesh MeshGenerator::getMesh()
{
    return mesh;
}
