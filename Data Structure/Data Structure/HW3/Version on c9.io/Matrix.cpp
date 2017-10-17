#include "Matrix.h"
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
#include <math.h>  


//Default Construstor
Matrix::Matrix()
{
    rows = 0;
    columns = 0;
    fill = 0;
    data = NULL;
}

//Constructor with input arguments: rows, columns, fill value.
Matrix::Matrix(unsigned int arows, unsigned int acolumns, double afill)
{
    if(arows == 0 || acolumns == 0)
    {
        rows = 0;
        columns = 0;
        data = NULL;
    }
    else
    {
        rows = arows;
        columns = acolumns;
        fill = afill;
        data = new double*[rows];
        for (unsigned int i=0; i < rows; i++)
        {
            data[i] = new double[columns];
            for(unsigned int j=0; j < columns; j++)
            {
                data[i][j] = fill;
            }
        }
    }
}

//Constructor to copy value from another
Matrix::Matrix(const Matrix& m)
{
    rows = m.rows;
    columns = m.columns;
    data = new double*[rows];
    for(unsigned int i = 0; i<rows; i++)
    {
        data[i] = new double[columns];
        for(unsigned int j = 0; j<columns;j++)
        {
            data[i][j] = m.data[i][j];
        }
    }
}


//Destructor helper function
void Matrix::clear()
{
    for(unsigned int i = 0; i< rows; i++)
    {
        delete [] data[i];
    }
    delete []data;
    data = NULL;
    rows = 0;
    columns = 0;
}

//Destructor
Matrix::~Matrix()
{
    this->clear();
    
}


//Accessor get the value of position (arow, acolumn)
//Return true, if we get the value and store in aresult, return false, if the position exceeds the bound.
bool Matrix::get(int arow, int acolumn, double &aresult) const
{
    if( (arow < rows)&&(acolumn < columns))
    {
        aresult = data[arow][acolumn];
        
        return true;
    }
    else
    return false;
}

//Return the columns
unsigned int Matrix::num_cols() const
{
    return columns;
}

//Return the rows
unsigned int Matrix::num_rows() const
{
    return rows;
}

//Get the one row of values and return a double array with them.
//Input argumen: number of row you want to get.
//Output: a array of doubles.
double* Matrix::get_row(unsigned int r) const{
    double* temp = NULL;
    if(r >= rows)
    {
        return temp;
    }
    else
    {
        temp = new double[columns];
        for(unsigned int i = 0; i < columns; i++)
        {
            temp[i] = data[r][i];
        }
        return temp;
    }
}


//Get the one column of values and return a double array with them.
//Input argumen: number of column you want to get.
//Output: a array of doubles.
double* Matrix::get_col(unsigned int c) const{
    double* temp = NULL;
    if(c >= columns)
    {
        return temp;
    }
    else
    {
        temp = new double[rows];
        for(unsigned int i = 0; i < rows; i++)
        {
            temp[i] = data[i][c];
        }
        return temp;
    }
}


//Modifier

//Set function 
//Input: position to be modified, and the number to be filled.
//Output: return true, if we successfully set the number otherwise false.
bool Matrix::set(int arow, int acolumn, double anumber)
{
    if( (arow < rows)&&(acolumn < columns))
    {
        data[arow][acolumn] = anumber;
        return true;
    }
    else
    return false;
}

//Multiply every value by the input number
void Matrix::multiply_by_coefficient(double input)
{
    for (unsigned int i=0; i < rows; i++)
    {
        for(unsigned int j=0; j < columns; j++)
        {
            data[i][j] = input * data[i][j];
        }
    }
}

//Swap the target rows.
void Matrix::swap_row(unsigned int s, unsigned int t)
{
    double *temp = new double[columns];
    for(unsigned int i = 0; i < columns; i++)
    {
        temp[i] = data[t][i];
        data[t][i] = data[s][i];
        data[s][i] = temp[i];
    }
    delete []temp;
    
}

//Transpose the whole matrix.
void Matrix::transpose()
{
    double **temp;
    unsigned int temp1;
    temp1 = rows;
    rows = columns;
    columns = temp1;
    temp = new double*[rows];
    for (unsigned int i=0; i < rows; i++)
    {
        temp[i] = new double[columns];
        for(unsigned int j=0; j < columns; j++)
        {
            temp[i][j] = data[j][i];
        }
    }
    for(unsigned int k = 0; k< columns; k++)
    {
        delete [] data[k];
    }
    delete []data;
    data = temp;
}

