#include <iostream>
#include <chrono>
#include <adept_arrays.h>
// Adept vectors know their own length, so lengths do not need to be
// passed in as well

adept::aVector algorithm2(const adept::aVector& x) {

    adept::aVector y(5);

    // Nested loops to enforce O(n^4) complexity
    for (size_t j = 0; j < 5; ++j) {
        adept::adouble sum = 0.0;
        adept::adouble a = x[2 * j];       // First value in the pair
        adept::adouble b = x[2 * j + 1];   // Second value in the pair

        // O(n^4) computation
        for (adept::adouble a_i = 0; a_i < 10; a_i += 1) {
            for (adept::adouble b_i = 0; b_i < 10; b_i += 1) {
                for (adept::adouble c_i = 0; c_i < 10; c_i += 1) {
                    for (adept::adouble d_i = 0; d_i < 10; d_i += 1) {
                        sum += adept::pow(a + b, 1.01);
                    }
                }
            }
        }

        // Store the computed value in the output vector
        y[j] = sum / 10000;  // Normalize to prevent overflow
    }

    return y;
}


void algorithm2_jacobian(
        const adept::Vector& x_val,
        adept::Vector& y_val,
        adept::Matrix& jac) {

    adept::Stack stack;
    adept::aVector x = x_val;
    stack.new_recording();
    adept::aVector y = algorithm2(x);
    stack.independent(x);
    stack.dependent(y);
    stack.jacobian(jac);
    y_val = value(y);
}

adept::Matrix finite_difference_gradient(const adept::Vector& x, double eps = 1e-5) {

    adept::Stack stack;
    adept::Matrix gradient(5, 10);

    for (size_t index = 0; index < x.size(); ++index) {
        adept::aVector x_plus = x;
        adept::aVector x_minus = x;

        x_plus[index] += eps;
        x_minus[index] -= eps;

        adept::aVector result_plus = algorithm2(x_plus);
        adept::aVector result_minus = algorithm2(x_minus);

        for  (size_t jindex = 0; jindex < result_plus.size(); ++jindex) {
            gradient[jindex][index] = (value(result_plus[jindex]) - value(result_minus[jindex])) / (2 * eps);
        }
    }

    return gradient;
}

int main() {

    adept::Vector x = {5.0, 10.0, 18.0, 25.0, 30.0, 50.0, 90.0, 100.0, 30.0, 70.0};
    adept::Vector y(5);
    adept::Matrix jac(5, 10);

    auto start_fd =  std::chrono::high_resolution_clock::now();
    algorithm2_jacobian(x, y, jac);
    auto end_fd =  std::chrono::high_resolution_clock::now();

    auto duration_fd =  std::chrono::duration_cast< std::chrono::microseconds>(end_fd - start_fd);
    std::cout << "Auto Difference Time: " << duration_fd.count() << " µs" << std::endl;

    std::cout << jac << std::endl;

    adept::Matrix jac2(5, 10);
    start_fd =  std::chrono::high_resolution_clock::now();
    jac2 = finite_difference_gradient(x);
    end_fd =  std::chrono::high_resolution_clock::now();

    duration_fd =  std::chrono::duration_cast< std::chrono::microseconds>(end_fd - start_fd);
    std::cout << "Finite Difference Time: " << duration_fd.count() << " µs" << std::endl;

    std::cout << jac2 << std::endl;
}