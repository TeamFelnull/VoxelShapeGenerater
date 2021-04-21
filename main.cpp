#include <iostream>
#include <vector>
#include <fstream>
#include "json.hpp"
#include "shape.h"
#include  <ctime>

namespace nl = nlohmann;
const std::string version("1.0");
const bool debug(false);

box createBox(nl::json);

int main() {

    std::cout << "VoxelShapeGenerater V" + version << std::endl;
    std::vector<std::string> paths;
    if (!debug) {
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
    } else {
     //   paths.emplace_back("../test.json");
     //   paths.emplace_back("../test2.json");
        paths.emplace_back("../test3.json");
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

    std::vector<box> outboxs;

    for (int i = 0; i < boxs.size(); ++i) {
        bool nflag = false;
        for (int k = 0; k < boxs.size(); ++k) {
            if (i != k) {
                vec3 st = boxs[i].start;
                vec3 en = boxs[i].end;
                double sx = st.x;
                double sy = st.y;
                double sz = st.z;
                double ex = en.x;
                double ey = en.y;
                double ez = en.z;

                vec3 kst = boxs[k].start;
                vec3 ken = boxs[k].end;
                double ksx = kst.x;
                double ksy = kst.y;
                double ksz = kst.z;
                double kex = ken.x;
                double key = ken.y;
                double kez = ken.z;

                if (sx >= ksx && ex <= kex && sy >= ksy && ey <= key && sz >= ksz && ez <= kez) {
                    nflag = true;
                    break;
                }
            }
        }
        if (!nflag)
            outboxs.push_back(boxs[i]);
    }

    for (const auto &item : outboxs) {
        jo["shapes"].push_back({item.start.x, item.start.y, item.start.z, item.end.x, item.end.y, item.end.z});
    }

    std::string op = "./output_shapes.json";
    if (debug) {
        op = "." + op;
    }
    std::ofstream o(op);
    o << jo << std::endl;

    return 0;
}

box createBox(nl::json json) {
    return {json["from"][0], json["from"][1], json["from"][2], json["to"][0], json["to"][1], json["to"][2]};
}
