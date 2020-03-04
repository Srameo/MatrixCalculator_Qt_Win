/************************************************************************
 *
 * 矩阵的基类
 *
 ************************************************************************/

#ifndef MATRIX_H
#define MATRIX_H

#include<iostream>
#include<vector>
#include<QString>
#include"const.h"

class Matrix {
private:
    //data:
    QString nme;
    QString file;
    const QString LocFile;
    std::vector< std::vector<double> > data;
    ERROR ok;
    //function:
    bool swap_row(int, int);
    bool multi_row(int, double);
    bool add_row(int, int, double);
public:
    //constuctor:
    Matrix();
    Matrix(const int&, const int&);
    Matrix(const double&);
    Matrix(const Matrix&);
    Matrix(const QString&);
    Matrix(const QString&, int);
    //function:
    const QString change_name(QString);
    ERROR set_OK();
    bool save();
    const QString output_str() const;
    void carve();
    //operator:
    bool operator==(const Matrix&) const;
    bool operator!=(const Matrix&) const;
    const Matrix operator+(const Matrix&) const;
    const Matrix operator-(const Matrix&) const;
    const Matrix operator*(const Matrix&) const;
    const Matrix operator/(const Matrix&) const;
    const Matrix operator^(const Matrix&) const;
    std::vector<double>& operator[](const int);
    const Matrix det() const;
    const Matrix inv() const;
    const Matrix trans() const;
    //data:
    ERROR OK() const;
    double at(int,int) const;
    const Matrix get_data(int, int) const;
    const QString name() const;
    const QString file_name() const;
    bool isDouble() const;
    //deconstructor:
    ~Matrix();
};

#endif // MATRIX_H
