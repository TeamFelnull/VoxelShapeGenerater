#include <iostream>
#include <vector>
#include <fstream>
#include "json.hpp"
#include "shape.h"
#include  <ctime>
#include <cmath>

namespace nl = nlohmann;
namespace sh = shape;

const std::string version("1.0");
const bool debug(true);
const double oneradian(M_PI / 180);
const double onepixel(1);

sh::box createBox(nl::json);

sh::anglebox createBoxAngled(nl::json);

std::vector<sh::box> getBoxsByAngled(sh::anglebox anglebox);

void printProgress(int cont, int max, const std::string &log);

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
        //  paths.emplace_back("../test.json");
        //  paths.emplace_back("../test2.json");
        //  paths.emplace_back("../test3.json");
        paths.emplace_back("../test4.json");
        //  paths.emplace_back("../testmodel.json");
    }

    std::vector<sh::box> boxs;
    std::vector<sh::anglebox> angleboxs;

    for (const auto &item : paths) {
        std::ifstream js(item);
        if (js.good()) {
            nl::json ji;
            js >> ji;
            int cont = 0;
            for (const auto &elem : ji["elements"]) {
                cont++;
                printProgress(cont, ji["elements"].size(), "Model Extracting: " + item);

                nl::json el = elem;
                bool jr = el["rotation"].is_null();
                if (!jr) {
                    angleboxs.push_back(createBoxAngled(elem));
                } else {
                    boxs.push_back(createBox(elem));
                }
            }
            std::cout << std::endl;
            if (!angleboxs.empty()) {
                cont = 0;
                for (const auto &ag : angleboxs) {
                    cont++;
                    printProgress(cont, angleboxs.size(), "Model Angle Calculating: " + item);
                    for (const auto &abox : getBoxsByAngled(ag)) {
                        boxs.push_back(abox);
                    }
                }
                std::cout << std::endl;
            }

        }
    }
    std::vector<sh::box> boxs2;
    int cont = 0;
    for (const auto &item : boxs) {
        cont++;
        printProgress(cont, boxs.size(), "Shape Checking");
        sh::vec3 st = item.start;
        sh::vec3 en = item.end;
        double sx = st.x;
        double sy = st.y;
        double sz = st.z;
        double ex = en.x;
        double ey = en.y;
        double ez = en.z;
        boxs2.emplace_back(std::min(sx, ex), std::min(sy, ey), std::min(sz, ez), std::max(sx, ex), std::max(sy, ey),
                           std::max(sz, ez));
    }
    std::cout << std::endl;


    nl::json jo;
    jo["version"] = 1;
    jo["date"] = time(nullptr);
    jo["meta"] = "VoxelShapeGenerater V" + version;

    for (const auto &item : paths) {
        jo["paths"].push_back(item);
    }

    std::vector<sh::box> outboxs;

    for (int i = 0; i < boxs2.size(); ++i) {
        bool nflag = false;
        for (int k = 0; k < boxs2.size(); ++k) {
            if (i != k) {
                sh::vec3 st = boxs2[i].start;
                sh::vec3 en = boxs2[i].end;
                double sx = st.x;
                double sy = st.y;
                double sz = st.z;
                double ex = en.x;
                double ey = en.y;
                double ez = en.z;

                sh::vec3 kst = boxs2[k].start;
                sh::vec3 ken = boxs2[k].end;
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
            outboxs.push_back(boxs2[i]);
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

    std::cout << "Voxel shape generate Succses full" << std::endl;

    if (!debug) {
        system("pause");
    }
    return 0;
}

sh::box createBox(nl::json json) {
    return {json["from"][0], json["from"][1], json["from"][2], json["to"][0], json["to"][1], json["to"][2]};
}

sh::anglebox createBoxAngled(nl::json json) {
    sh::axis ax = sh::X;
    if (json["rotation"]["axis"] == "y") {
        ax = sh::Y;
    } else if (json["rotation"]["axis"] == "z") {
        ax = sh::Z;
    }
    return {sh::vec3(json["from"][0], json["from"][1], json["from"][2]),
            sh::vec3(json["to"][0], json["to"][1], json["to"][2]),
            sh::rotation(json["rotation"]["angle"], ax,
                         sh::vec3(json["rotation"]["origin"][0], json["rotation"]["origin"][1],
                                  json["rotation"]["origin"][2]))};

}

std::vector<sh::box> getBoxsByAngled(sh::anglebox anglebox) {
    std::vector<sh::box> boxs;
    double w = anglebox.end.x - anglebox.start.x;
    double h = anglebox.end.y - anglebox.start.y;
    double d = anglebox.end.z - anglebox.start.z;

    double yang = 90 - anglebox.arotation.angle;
    double mw = w * cos(yang * oneradian);
    double mh = w * sin(yang * oneradian);
    double mt = h * cos(yang * oneradian);

    for (int i = 0; i < 16; ++i) {
        double yt = yang * oneradian;
        double aw = i / tan(yt);
        boxs.emplace_back(-mw + aw, -mh + i, anglebox.end.z, -mw + mt, -mh + i + 1, anglebox.start.z);
    }


    boxs.emplace_back(0, 0, 0, 0, 0, 0);

    std::cout << w << ":" << h << ":" << d << std::endl;

    return boxs;
}

void printProgress(int cont, int max, const std::string &log) {
    int progress = (int) (((double) cont / (double) max) * 100);
    std::cout << "\r" << "[" << progress << "%" << cont << "/" << max << "]" << log;
}
