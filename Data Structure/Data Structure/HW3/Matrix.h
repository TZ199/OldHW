#include <fstream>
#include <string>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iomanip> 
#include <algorithm>

class Matrix{
    private:
        double **data;
        
        unsigned int rows;
        unsigned int columns;
        double fill;
    
    
    
    
    public:
        //Construstor.
        Matrix();
        Matrix(unsigned int arows, unsigned int acolumns, double afill);
        Matrix(const Matrix& m);
        
        //Destructor
        void clear();
        ~Matrix();
        
        //Accessor
        bool get(int arow, int acolumn, double &aresult) const;
        unsigned int num_cols() const;
        unsigned int num_rows() const;
        double* get_row(unsigned int r) const;
        double* get_col(unsigned int c) const;
        
        //Modifer
        bool set(int arow, int acolumn, double anumber);
        void multiply_by_coefficient(double input);
        void swap_row(unsigned int s, unsigned int t);
        void transpose();
        bool add(const Matrix &b);
        bool subtract(const Matrix &b);
        
        //Operators
        bool operator== ( const Matrix &B);
        bool operator!= (const Matrix &B);
        Matrix* quarter();
        Matrix& operator=(const Matrix& m); 
};

std::ostream& operator<< (std::ostream& out, const Matrix& m);
