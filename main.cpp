#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <string>

#include "matplotlibcpp.h"

#include "class_cubic_bspline.h"

namespace plt = matplotlibcpp;

std::vector<std::array<double, 2>> parseFile(const std::string& filename) {
    std::vector<std::array<double, 2>> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return data;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string num1, num2;
        // Split line by comma
        if (std::getline(lineStream, num1, ',') && std::getline(lineStream, num2)) {
            // Convert strings to doubles and add to array
            std::array<double, 2> pair = {std::stod(num1), std::stod(num2)};
            data.push_back(pair);
        }
    }
    file.close();
    return data;
}



int main(){

    // Read control points from file
    std::vector<std::array<double, 2>> control_points = parseFile("raw_points/road_points_4.txt");

    // Create a cubic bspline object
    cubic_bspline_maker::CubicBSpline cubic_bspline(control_points);

    // Generate points on the curve
    std::vector<std::array<double, 2>> points;

    double dt = 0.005;
    
    for (double t = 0.0; t <= 1.0; t += dt){
        auto xy = cubic_bspline.calc_xy_from_t(t);
        points.push_back(xy);
    }

    // ----- Plotting -----

    // Plot the control points as scatter
    std::vector<double> x, y;
    for (int i = 0; i < control_points.size(); i++){
        x.push_back(control_points.at(i).at(0));
        y.push_back(control_points.at(i).at(1));
    }
    
        
    plt::grid(true);
    plt::axis("equal");
    plt::scatter(x, y, 5, {{"color", "blue"}});

    // Plot the cubic bspline curve
    std::vector<double> x_curve, y_curve;
    for (int i = 0; i < points.size(); i++){
        x_curve.push_back(points.at(i).at(0));
        y_curve.push_back(points.at(i).at(1));
    }

    plt::plot(x_curve, y_curve, "r-");

    plt::show();

    return 0;
}