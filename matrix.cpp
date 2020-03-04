#include"matrix.h"
#include"const.h"
#include<fstream>
#include"calculator.h"
#include<QDir>

Matrix::Matrix(): LocFile(FILE_DIR), data(0), ok(SIZE) {}

Matrix::Matrix(const int& r, const int& c):
    data(r, std::vector<double>(c, 0))
{
    if(r > 0 && c >0) {
        ok = CORRECT;
        int min = r < c ? r : c;
        for(int i = 0; i < min; i++)
            data[i][i] = 1;
    }
    else ok = SIZE;
}

Matrix::Matrix(const double &dd):
    LocFile(FILE_DIR), data(1), ok(CORRECT)
{
    data[0].push_back(dd);
}

Matrix::Matrix(const Matrix &m):
    nme("bak_" + m.nme), LocFile(FILE_DIR),
    data(m.data), ok(m.ok) {}

Matrix::Matrix(const QString& qs): LocFile(FILE_DIR) {
    Calculator cc;
    QString a = qs;
    data.push_back(std::vector<double>());
    for(int i = 0, r = 0; i < a.size();) {
        if(i > 1)
            if(qs[i - 1] == '\n') {
                data.push_back(std::vector<double>());
                r++;
            }
        int dot = a.indexOf(",", i);
        int enter = a.indexOf("\n", i);
        int min = dot < enter ? dot : enter;
        if(dot == -1 && enter != -1)min = enter;
        else if(dot != -1 && enter == -1)min = dot;
        Matrix* data_ = cc.calculate(a.mid(i, min - i));
        if(data_ == nullptr)break;
        if(data_->ok != CORRECT)break;
        if(data_->data.size() != 1 ||
           data_->data[0].size() != 1)
            break;
        data[r].push_back(data_->data[0][0]);
        if(min != -1)i = min + 1;
        else break;
    }
    set_OK();
}

Matrix::Matrix(const QString& qs, int): LocFile(FILE_DIR) {
    std::ifstream f(FILE_DIR.toStdString() + "//" + qs.toStdString());
    file = qs;
    nme = qs.mid(0, qs.indexOf('.'));
    int r = -1;
    while(!f.eof()) {
        std::string s; getline(f, s);
        QString ss; ss = QString::fromStdString(s).simplified();
        if(ss != "") {
            data.push_back(std::vector<double>());
            r++;
        }
        else continue;
        for(int i = 0; i < ss.size();) {
            int space = ss.indexOf(" ", i);
            data[r].push_back(ss.mid(i, space - i).toDouble());
            if(space != -1)i = space + 1;
            else break;
        }
    }
    set_OK();
}

bool Matrix::save() {
    QDir dir;
    if(!dir.exists(LocFile))
        dir.mkdir(LocFile);
    if(file == "")file = nme + ".txt";
    std::ofstream f((LocFile + "//" + file).toStdString());
    if(ok == CORRECT) {
        int r = data.size(), c = data[0].size();
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++) {
                f << data[i][j];
                f << ' ';
            }
            f << '\n';
        }
        return true;
    }
    return false;
}

const QString Matrix::output_str() const {
    if(ok != CORRECT)return "Matrix " + nme + " is illegal\n";
    QString ans = "  ";
    int r = data.size(), c = data[0].size();
    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
            char str[10];
            sprintf(str,"%8g",data[i][j]);
            ans += QString::fromStdString(std::string(str)) + " ";
        }
        ans += "\n  ";
    }
    return ans;
}

bool Matrix::swap_row(int r1, int r2) {
    if(ok != CORRECT)return false;
    if(r1 >= data.size() || r2 >= data.size())
        return false;
    int c = data[0].size();
    std::vector<double> temp(c);
    for(int i = 0; i < c; i++){
        temp[i] = data[r2][i];
        data[r2][i] = data[r1][i];
        data[r1][i] = temp[i];
    }
    return true;
}

bool Matrix::multi_row(int r, double num) {
    if(ok != CORRECT)return false;
    if(r >= data.size())return false;
    for(int i = 0, c = data[0].size(); i < c; i++)
        data[r][i] *= num;
    return true;
}

bool Matrix::add_row(int r1, int r2, double num) {
    if(ok != CORRECT)return false;
    if(r1 >= data.size() || r2 >= data.size())
        return false;
    for(int i = 0, c = data[0].size(); i < c; i++)
        data[r1][i] += num * data[r2][i];
    return true;
}

