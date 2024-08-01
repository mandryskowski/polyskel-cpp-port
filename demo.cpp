#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <sstream>
#include <tuple>
#include "polyskel.h" // Assuming this contains the necessary functions for skeletonization
#include "lavertex.h"


int main(int argc, char** argv) {
    // Setup logging and argument parsing
    std::string loglevel = "WARNING";
    bool verbose = false;
    std::string polygonFile;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--verbose" || arg == "--v") {
            verbose = true;
        } else if (arg == "--log") {
            if (i + 1 < argc) {
                loglevel = argv[++i];
            }
        } else {
            polygonFile = arg;
        }
    }

    std::ifstream infile(polygonFile);
    if (!infile) {
        std::cerr << "Error opening file: " << polygonFile << std::endl;
        return 1;
    }

    std::regex polygon_line_pat(R"(\s*(\d+(\.\d+)?)\s*,\s*(\d+(\.\d+)?)\s*(#.*)?)");
    std::vector<std::vector<Vec2>> contours;
    std::vector<Vec2> poly;
    std::string line;

    while (std::getline(infile, line)) {
        line = std::regex_replace(line, std::regex(R"(\s+$)"), ""); // trim right
        if (line.empty() || line[0] == '#') {
            continue;
        }

        if (line[0] == '-') {
            contours.push_back(poly);
            poly.clear();
            continue;
        }

        std::smatch match;
        if (std::regex_match(line, match, polygon_line_pat)) {
            poly.emplace_back(Vec2{std::stod(match[1]), std::stod(match[3])});
        }
    }

    if (infile.is_open()) {
        infile.close();
    }

    contours.push_back(poly);
    poly = contours[0];
    std::vector<std::vector<Vec2>> holes(contours.begin() + 1, contours.end());

    double max_x = -DBL_MAX, max_y = -DBL_MAX;
    for (const auto& point : poly) {
        if (point.x > max_x) max_x = point.x;
        if (point.y > max_y) max_y = point.y;
    }
    int bbox_end_x = static_cast<int>(max_x) + 20;
    int bbox_end_y = static_cast<int>(max_y) + 20;

    // Rendering part is omitted as requested
    // Assuming polyskel::skeletonize exists and returns skeletons
    std::vector<shared_ptr<Subtree>> skeleton = skeletonize(poly, holes);

    for (const auto& res : skeleton) {
        std::cout << res << std::endl;
    }

    // Rendering the skeletons
    for (const auto& arc : skeleton) {
        cout << *arc << endl;
    }

    return 0;
}
