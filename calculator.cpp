#include"calculator.h"
#include"matrix.h"
#include"matrixdatabase.h"
#include<cmath>

Calculator::Calculator() {
    Fault = false;
    expression = bak_expression = "";
    ans = nullptr;
    save_matrixs = new MatrixDataBase(true);
    temp_matrixs = new MatrixDataBase(false);
}

Calculator::~Calculator() {
    delete save_matrixs;
    delete temp_matrixs;
}

Matrix* Calculator::ANS() const {
    return ans;
}

Matrix* Calculator::calculate(QString qs) {
    Fault = false;
    expression = bak_expression = "(" + qs + ")";
    bool paren = false;
    for(int i = 0; i < bak_expression.size(); i++) {
        QChar qc = bak_expression[i];
        if(qc == '|') {
            if(!paren) {
                bak_expression.insert(i + 1,'(');
                paren = true;
            }
            else {
                bak_expression.insert(i, ')');
                paren = false;
                i++;
            }
        }
    }
    if(ans != nullptr)temp_matrixs->push("ANS", ans);
    ans = nullptr;
    num_to_mat();
    if(cal(0) && ans == nullptr) {
        QString nm = get_name(0,true);
        if(save_matrixs->find(nm) != nullptr ||
           temp_matrixs->find(nm) != nullptr)
        {
            if(save_matrixs->find(nm) == nullptr)
                ans = new Matrix(*temp_matrixs->find(nm));
            else ans = save_matrixs->find(nm);
        }
    }
    else return nullptr;
    temp_matrixs->clear();
    return ans;
}

QString Calculator::output_str() const {
    if(ans != nullptr) {
        switch(ans->OK()) {
        case CORRECT:
            return ans->output_str();
        case ADD:
            return "  An ADD went wrong\n";
        case SUB:
            return "  An SUBTRACT went wrong\n";
        case MULTI:
            return "  An MULTIPLY went wrong\n";
        case DIVIDE:
            return "  An DIVIDE went wrong\n";
        case POW:
            return "  An POW went wrong\n";
        case DET:
            return "  An DET went wrong\n";
        case SIZE:
            return "  An SIZE went wrong\n";
        }
    }
    return "  NO ANSWER!\n";
}

MatrixDataBase* Calculator::saves() {
    return save_matrixs;
}

bool Calculator::cal(int i) {
    for(int begin = i + 1; begin < find_close_paren(i); begin++) {
        if(bak_expression[begin] == '(')cal(begin);
        if(Fault)return false;
    }
    for(int begin = i + 1; begin < find_close_paren(i); begin++) {
        QChar qc = bak_expression[begin - 1], qcc = bak_expression[begin];
        if(qc == '(' || qc == '+' || qc == '-' ||
           qc == '*' || qc == '/' || qc == '^')
            if((qcc <= 'Z' && qcc >= 'A') ||
               (qcc <= 'z' && qcc >= 'a'))
                if(isFunc(begin) != NONE)cal_func(begin);
    }
    for(int begin = i + 1; begin < find_close_paren(i); begin++) {
        if(bak_expression[begin] == '\'')
            if(cal_trans(begin))begin = last_operator(begin);
        if(Fault)return false;
    }
    for(int begin = i + 1; begin < find_close_paren(i); begin++) {
        if(bak_expression[begin] == '|')cal_abs(begin);
        if(Fault)return false;
    }
    for(int begin = i + 1; begin < find_close_paren(i); begin++) {
        if(bak_expression[begin] == '^')
            if(cal_pow(begin))begin = last_operator(begin);
        if(Fault)return false;
    }
    for(int begin = i + 1; begin < find_close_paren(i); begin++) {
        if(bak_expression[begin] == '*' ||
           bak_expression[begin] == '/')
            if(cal_high(begin))begin = last_operator(begin);
        if(Fault)return false;
    }
    for(int begin = i + 1; begin < find_close_paren(i); begin++) {
        if(bak_expression[begin] == '+' ||
           bak_expression[begin] == '-')
            if(cal_low(begin))begin = last_operator(begin);
        if(Fault)return false;
    }
    bak_expression.remove(find_close_paren(i), 1);
    bak_expression.remove(i, 1);
    return true;
}