ERROR Matrix::set_OK() {
    int r = data.size();
    if(r == 0)return ok = SIZE;
    int c = data[0].size();
    if(c == 0)return ok = SIZE;
    for(int i = 0; i < r; i++)
        if(data[i].size() != c)return ok = SIZE;
    return ok = CORRECT;
}

ERROR Matrix::OK() const {
    return ok;
}

double Matrix::at(int rw, int clm) const {
    return data[rw][clm];
}

const Matrix Matrix::get_data(int row, int column) const {
    return Matrix(data[row][column]);
}

const QString Matrix::name() const {
    return nme;
}

const QString Matrix::file_name() const {
    return file;
}

const QString Matrix::change_name(QString new_name) {
    QString last_nme;
    last_nme = nme;
    nme = new_name;
    return last_nme;
}

bool Matrix::operator==(const Matrix& m) const {
    if(ok != CORRECT || m.ok != CORRECT)return false;
    int r = this->data.size(), mr = m.data.size();
    int c = this->data[0].size(), mc = m.data[0].size();
    if(r != mr || c != mc)return false;
    for(int i = 0; i < r; i++)
        for(int j = 0; j < c; j++)
            if(data[i][j] != m.data[i][j])
                return false;
    return true;
}

bool Matrix::operator!=(const Matrix& m) const {
    if(*this == m)return false;
    return true;
}

std::vector<double>& Matrix::operator[](const int row){
    if(row < data.size())return data[row];
    return data[0];
}

const Matrix Matrix::operator+(const Matrix& m) const {
    Matrix ans;
    ans.ok = ADD;
    if(!(ok == CORRECT && m.ok == CORRECT))return ans;
    int r = this->data.size(), mr = m.data.size();
    int c = this->data[0].size(), mc = m.data[0].size();
    if(r != mr || c != mc)return ans;
    ans.ok = CORRECT;
    for(int i = 0; i < r; i++) {
        ans.data.push_back(std::vector<double>(mc));
        for(int j = 0; j < mc; j++)
            ans[i][j] = this->data[i][j] + m.data[i][j];
    }
    ans.carve();
    return ans;
}

const Matrix Matrix::operator-(const Matrix& m) const {
    Matrix ans;
    ans.ok = SUB;
    if(!(ok == CORRECT && m.ok == CORRECT))return ans;
    int r = this->data.size(), mr = m.data.size();
    int c = this->data[0].size(), mc = m.data[0].size();
    if(r != mr || c != mc)return ans;
    ans.ok = CORRECT;
    for(int i = 0; i < r; i++) {
        ans.data.push_back(std::vector<double>(mc));
        for(int j = 0; j < mc; j++)
            ans[i][j] = this->data[i][j] - m.data[i][j];
    }
    ans.carve();
    return ans;
}

const Matrix Matrix::operator*(const Matrix& m) const {
    Matrix ans;
    ans.ok = MULTI;
    if(!(ok == CORRECT && m.ok == CORRECT))return ans;
    int r = this->data.size(), mr = m.data.size();
    int c = this->data[0].size(), mc = m.data[0].size();
    if(c == mr) {
        ans.ok = CORRECT;
        for(int i = 0; i < r; i++) {
            ans.data.push_back(std::vector<double>(mc, 0));
            for(int j = 0; j < mc; j++)
                for(int k = 0; k < c; k++)
                    ans[i][j] += this->data[i][k] * m.data[k][j];
        }
    }
    else if(c == 1 && r == 1) {
        ans.ok = CORRECT;
        for(int i = 0; i < mr; i++) {
            ans.data.push_back(std::vector<double>(mc));
            for(int j = 0; j < mc; j++)
                ans[i][j] = this->data[0][0] * m.data[i][j];
        }
    }
    else if(mc == 1 && mr == 1) {
        ans.ok = CORRECT;
        for(int i = 0; i < r; i++) {
            ans.data.push_back(std::vector<double>(c));
            for(int j = 0; j < c; j++)
                ans[i][j] = this->data[i][j] * m.data[0][0];
        }
    }
    ans.carve();
    return ans;
}

const Matrix Matrix::operator/(const Matrix& m) const {
    Matrix ans = (*this) * m.inv();
    if(ans.ok != CORRECT)ans.ok = DIVIDE;
    ans.carve();
    return ans;
}

