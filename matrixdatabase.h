#ifndef MATRIXDATABASE_H
#define MATRIXDATABASE_H

#include<QVector>
#include<QString>

class Matrix;

class MatrixDataBase {
private:
    QVector<QString>* names;
    QVector<Matrix*>* matrixs;
public:
    MatrixDataBase(bool);
    bool push(QString, Matrix*);
    void pop();
    Matrix* find(QString) const;
    int size() const;
    Matrix* operator[](int) const;
    Matrix* at(int) const;
    bool del(QString);
    void clear();
    bool need_to_save;
    ~MatrixDataBase();
};

#endif // MATRIXDATABASE_H