//Add two matrix.
bool Matrix::add(const Matrix &b)
{
    Matrix a;
    if(rows == b.rows && columns == b.columns)
    {
        for(unsigned int i = 0; i < rows; i ++)
        {
            for(unsigned int j = 0; j < columns; j++)
            {
                data[i][j] += b.data[i][j];
            }
        }
        
        return true;
    }
    
    else
        return false;
}

//Substract two Matrix.
bool Matrix::subtract(const Matrix &b)
{
    Matrix a;
    
    if(rows == b.rows && columns == b.columns)
    {
        for(unsigned int i = 0; i < rows; i ++)
        {
            for(unsigned int j = 0; j < columns; j++)
            {
                data[i][j] -= b.data[i][j];
            }
        }
        
        return true;
    }
    
    else
        return false;
}

//Operator == to compare two matrices if there are equal.
bool Matrix::operator== (const Matrix &B) const
{
    if(rows == B.rows && columns == B.columns )
    {
        for(unsigned int i = 0; i < rows ; i++)
        {
            for(unsigned int j = 0; j < rows; j++)
            {
                if(data[i][j] != B.data[i][j])
                {
                    return false;
                }
            }
        }
        return true;
    }
    
    else
        return false;
}


//Operator != to compare two matrices if there are not equal.
bool Matrix::operator!= (const Matrix &B) const
{
    return !(*this == B);
}

//The operator = to copy matrices
Matrix& Matrix::operator=(const Matrix& m)
{
    this->clear();
    this->rows = m.rows;
    this->columns = m.columns;
    this->data = new double*[this->rows];
    for(unsigned int i = 0; i<this->rows; i++)
    {
        this->data[i] = new double[this->columns];
        for(unsigned int j = 0; j<this->columns;j++)
        {
            this->data[i][j] = m.data[i][j];
        }
    }
    return *this;
    
}

//Quarter function return a array with 4 matrix in particular styles.
Matrix* Matrix::quarter() const
{
    Matrix* temp = new Matrix[4];
    int L = ceil(float(columns)/2);
    int R = columns/2;
    int U = ceil(float(rows)/2);
    int B = rows/2;

    
   temp[0] = Matrix(L,U,1.0);
    // UL
    for(unsigned int i = 0; i< U; i++)
    {
        for(unsigned int j = 0; j < L; j++)
        {
            
            temp[0].set(i,j,data[i][j]);
               
        }
    }
    
    
    temp[1] = Matrix(L,U,1.0);
    //UR
    for(unsigned int k = 0; k< U; k++)
    {
        for(unsigned int l = R; l < columns; l++)
        {
            temp[1].set(k,l-R,data[k][l]);
        }
    }
        
    
    temp[2] = Matrix(L,U,1.0);
    //LL
    for(unsigned int i = B; i< rows ; i++)
    {
        for(unsigned int j = 0; j < L; j++)
        {
            temp[2].set(i-B,j,data[i][j]);
        }
    }

    temp[3] = Matrix(L,U,1.0);
    //LR
    for(unsigned int i = B; i< rows; i++)
    {
        for(unsigned int j = R; j < columns; j++)
        {
            temp[3].set(i-B,j-R,data[i][j]);
        }
    }
    
    return temp;
}

//Operator << to printout in right format.
std::ostream& operator<< (std::ostream& out, const Matrix& m) 
{
    out << m.num_rows() << " x " << m.num_cols() <<" matrix: " << std::endl;
    out << "[";
    for(unsigned int i = 0; i < m.num_rows(); i++)
    {
        for(unsigned int j = 0; j< m.num_cols(); j++)
        {
            double temp = 0;
            m.get(i,j,temp);
            out <<" " << temp; 
        }
        if(i < m.num_rows()-1)
            out << std::endl <<" ";
    }
    out << " ]";
    return out;
    
}

//Resize() implementation for extra credits.
//This function takes in three arguments: target rows, target columns and auto fill value.
//This function will modify the old data and old row, columns
void Matrix::resize(unsigned int rrow, unsigned int rcol, double rfill)
{
    double **temp;
    if(rrow == 0 || rcol == 0)
    {
        rows = 0;
        columns = 0;
        data = NULL;
    }
    
    else
        temp = new double*[rrow];
        for (unsigned int i=0; i < rrow; i++)
        {
            temp[i] = new double[rcol];
            for(unsigned int j=0; j < rcol; j++)
            {
                if(i >=rows || j >= columns)
                {
                    temp[i][j] = rfill;
                }
                else
                    temp[i][j] = data[i][j];
            }
        }
        for(unsigned int k = 0; k< columns; k++)
        {
            delete [] data[k];
        }
        delete []data;
        data = temp;
        rows = rrow;
        columns = rcol;

}