#include "fully_connected.h"

MatrixXd fully_connect(MatrixXd &input, const int k_num, const MatrixXd &weights, const VectorXd &biases) {
    input.transposeInPlace();
    VectorXd input_flattened(Map<VectorXd>(input.data(), k_num * input.rows()));
    // Fully connect.
    MatrixXd fc = input_flattened.transpose() * weights.transpose();
    // Add Biases.
    fc += biases;

    return fc.transpose();
}
