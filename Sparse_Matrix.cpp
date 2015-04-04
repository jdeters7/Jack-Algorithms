/* 
 * John Deters
 * Sparse Matrix
 */

#include "sparse-matrix.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <algorithm>

// update map
void update(std::map<std::pair<long, long>, long> &mat, long x, long y, long val){
    // make pair
    std::pair<long, long> p = std::make_pair(x, y);
    // assign value
    mat[p] += val;
}


// 3 arg constructor
SparseMatrix::SparseMatrix(long x, long y, std::initializer_list<long> vals){
    // default values
    dim_x_ = x;
    dim_y_ = y;
    
    // good initializer list
    if (vals.size()%3 == 0){
        long row, column, val;
        for (auto itr = vals.begin(); itr != vals.end(); itr+=3){
            row = *itr;
            column = *(itr+1);
            val = *(itr+2);
            
            // don't add zero
            if (val != 0)
                update(pos_, row, column, val);
        }
    
    }
    
    else 
        throw std::runtime_error("Bad list of values");
    
}

// set from main
void SparseMatrix::set(long x, long y, long val){
    //updates
    if ((x <= dim_x_) && (y <= dim_y_) && (val != 0))
        update(pos_, x, y, val);
}

// dimensions of map
std::pair<long, long> SparseMatrix::dimensions(){
    return std::make_pair(dim_x_, dim_y_);
}

// count non-default values
long SparseMatrix::element_count(){
    return pos_.size();
}

// () operator call from main
long SparseMatrix::operator()(long x, long y){
    // make pair out of numbers and return value in map
    if (pos_.find(std::make_pair(x,y)) != pos_.end())
        return pos_[std::make_pair(x,y)];
    else
        //std::runtime_error;
        return 0;
}

// add matrix and long
SparseMatrix operator +(SparseMatrix lhs, long rhs){
    SparseMatrix temp(lhs.dimensions().first, lhs.dimensions().second);
    for (auto key : lhs.pos_){
        // add non-defaults
        update(temp.pos_, key.first.first, key.first.second, (key.second+rhs));
        if ((key.second+rhs) == 0)
            temp.pos_.erase(temp.pos_.find(key.first));
    }
    return temp;
}

// add long and matrix
SparseMatrix operator +(long lhs, SparseMatrix rhs){
    SparseMatrix temp(rhs.dimensions().first, rhs.dimensions().second);
    for (auto key : rhs.pos_){
        // add non-default values
        update(temp.pos_, key.first.first, key.first.second, (key.second+lhs));
        if ((key.second+lhs) == 0)
            temp.pos_.erase(temp.pos_.find(key.first));
    }
    return temp;
}

// add matrix and matrix
SparseMatrix operator +(SparseMatrix lhs, SparseMatrix rhs){
    
    SparseMatrix temp(rhs.dimensions().first, rhs.dimensions().second);
    if (rhs.dimensions() == lhs.dimensions()){
        for (auto key : rhs.pos_)
            // add all of first matrix to temp
            update(temp.pos_, key.first.first, key.first.second, key.second);
        for (auto key: lhs.pos_){
            // add if non-default at same position in first
            if (temp.pos_.find(key.first) != temp.pos_.end())
                update(temp.pos_, key.first.first, key.first.second, 
                        (temp.pos_[key.first]+key.second));
            else 
                // add non-defaults at different positions
                update(temp.pos_, key.first.first, key.first.second, key.second);
            
        }
    }
    else
        throw (std::runtime_error("Matrixes must be same size"));
    return temp;
}

// multiply long by a matrix
SparseMatrix operator*(long lhs, SparseMatrix rhs){
    
    // create matrix to return
    SparseMatrix temp(rhs.dimensions().first, rhs.dimensions().second);
    // dont multiply and store 0 values
    if (lhs != 0)
        for (auto key : rhs.pos_)
            update(temp.pos_, key.first.first, key.first.second, (key.second*lhs));
    return temp;
    
}

// multiply matrix by a long
SparseMatrix operator*(SparseMatrix lhs, long rhs){
    
    // create matrix to return
    SparseMatrix temp(lhs.dimensions().first, lhs.dimensions().second);
    // dont multiply and store 0 values
    if (rhs != 0)
        for (auto key : lhs.pos_)
            update(temp.pos_, key.first.first, key.first.second, (key.second*rhs));
    return temp;
    
}

// matrix by matrix multiplication
SparseMatrix operator*(SparseMatrix lhs, SparseMatrix rhs){
    
    // largest x value on left
    if (rhs.dim_x_ > lhs.dim_x_)
        std::swap(rhs, lhs);
    // create new matrix
    SparseMatrix temp (lhs.dim_x_, rhs.dim_y_);
    // matrix columns must equal opposite matrix rows
    if (lhs.dim_y_ == rhs.dim_x_){
        // for loop of each position in matrix on left
        for (long a = 0; a < lhs.dim_x_; a++)
            for (long b = 0; b < lhs.dim_y_; b++)
                // checks current left matrix position to existing right position
                for (auto key : rhs.pos_)
                    // left column must equal right row and be a non-default
                    if ((key.first.first == b) &&
                            (lhs.pos_.find(std::make_pair(a,b)) != lhs.pos_.end()))
                        //update(temp.pos_, a, key.first.second, 
                                //(lhs.pos_[std::make_pair(a,b)] * key.second));
                        temp.pos_[std::make_pair(a, key.first.second)] += (lhs.pos_[std::make_pair(a,b)] * key.second);
                
    }
    else
        throw (std::runtime_error("A matrix row number must equal other matrix column number"));

    return temp;
}

// print matrix
std::ostream& operator<< (std::ostream &out, SparseMatrix mat){
    int count = mat.pos_.size();
    // print empty matrix if no non-default values
    if (mat.pos_.empty())
        out<<"Empty Matrix";
    for (auto key : mat.pos_){
            out<<"("<<std::to_string(key.first.first)<<","<<
                    std::to_string(key.first.second)<<"):"<<
                    std::to_string(key.second);
            count--;
            
            // prevent trailing comma
            if (count != 0)
                out<<", ";
        }
    return out;
}
