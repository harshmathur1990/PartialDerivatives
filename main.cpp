//
// Created by Harsh on 20-03-2025.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <autodiff/reverse/var/var.hpp>

using namespace std;
using namespace std::chrono;

using namespace autodiff;

// Function to simulate an iterative process that converges
vector<var> iterative_function(vector<var> x) {
    vector<var> y;

    y.resize(x.size() / 2);

    for (size_t i = 0; i < y.size(); ++i) {
        var temp = 0;
        for (size_t j = 0; j < x.size(); ++j) {
            temp += x[j] * (j + 3) * (i + 5) / 150;
        }
        y[i] = temp;
    }
    return y;
}


vector<vector<var>> finite_difference_gradient(vector<var> x, var eps = 1e-5) {
    vector<vector<var>> gradient(x.size());

    for (size_t index = 0; index < x.size(); ++index) {
        vector<var> x_plus(x);
        vector<var> x_minus(x);

        x_plus[index] += eps;
        x_minus[index] -= eps;

        vector<var> result_plus = iterative_function(x_plus);
        vector<var> result_minus = iterative_function(x_minus);

        gradient[index].resize(result_plus.size());

        for  (size_t jindex = 0; jindex < result_plus.size(); ++jindex) {
            gradient[index][jindex] = (result_plus[jindex]  - result_minus[jindex]) / (2 * eps);
        }
    }

    return gradient;
}

int main()
{
    vector<var> x = {5.0, 10.0, 18.0, 25.0}; // Input array
    cout << "Input x: ";
    for (var val : x) cout << val << " ";
    cout << endl;

    // Compute function output
    vector<var> y = iterative_function(x);
    auto start_fd = high_resolution_clock::now();
    vector<vector<var>> ux = derivatives(y, wrt(x));
    auto end_fd = high_resolution_clock::now();
    auto duration_fd = duration_cast<microseconds>(end_fd - start_fd);
    cout << "Auto Gradient Time: " << duration_fd.count() << " µs" << endl;

    cout << "Output y: ";
    for (var val : y) cout << val << " ";
    cout << endl;

    start_fd = high_resolution_clock::now();
    vector<vector<var>> gradient = finite_difference_gradient(x);
    end_fd = high_resolution_clock::now();
    duration_fd = duration_cast<microseconds>(end_fd - start_fd);

    cout << "Finite Difference Time: " << duration_fd.count() << " µs" << endl;

    cout << "Finite Difference Gradient:\n";
    for  (size_t index = 0; index < gradient.size(); ++index) {
        for  (size_t jindex = 0; jindex < gradient[index].size(); ++jindex) {
            cout << gradient[index][jindex] << "\t";
        }
        cout<< endl;
    }


    return 0;
}