bool Calculator::cal_func(int i) {
    Matrix* new_matrix = nullptr;
    int a = isFunc(i), l;
    QString nm;
    if(a <= 2 || a == 6)l = 3;
    else if(a >= 3 && a <= 5)l = 4;
    nm = get_name(i + l, true);
    if(save_matrixs->find(nm) == nullptr &&
       temp_matrixs->find(nm) == nullptr){
        Fault = true;
        return false;
    }
    Matrix* m;
    if(save_matrixs->find(nm) == nullptr)
        m = temp_matrixs->find(nm);
    else m = save_matrixs->find(nm);
    if(!m->isDouble())return false;
    switch(isFunc(i)) {
    case SIN:
        new_matrix = new Matrix(std::sin(m->at(0,0)));
        break;
    case COS:
        new_matrix = new Matrix(std::cos(m->at(0,0)));
        break;
    case TAN:
        new_matrix = new Matrix(std::tan(m->at(0,0)));
        break;
    case LOG:
        new_matrix = new Matrix(std::log10(m->at(0,0)));
        break;
    case ASIN:
        new_matrix = new Matrix(std::asin(m->at(0,0)));
        break;
    case ACOS:
        new_matrix = new Matrix(std::acos(m->at(0,0)));
        break;
    case ATAN:
        new_matrix = new Matrix(std::atan(m->at(0,0)));
        break;
    case NONE:
        return false;
    }
    if(new_matrix->OK() != CORRECT) {
        ans = new_matrix;
        Fault = true;
        return false;
    }
    new_matrix->carve();
    temp_matrixs->push("temp_" +
                       QString::number(temp_matrixs->size()),
                       new_matrix);
    bak_expression.replace(i, nm.size() + l, new_matrix->name());
    return true;
}

bool Calculator::cal_abs(int i) {
    if(bak_expression[i] != '|')return false;
    QString nm = get_name(i,true);
    if(save_matrixs->find(nm) == nullptr &&
       temp_matrixs->find(nm) == nullptr){
        Fault = true;
        return false;
    }
    Matrix* m;
    if(save_matrixs->find(nm) == nullptr)
        m = temp_matrixs->find(nm);
    else m = save_matrixs->find(nm);
    Matrix* new_matrix = new Matrix(m->det());
    if(new_matrix->OK() != CORRECT) {
        ans = new_matrix;
        Fault = true;
        return false;
    }
    temp_matrixs->push("temp_" +
                       QString::number(temp_matrixs->size()),
                       new_matrix);
    bak_expression.replace(i, nm.size() + 2, new_matrix->name());
    return true;
}

bool Calculator::cal_high(int i) {
    if(bak_expression[i] != '*' &&
       bak_expression[i] != '/')
        return false;
    QString nm1 = get_name(i,false),
            nm2 = get_name(i,true);
    if(save_matrixs->find(nm1) == nullptr &&
       temp_matrixs->find(nm1) == nullptr) {
        Fault = true;
        return false;
    }
    if(save_matrixs->find(nm2) == nullptr &&
       temp_matrixs->find(nm2) == nullptr) {
        Fault = true;
        return false;
    }
    Matrix *m1, *m2;
    if(save_matrixs->find(nm1) == nullptr)
        m1 = temp_matrixs->find(nm1);
    else m1 = save_matrixs->find(nm1);
    if(save_matrixs->find(nm2) == nullptr)
        m2 = temp_matrixs->find(nm2);
    else m2 = save_matrixs->find(nm2);
    Matrix* new_matrix;
    if(bak_expression[i] == '*')
        new_matrix = new Matrix((*m1)*(*m2));
    else
        new_matrix = new Matrix((*m1)/(*m2));
    if(new_matrix->OK() != CORRECT) {
        ans = new_matrix;
        Fault = true;
        return false;
    }
    temp_matrixs->push("temp_" +
                       QString::number(temp_matrixs->size()),
                       new_matrix);
    bak_expression.replace(i - nm1.size(),
                           nm1.size() + nm2.size() + 1,
                           new_matrix->name());
    return true;
}

bool Calculator::cal_low(int i) {
    if(bak_expression[i] != '+' &&
       bak_expression[i] != '-')
        return false;
    QString nm1 = get_name(i,false),
            nm2 = get_name(i,true);
    if(save_matrixs->find(nm1) == nullptr &&
       temp_matrixs->find(nm1) == nullptr){
        Fault = true;
        return false;
    }
    if(save_matrixs->find(nm2) == nullptr &&
       temp_matrixs->find(nm2) == nullptr){
        Fault = true;
        return false;
    }
    Matrix *m1, *m2;
    if(save_matrixs->find(nm1) == nullptr)
        m1 = temp_matrixs->find(nm1);
    else m1 = save_matrixs->find(nm1);
    if(save_matrixs->find(nm2) == nullptr)
        m2 = temp_matrixs->find(nm2);
    else m2 = save_matrixs->find(nm2);
    Matrix* new_matrix;
    if(bak_expression[i] == '+')
        new_matrix = new Matrix((*m1)+(*m2));
    else
        new_matrix = new Matrix((*m1)-(*m2));
    if(new_matrix->OK() != CORRECT) {
        ans = new_matrix;
        Fault = true;
        return false;
    }
    temp_matrixs->push("temp_" +
                       QString::number(temp_matrixs->size()),
                       new_matrix);
    bak_expression.replace(i - nm1.size(),
                           nm1.size() + nm2.size() + 1,
                           new_matrix->name());
    return true;
}