const Matrix Matrix::operator^(const Matrix& m) const {
    Matrix ans;
    ans.ok = POW;
    if(!(ok == CORRECT && m.ok == CORRECT))return ans;
    if(m.data[0].size() != 1 || m.data.size() != 1)return ans;
    int r = data.size();
    if(r != data[0].size())return ans;
    int pow = m.data[0][0];
    if(pow < 0)return ((*this) ^ Matrix(-pow)).inv();
    if(pow == 0)return Matrix(r, r);
    if(pow == 2)return (*this) * (*this);
    if(pow % 2)return ((*this) ^ Matrix(pow - 1)) * (*this);
    return ((*this) ^ Matrix(pow / 2)) * (*this) * (*this);
}

const Matrix Matrix::det() const {
    Matrix Ans;
    Ans.ok = DET;
    if(ok != CORRECT)return Ans;
    if(data.size() != data[0].size())return Ans;
    int r = data.size();
    if(r == 1) return Matrix(
            data[0][0] > -data[0][0] ?
            data[0][0] : -data[0][0]);
    Matrix bak(*this);
    double ans = 1;
    for(int cc = 0; cc < r - 1; cc++) {
        int rr = 0;
        if(bak.data[cc][cc] == 0)
            for(rr = cc + 1; rr < r; rr++)
                if(bak.data[rr][cc] != 0)
                    bak.swap_row(rr, cc);
        if(rr == r)continue;
        for(rr = cc + 1; rr < r; rr++) {
            if(bak.data[rr][cc] == 0)continue;
            double multiple = -(bak.data[rr][cc] / bak.data[cc][cc]);
            bak.add_row(rr, cc, multiple);
        }
        ans *= bak.data[cc][cc];
    }
    ans *= bak.data[r -1][r - 1];
    return Matrix(ans);
}

const Matrix Matrix::inv() const {
    Matrix ANS;
    ANS.ok = SIZE;
    if(ok != CORRECT)return ANS;
    if(data.size() != data[0].size())return ANS;
    if(det() == 0)return ANS;
    int r = data.size();
    if(r == 1)return Matrix(1 / data[0][0]);
    Matrix ans(r, r), bak(*this);
    for(int cc = 0; cc < r - 1; cc++) {
        int rr = 0;
        if(bak.data[cc][cc] == 0)
            for(rr = cc + 1; rr < r; rr++)
                if(bak.data[rr][cc] != 0) {
                    bak.swap_row(rr, cc);
                    ans.swap_row(rr, cc);
                }
        if(rr == r)continue;
        for(rr = cc + 1; rr < r; rr++) {
            if(data[rr][cc] == 0)continue;
            double multiple = -(bak.data[rr][cc] / bak.data[cc][cc]);
            bak.add_row(rr, cc, multiple);
            ans.add_row(rr, cc, multiple);
        }
    }
    for(int cc = r - 1; cc > 0; cc--) {
        int rr = 0;
        if(bak.data[cc][cc] == 0)
            for(rr = cc - 1; rr >= 0; rr--)
                if(bak.data[rr][cc] != 0) {
                    bak.swap_row(rr, cc);
                    ans.swap_row(rr, cc);
                }
        if(rr == -1)continue;
        for(rr = cc - 1; rr >= 0; rr--) {
            if(bak.data[rr][cc] == 0)continue;
            double multiple = -(bak.data[rr][cc] / bak.data[cc][cc]);
            bak.add_row(rr, cc, multiple);
            ans.add_row(rr, cc, multiple);
        }
        ans.multi_row(cc, 1 / bak[cc][cc]);
    }
    ans.multi_row(0, 1 / bak[0][0]);
    ans.carve();
    return ans;
}

const Matrix Matrix::trans() const {
    Matrix ANS;
    ANS.ok = SIZE;
    if(ok != CORRECT)return ANS;
    int r = data.size(), c = data[0].size();
    Matrix ans;
    ans.ok = CORRECT;
    for(int j = 0; j < c; j++)
        ans.data.push_back(std::vector<double>(r));
    for(int i = 0; i < r; i++)
        for(int j = 0; j < c; j++)
            ans.data[j][i] = data[i][j];
    ans.carve();
    return ans;
}

void Matrix::carve() {
    for(int x = 0, row = data.size(); x < row; x++)
        for(int y = 0, col = data[0].size(); y < col; y++)
            if(data[x][y] < PRECISION && data[x][y] > -PRECISION)
                data[x][y] = 0;
}

bool Matrix::isDouble() const {
    int r = data.size();
    if(r != 1)return false;
    int c = data[0].size();
    if(c != 1)return false;
    return true;
}

Matrix::~Matrix() {}
