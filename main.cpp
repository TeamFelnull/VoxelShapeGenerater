#include <iostream>
#include <vector>
#include <fstream>
#include "json.hpp"
#include "shape.h"
#include  <ctime>

namespace nl = nlohmann;
const std::string version("1.0");

box createBox(nl::json);

int main() {
    std::cout << "VoxelShapeGenerater V" + version << std::endl;
    std::vector<std::string> paths;
    int mc = 1;
    while (true) {
        std::string path;
        std::cout << "ModelPath(" << mc << ") = ";
        std::cin >> path;
        std::ifstream ps(path);
        ++mc;
        if (!ps.is_open()) {
            ps.close();
            break;
        }
        paths.push_back(path);
        ps.close();
    }
    std::vector<box> boxs;
    for (const auto &item : paths) {
        std::ifstream js(item);
        if (js.good()) {
            nl::json ji;
            js >> ji;
            for (const auto &elem : ji["elements"]) {
                boxs.push_back(createBox(elem));
            }
        }
    }

    nl::json jo;
    jo["version"] = 1;
    jo["date"] = time(nullptr);
    jo["meta"] = "VoxelShapeGenerater V" + version;

    for (const auto &item : paths) {
        jo["paths"].push_back(item);
    }

    for (const auto &item : boxs) {
        jo["shapes"].push_back({item.start.x, item.start.y, item.start.z, item.end.x, item.end.y, item.end.z});
    }

    std::cout << "1" << std::endl;

    std::ofstream o("./output_shapes.json");
    o << jo << std::endl;

    return 0;
}

box createBox(nl::json json) {
    return {json["from"][0], json["from"][1], json["from"][2], json["to"][0], json["to"][1], json["to"][2]};
}
