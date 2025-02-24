#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <limits>  // For min/max values
#include <cstdlib>

struct Point {
    double x, y;
    std::string cellType;
};

std::vector<std::string> generateColors(int numClusters) { 
    std::vector<std::string> colors;
    for (int i = 0; i < numClusters; ++i) { 
        int r = (rand() % 256);
        int g = (rand() % 256);
        int b = (rand() % 256);
        colors.push_back("rgb(" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + ")");
    }
    return colors;
}

double safe_stod(const std::string& str) {
    try {
        return std::stod(str);
    }
    catch (const std::exception& e) {
        std::cerr << "Error converting to double: " << str << " - " << e.what() << std::endl;
        return 0.0;
    }
}

int main() {
    std::ifstream file("GSE118614_Smart_cells.tsv");
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::string line, header;
    std::getline(file, header);

    std::map<std::string, std::string> clusterColors;
    std::vector<Point> points;

    double min_x = std::numeric_limits<double>::max();
    double max_x = std::numeric_limits<double>::lowest();
    double min_y = std::numeric_limits<double>::max();
    double max_y = std::numeric_limits<double>::lowest();

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string firstColumn, cellName, age, plate, numGenes, totalMRNAs, tSNE1, tSNE2, rawCluster, cellType;

        std::getline(ss, firstColumn, '\t');
        std::getline(ss, cellName, '\t');
        std::getline(ss, age, '\t');
        std::getline(ss, plate, '\t');
        std::getline(ss, numGenes, '\t');
        std::getline(ss, totalMRNAs, '\t');
        std::getline(ss, tSNE1, '\t');
        std::getline(ss, tSNE2, '\t');
        std::getline(ss, rawCluster, '\t');
        std::getline(ss, cellType, '\t');

        if (tSNE1.empty() || tSNE2.empty()) {
            std::cerr << "Skipping invalid row: " << line << std::endl;
            continue;
        }

        double x = safe_stod(tSNE1);
        double y = safe_stod(tSNE2);

        if (x < min_x) min_x = x;
        if (x > max_x) max_x = x;
        if (y < min_y) min_y = y;
        if (y > max_y) max_y = y;

        if (clusterColors.find(cellType) == clusterColors.end()) {
            clusterColors[cellType] = "";
        }

        points.push_back({ x, y, cellType });
    }
    file.close();

    std::vector<std::string> colors = generateColors(clusterColors.size());
    int index = 0;
    for (auto& pair : clusterColors) {
        pair.second = colors[index++];
    }

    std::ofstream svg("plot.svg");
    svg << "<svg width='800' height='600' xmlns='http://www.w3.org/2000/svg'>\n";
    svg << "<line x1='10' y1='600' x2='900' y2='600' stroke='black' stroke-width='2' />\n"; // X-axis
    svg << "<line x1='10' y1='10' x2='10' y2='900' stroke='black' stroke-width='2' />\n"; // Y-axis

    for (const auto& point : points) {
        double norm_x = ((point.x - min_x) / (max_x - min_x)) * 700 + 50;
        double norm_y = ((point.y - min_y) / (max_y - min_y)) * 500 + 50;

        svg << "<circle cx='" << norm_x << "' cy='" << (600 - norm_y)
            << "' r='5' fill='" << clusterColors[point.cellType]
            << "' stroke='black' stroke-width='1' id='circle'><title>("<< norm_x<<", "<< (600 - norm_y)<<")</title></circle>"
            << "<style>#circle:hover{fill:green; cursor:pointer;}</style>\n";
    }

    int legend_y = 50;
    for (const auto& pair : clusterColors) {
        svg << "<rect x='650' y='" << legend_y << "' width='20' height='20' fill='" << pair.second << "' stroke='black'/>\n";
        svg << "<text x='675' y='" << (legend_y + 15) << "' font-size='12' fill='black'>" << pair.first << "</text>\n";
        legend_y += 25;
    }

    svg << "</svg>\n";
    svg.close();

    std::cout << "Plot saved as plot.svg" << std::endl;
    return 0;
}
