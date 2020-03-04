#ifndef CALCULATOR_H
#define CALCULATOR_H

#include<QString>
#include"const.h"

class Matrix;
class MatrixDataBase;

class Calculator {
private:
    bool Fault;
    QString expression;
    QString bak_expression;
    MatrixDataBase* save_matrixs;
    MatrixDataBase* temp_matrixs;
    Matrix* ans;
    bool isNumber(int) const;
    bool isOperator(int) const;
    FUNC_NUM isFunc(int) const;
    double str_to_num(int i) const;
    bool num_to_mat();
    int find_close_paren(int) const;
    int last_operator(int) const;
    QString get_name(int, bool) const;
    bool cal_func(int);
    bool cal_trans(int);// '
    bool cal_pow(int);// ^
    bool cal_abs(int);// ||
    bool cal_high(int);// * /
    bool cal_low(int);// + -
    bool cal(int);// ()
public:
    Calculator();
    Matrix* calculate(QString);
    Matrix* ANS() const;
    MatrixDataBase* saves();
    QString output_str() const;
    ~Calculator();
};

#endif // CALCULATOR_H
