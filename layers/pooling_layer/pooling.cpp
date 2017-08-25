#include "pooling.h"

MatrixXd add_pool_padding(const MatrixXd &box, const int im_height, const int im_width, const int pp1, const int pp2) {
    const int ppa1 = im_width;
    const int ppa2 = im_height+(2*pp1);
    MatrixXd padding1 = MatrixXd::Zero(ppa1,pp1);
    MatrixXd padding2 = MatrixXd::Zero(pp2,ppa2); 

    MatrixXd temp(box.rows(), box.cols()+padding1.cols()+padding1.cols());
    temp << padding1, box, padding1;
    
    MatrixXd padded_box(temp.rows()+padding2.rows()+padding2.rows(), temp.cols());  
    padded_box << padding2, temp, padding2;

    return padded_box;
}

MatrixXd pool(const MatrixXd &convolved, const int f, const int s, const int im_width, const int im_height, const int pp1, const int pp2, std::string mode) {
    const int w = ((im_width - f + 2 * pp1) / s) + 1;
    const int h = ((im_height - f + 2 * pp2) / s) + 1;

    MatrixXd pooled(convolved.rows(), w*h);
    for (int i=0; i < convolved.rows(); i++) {
        // Take filter from stack of filters.
        MatrixXd slice = convolved.row(i);

        // Shape into image.
        Map<MatrixXd> box(slice.data(), im_width, im_height);

        // Pad box with 0s in case of stride = 3.
        MatrixXd padded_box = add_pool_padding(box, im_height, im_width, pp1, pp2);

        // for each block from image:
        MatrixXd pooling(w, h);
        for (int m=0; m < w; m++) {
            for (int n=0; n < h; n++) {
                MatrixXd smallblock = padded_box.block(0+(m*s),0+(n*s),f,f);

                if (mode == "max") {
                    pooling(m,n) = smallblock.maxCoeff();
                }

                if (mode == "ave") {
                    pooling(m,n) = smallblock.mean();
                }
            }
        }

        // Flatten pooling:
        Map<VectorXd> pcollapsed(pooling.data(),pooling.size());
        // Add back to rows of filters.
        pooled.row(i) = pcollapsed;
    }
    
    return pooled;
}
