#ifndef CLASS_CUBIC_BSPLINE_H
#define CLASS_CUBIC_BSPLINE_H

#include <iostream>
#include <vector>
#include <array>
#include <Eigen/Dense>


namespace cubic_bspline_maker
{

class CubicBSpline{

private:
    Eigen::MatrixXd control_points_;
    std::vector<double> knots_;
    const int degree_ = 3;
    Eigen::MatrixXd basis_table_;
    int num_control_points_;
    int num_knots_;

    void generate_knots(){
        num_knots_ = num_control_points_ - degree_ + 1 + 6;
        knots_.resize(num_knots_);
        std::cout << "num_knots = " << num_knots_ << std::endl;
        // print_knots();
        knots_.at(0) = knots_.at(1) = knots_.at(2) = knots_.at(3) = 0;
        knots_.at(num_knots_-1) = knots_.at(num_knots_-2) 
        = knots_.at(num_knots_-3) = knots_.at(num_knots_-4) = 1;
        double spacing = 1.0 / (num_knots_-7);
        for(int i = 4; i < num_knots_-4; i++) knots_.at(i) = knots_.at(i-1) + spacing;
        print_knots();
        basis_table_.resize(degree_+1, num_knots_-1);
        basis_table_.setZero();
    }

    void print_knots() { for (int i = 0; i < knots_.size(); i++) std::cout << "knots_[" << i << "] = " << knots_.at(i) << std::endl; }
    

public:
    CubicBSpline(const std::vector<std::array<double, 2>> &control_points)
    : num_control_points_(control_points.size())
    {
        control_points_.resize(num_control_points_, 2);
        for (int i = 0; i < control_points.size(); i++){
            control_points_.row(i) << control_points.at(i).at(0), control_points.at(i).at(1);
        }

        // // print control points
        // for (int i = 0; i < control_points.size(); i++){
        //     std::cout << "control_points[" << i << "] = (" << control_points.at(i).at(0) << ", " << control_points.at(i).at(1) << ")" << std::endl;
        // }

        generate_knots();

        auto xy = calc_xy_from_t(0.321);
    }
    
    std::array<double, 2> calc_xy_from_t(double t){
        t = std::max(0.0, std::min(1.0, t));
        basis_table_.setZero();

        // fill the 0-th degree basis function
        for (int i=0; i<num_knots_-1; i++){
            if (knots_.at(i)<=t && t<knots_.at(i+1)){
                basis_table_(0, i) = 1.0;
                break;
            }
        }
        // fill the higher dergees
        for (int deg=1; deg<=3; deg++){
            for (int i=0; i<num_knots_-1-deg; i++){
                double demon1 = knots_.at(i+deg) - knots_.at(i);
                double demon2 = knots_.at(i+deg+1) - knots_.at(i+1);
                double term1, term2;
                if (demon1 != 0) term1 = (t - knots_.at(i)) / demon1 * basis_table_(deg-1, i);
                else term1 = 0;
                if (demon2 != 0) term2 = (knots_.at(i+deg+1) - t) / demon2 * basis_table_(deg-1, i+1);
                else term2 = 0;
                basis_table_(deg,i) = term1 + term2;
            }
        }

        auto b_final = basis_table_.row(3).head(num_control_points_);
        auto xy = b_final * control_points_;
        return {xy(0), xy(1)};
    }

};

} // namespace cubic_bspline_maker




#endif // CLASS_CUBIC_BSPLINE_H