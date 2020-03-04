#include"matrixdatabase.h"
#include"matrix.h"
#include"const.h"
#include<QFile>

MatrixDataBase::MatrixDataBase(bool b): need_to_save(b) {
    matrixs = new QVector<Matrix*>(0);
    names = new QVector<QString>(0);
}

bool MatrixDataBase::push(QString name, Matrix* mat) {
    for(int i = 0, length = names->size(); i < length; i++)
        if(name == names->at(i))return false;
    names->push_back(name);
    Matrix* m = mat;
    matrixs->push_back(m);
    mat->change_name(name);
    return true;
}

void MatrixDataBase::pop() {
    if(size() > 1) {
        names->pop_back();
        delete matrixs->at(matrixs->size() - 1);
        matrixs->pop_back();
    }
}

Matrix* MatrixDataBase::find(QString name) const {
    for(int i = 0, length = names->size(); i < length; i++)
        if(name == names->at(i))return matrixs->at(i);
    return nullptr;
}

MatrixDataBase::~MatrixDataBase() {
    if(need_to_save) {
        for(int i = 0, length = size(); i < length; i++)
            if(matrixs->at(i))matrixs->at(i)->save();
    }
    for(int i = 0, length = matrixs->size(); i < length; i++)
        if(matrixs->at(i)->name() != "")delete matrixs->at(i);
    delete matrixs;
    delete names;
}

int MatrixDataBase::size() const {
    return names->size();
}

Matrix* MatrixDataBase::at(int i) const {
    if(i >= matrixs->size())return nullptr;
    return matrixs->at(i);
}

Matrix* MatrixDataBase::operator[](int i) const {
    if(i >= matrixs->size())return nullptr;
    return matrixs->at(i);
}

void MatrixDataBase::clear() {
    for(int i = 0, length = matrixs->size(); i < length; i++)
        if(matrixs->at(i)->name() != "")
            delete matrixs->at(i);
    matrixs->clear();
    names->clear();
}

bool MatrixDataBase::del(QString qs) {
    for(int i = 0, length = names->size(); i < length; i++)
        if(qs == names->at(i)) {
            if(matrixs->at(i)->file_name() != "")
                QFile::remove(FILE_DIR + "//" +
                              matrixs->at(i)->file_name());
            if(matrixs->at(i))delete matrixs->at(i);
            matrixs->erase(matrixs->begin() + i);
            names->erase(names->begin() + i);
            return true;
        }
    return false;
}