bool Calculator::cal_pow(int i) {
    if(bak_expression[i] != '^')return false;
    QString nm1 = get_name(i,false),
            nm2 = get_name(i,true);
    if(save_matrixs->find(nm1) == nullptr &&
       temp_matrixs->find(nm1) == nullptr){
        Fault = true;
        return false;
    }
    if(save_matrixs->find(nm2) == nullptr &&
       temp_matrixs->find(nm2) == nullptr){
        Fault = true;
        return false;
    }
    Matrix *m1, *m2;
    if(save_matrixs->find(nm1) == nullptr)
        m1 = temp_matrixs->find(nm1);
    else m1 = save_matrixs->find(nm1);
    if(save_matrixs->find(nm2) == nullptr)
        m2 = temp_matrixs->find(nm2);
    else m2 = save_matrixs->find(nm2);
    Matrix* new_matrix = new Matrix((*m1)^(*m2));
    if(new_matrix->OK() != CORRECT) {
        ans = new_matrix;
        Fault = true;
        return false;
    }
    temp_matrixs->push("temp_" +
                       QString::number(temp_matrixs->size()),
                       new_matrix);
    bak_expression.replace(i - nm1.size(),
                           nm1.size() + nm2.size() + 1,
                           new_matrix->name());
    return true;
}

bool Calculator::cal_trans(int i) {
    if(bak_expression[i] != '\'')return false;
    QString nm = get_name(i,false);
    if(save_matrixs->find(nm) == nullptr &&
       temp_matrixs->find(nm) == nullptr){
        Fault = true;
        return false;
    }
    Matrix* m;
    if(save_matrixs->find(nm) == nullptr)
        m = temp_matrixs->find(nm);
    else m = save_matrixs->find(nm);
    Matrix* new_matrix = new Matrix(m->trans());
    if(new_matrix->OK() != CORRECT) {
        ans = new_matrix;
        Fault = true;
        return false;
    }
    temp_matrixs->push("temp_" +
                       QString::number(temp_matrixs->size()),
                       new_matrix);
    bak_expression.replace(i - nm.size(),
                           nm.size() + 1,
                           new_matrix->name());
    return true;
}

int Calculator::find_close_paren(int i) const {
    int count = 0;
    for(int length = bak_expression.size(); i < length; i++) {
        QChar qc = bak_expression[i];
        if(qc == '(')count++;
        else if(qc == ')')count--;
        if(count == 0)break;
    }
    return i;
}

int Calculator::last_operator(int i) const {
    for(;i >= 0; i--)if(isOperator(i))break;
    return i - 1;
}

QString Calculator::get_name(int i, bool back) const {
    if(back) {
        if(isOperator(i))i++;
        int len = 0;
        for(;i + len < bak_expression.size(); len++)
            if(isOperator(i + len))break;
        return bak_expression.mid(i,len);
    }
    else {
        if(isOperator(i))i--;
        int len = 0;
        for(;i >= 0; i--, len++)
            if(isOperator(i))break;
        return bak_expression.mid(i + 1,len);
    }
}

bool Calculator::isNumber(int i) const {
    QChar qc = bak_expression[i];
    if((qc <= '9' && qc >= '0') || qc == '.')return true;
    return false;
}

bool Calculator::isOperator(int i) const {
    QChar qc = bak_expression[i];
    if(qc == '(' || qc == ')' ||
       qc == '+' || qc == '-' ||
       qc == '*' || qc == '/' ||
       qc == '^' || qc == '\'' ||
       qc == '|')
        return true;
    return false;
}

FUNC_NUM Calculator::isFunc(int i) const {
    QString a = "";
    if(bak_expression[i]=='a')a = bak_expression.mid(i + 3, 1);
    QString nm = bak_expression.mid(i, 3) + a;
    int x = 0;
    for(int length = sizeof(FUNCTION) / sizeof (FUNCTION[0]);
        x < length; x++)
        if(nm == FUNCTION[x])break;
    return FUNC_NUM(x);
}

bool Calculator::num_to_mat() {
    for(int i = 0; i < bak_expression.size(); i++) {
        QChar qc = bak_expression[i], qcc = bak_expression[i + 1];
        if(qc == '(' || qc == '+' ||
           qc == '-' || qc == '*' ||
           qc == '/' || qc == '^')
            if(!((qcc <= 'Z' && qcc >= 'A') ||
               (qcc <= 'z' && qcc >= 'a')) &&
                qcc != '|' && qcc != '(')
            {
                int len = 0;i++;bool has_been_num = false;
                for(;i + len < bak_expression.size(); len++)
                    if(!isNumber(i + len) && has_been_num)break;
                    else if(isNumber(i + len))has_been_num = true;
                Matrix* new_matrix = new Matrix(str_to_num(i));
                temp_matrixs->push("temp_" +
                                   QString::number(temp_matrixs->size()),
                                   new_matrix);
                bak_expression.replace(i, len, new_matrix->name());
                i += new_matrix->name().size() - 1;
            }
    }
    return true;
}

double Calculator::str_to_num(int i) const {
    if(bak_expression[i] == '+')return str_to_num(i + 1);
    if(bak_expression[i] == '-')return -str_to_num(i + 1);
    int len = 0;
    for(;i + len < bak_expression.size(); len++)
        if(!isNumber(i + len))break;
    return bak_expression.mid(i, len).toDouble();
}
