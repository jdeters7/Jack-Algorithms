/* 
 * John Deters
 * Sparse Matrix.h
 */

#include <map>
#include <initializer_list>
#include <sstream>

#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

struct SparseMatrix{
    
private:
    std::map <std::pair<long, long>, long> pos_;
    long dim_x_, dim_y_;
public:   
    SparseMatrix(long x, long y){dim_x_ = x; dim_y_ = y;};
    SparseMatrix(long, long, std::initializer_list<long>);
    void set (long, long, long);
    std::pair<long, long> dimensions();
    long element_count();
    long operator()(long, long);
    friend SparseMatrix operator+(SparseMatrix, long);
    friend SparseMatrix operator+(long, SparseMatrix);
    friend SparseMatrix operator+(SparseMatrix, SparseMatrix);
    friend std::ostream &operator<<(std::ostream &, SparseMatrix);
    friend SparseMatrix operator*(SparseMatrix, long);
    friend SparseMatrix operator*(long, SparseMatrix);
    friend SparseMatrix operator*(SparseMatrix, SparseMatrix);
};
std::ostream &operator<<(std::ostream &, SparseMatrix);

#endif    /* SPARSE_MATRIX_H */
