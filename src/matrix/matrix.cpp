#include "matrix.h"
#include <math.h>
#include <stddef.h>

#define EPS 0.1e-15
#define Max(A, B) (((A) > (B)) ? (A) : (B))
#define Min(A, B) (((A) < (B)) ? (A) : (B))
#define Abs(A) (((A) > 0.) ? (A) : -(A))

// default constructor
matrix::matrix()
{
    array = NULL;
    sizeRow = 0;
    sizeCol = 0;
}

// copy constructor
matrix::matrix(matrix &m)
{
    array = NULL;
    sizeRow = 0;
    sizeCol = 0;

    if(m.sizeRow > 0 && m.sizeCol > 0) {
        sizeRow = m.sizeRow;
        sizeCol = m.sizeCol;
        array = new float * [sizeRow];

        for(int ic = 0; ic < sizeRow; ic++) {
            array[ic] = new float[sizeCol];
        }

        for(int i = 0; i < sizeRow; i++) {
            for(int j = 0; j < sizeCol; j++) {
                array[i][j] = m.array[i][j];
            }
        }
    }
}

// конструктор с заданием размера
matrix::matrix(int row, int col)
{
    array = NULL;
    sizeRow = 0;
    sizeCol = 0;

    if(row > 0 && col > 0) {
        sizeRow = row;
        sizeCol = col;
        array = new float*[sizeRow];

        for(int i = 0; i < sizeRow; i++) {
            array[i] = new float[sizeCol];
        }
    }
}

// деструктор
matrix::~matrix()
{
    if(array != NULL) {
        for(int i = 0; i < sizeRow; i++) {
             if(array[i] != NULL) {
                 delete [] array[i];
             }
        }
        delete [] array;
    }
}

// перегрузка оператора '='
matrix &matrix::operator = (matrix &m)
{
    if(this != &m) {
        if(sizeRow == 0 && sizeCol == 0) {
            sizeRow = m.sizeRow;
            sizeCol = m.sizeCol;
            array = new float*[sizeRow];

            for(int i = 0; i < sizeRow; i++) {
                array[i] = new float[sizeCol];
            }
        }

        if(sizeRow == m.sizeRow && sizeCol == m.sizeCol)
        {
            for(int i = 0; i < sizeRow; i++)
                for(int j = 0; j < sizeCol; j++)
                    array[i][j] = m.array[i][j];
        }
    }
    return *this;
}

// перегрузка оператора '()'
float& matrix::operator () (int row, int col)
{
    if(row > 0 && row <= sizeRow && col > 0 && col <= sizeCol) {
        return array[row-1][col-1];
    }
    return array[0][0];
}

// перегрузка оператора '[]'
float& matrix::operator [] (int k)
{
    if(sizeRow==1 && k>0 && k<=sizeCol) {
        return array[0][k-1];
    }

    if(sizeCol == 1 && k > 0 && k <= sizeRow) {
        return array[k-1][0];
    }

    return array[0][0];
}

// получение кол-ва строк
int matrix::getSizeRow()
{
    return sizeRow;
}

// получение кол-ва столбцов
int matrix::getSizeCol()
{
    return sizeCol;
}

// установка размера
void matrix::setSize(int row, int col)
{
    if(row == 0 && col == 0) {
        if(array != NULL) {
            for(int i = 0; i < sizeRow; i++) {
                 if(array[i] != NULL) {
                     delete [] array[i];
                 }
            }

            delete [] array;
            array=NULL;
        }

        sizeRow=0;
        sizeCol=0;
        return;
    }

    if(row > 0 && col > 0) {
        if(array != NULL) {
            for(int i=0; i<sizeRow; i++) {
                if(array[i]!=NULL) {
                    delete [] array[i];
                }
            }
            delete [] array;
        }

        sizeRow = row;
        sizeCol = col;
        array = new float*[sizeRow];

        for(int i=0; i<sizeRow; i++) {
            array[i] = new float[sizeCol];
        }
    }
}

// сделать единичной
matrix & matrix::setSingle(int n)
{
    if(n > 0) {
        setSize(n, n);
        init(0);

        for(int i = 0; i < n; i++) {
            array[i][i] = 1;
        }
    }
    return *this;
}

// инициализация числом
void matrix::init(float c)
{
    for(int i = 0; i < sizeRow; i++) {
        for(int j = 0; j < sizeCol; j++) {
            array[i][j] = c;
        }
    }
}

// инициализ. статич. массивом
void matrix::initStat(float *p, int size_r, int size_c)
{
    this->setSize(size_r,size_c);
    for(int i = 0; i < sizeRow; i++) {
        for(int j = 0; j < sizeCol; j++) {
            array[i][j] = p[i*size_c+j];
        }
    }
}

// инициализ. динамич. массивом
void matrix::initDynam(float **p, int size_r, int size_c)
{
    this->setSize(size_r,size_c);
    for(int i = 0; i < sizeRow; i++) {
        for(int j = 0; j < sizeCol; j++) {
            array[i][j] = p[i][j];
        }
    }
}

void matrix::initMatrix(matrix &m, int size_r, int size_c)
{
    this->setSize(size_r, size_c);
    for(int i = 0; i < sizeRow; i++) {
        for(int j = 0; j < sizeCol; j++) {
            array[i][j] = m.array[i][j];
        }
    }
}

void matrix::setElement(int row, int col, float k)
{
    array[row][col] = k;
}

float matrix::getElement(int row, int col)
{
    return array[row][col];
}

// сложение матриц
matrix& matrix::operator + (matrix &m)
{
    if(sizeRow == m.sizeRow && sizeCol == m.sizeCol) {
        for(int i = 0; i < sizeRow; i++) {
            for(int j = 0; j < sizeCol; j++) {
                array[i][j] = array[i][j] + m.array[i][j];
            }
        }
    }

    return *this;
}

// вычитание матриц
matrix& matrix::operator - (matrix &m)
{
    if(sizeRow == m.sizeRow && sizeCol == m.sizeCol)
        for(int i = 0; i < sizeRow; i++)
            for(int j = 0; j < sizeCol; j++)
                array[i][j] = array[i][j] - m.array[i][j];

    return *this;
}

// умножение матриц
matrix& matrix::operator * (matrix &m)
{
    matrix tmp(sizeRow, m.sizeCol);
    if(sizeCol == m.sizeRow) {
        for(int i=0; i<tmp.sizeRow; i++) {
            for(int j=0; j<tmp.sizeCol; j++) {
                float sum = 0;
                for(int k = 0; k < sizeCol; k++)
                    sum += array[i][k] * m.array[k][j];
                tmp.array[i][j] = sum;
            }
        }
    }

    for(int ir = 0; ir < tmp.sizeRow; ir++) {
        for(int jr = 0; jr < tmp.sizeCol; jr++) {
            array[ir][jr] = tmp.array[ir][jr];
        }
    }

    return *this;
}

//операции со скалярами
matrix& matrix::operator + (float c)
{
    if(sizeRow == sizeCol) {
        for(int i = 0; i < sizeRow; i++) {
            array[i][i] += c;
        }
    }

    return *this;
}

matrix& matrix::operator - (float c)
{
    if(sizeRow == sizeCol) {
        for(int i = 0; i < sizeRow; i++) {
            array[i][i] -= c;
        }
    }

    return *this;
}

matrix& matrix::operator * (float c)
{
    for(int i = 0; i < sizeRow; i++) {
        for(int j = 0; j < sizeCol; j++) {
            array[i][j] = array[i][j] * c;
        }
    }

    return *this;
}

matrix& matrix::operator / (float c)
{
    for(int i = 0; i < sizeRow; i++) {
        for(int j = 0; j < sizeCol; j++) {
            array[i][j] = array[i][j] / c;
        }
    }

    return *this;
}

// транспонирование
matrix matrix::getTranspose()
{
    matrix tmp(*this);

    for(int i = 0; i < sizeRow; i++) {
        for(int j = 0; j < sizeCol; j++) {
            array[j][i] = tmp.array[i][j];
        }
    }

    return *this;
}

// Получить сумму по строке
float matrix::getRowSum(int n)
{
    float sum = 0;
    for(int i = 0; i < sizeRow; i++) {
        sum += array[n][i];
    }

    return sum;
}

// обратная матрица
matrix matrix::getInverse()
{
    matrix tmp(*this);
    setSingle(sizeRow);

    int n;
    int j = 0;
    int m = 0;
    float prm = 0;
    int tmpSize = sizeRow * 4;
    float A[tmpSize][tmpSize], L[tmpSize][tmpSize];

    for(int it = 0; it < sizeRow; it++) {
        for(int jt = 0; jt < sizeRow; jt++) {
            A[it][jt] = tmp.array[it][jt];
        }
    }

    for(int ia = 0; ia < sizeRow; ia++) {
        for(int ja = 0; ja < sizeRow; ja++) {
            L[ia][ja] = array[ia][ja];
        }
    }

    do {
        m = j;

        for(int nj = j+1; nj < sizeRow; nj++) {
            if(fabs(A[m][j]) < fabs(A[nj][j])) {
                m = nj;
            }
        }

        for(int n = 0; n < sizeRow; n++) {
            prm = A[m][n];
            A[m][n] = A[j][n];
            A[j][n] = prm;

            prm = A[m][n];
            A[m][n] = A[j][n];
            A[j][n] = prm;
        }

        prm = A[j][j];
        for(int np = 0; np < sizeRow; np++) {
            A[j][np] = float(A[j][np] / prm);
            L[j][np] = float(L[j][np] / prm);
        }

        for(int in=0; in<=sizeRow; in++) {
            if (in == j) {
                continue;
            }

            prm = A[in][j];
            for(int n1 = j; n1 <= sizeRow; n1++) {
                A[in][n1] = A[in][n1] - float(prm * A[j][n1]);
            }

            for(int n2 = 0; n2 <= sizeRow; n2++) {
                L[in][n2] = L[in][n2] - float(prm * L[j][n2]);
            }
        }

        j++;
    } while (j < sizeRow);

    for(int ir = 0; ir < sizeRow; ir++) {
        for(int jr = 0; jr < sizeRow; jr++) {
            array[ir][jr] = L[ir][jr];
        }
    }

    return *this;
}

// проверка на неотрицательность
bool matrix::isNotNegative()
{
    for(int i = 0; i < sizeRow; i++) {
        for(int j = 0; j < sizeCol; j++) {
            if(array[i][j] < 0) {
                 return false;
            }
        }
    }

    return true;
}

bool matrix::isDiagonalLower()
{
    for(int i = 0; i < sizeRow; i++) {
        if(array[i][i] > 1) {
             return false;
        }
    }

    return true;
}

//проверка на продуктивность
bool matrix::isProductive()
{
    matrix tmp(*this);
    matrix E;
    E.setSingle(sizeRow);

    if (!tmp.isNotNegative()) {
        return false;
    }

    if (!tmp.isDiagonalLower()) {
        return false;
    }

    tmp = E - tmp;
    tmp.getInverse();

    for(int i = 0; i < tmp.sizeCol; i++) {
        for(int j = 0; j < tmp.sizeRow;j++) {
            if(tmp.array[i][j] < 0) {
                 return false;
            }
        }
    }

    return true;
}

// Возведение в степень
matrix matrix::getPow(int p)
{
    matrix tmp1(*this);
    matrix tmp2(*this);
    matrix tmp3(*this);

    if(p == -1) {
        getInverse();
    }

    if(p == 0) {
        if(sizeRow = sizeCol) {
             setSingle(sizeRow);
        }
    }

    if(p > 1) {
        matrix mult(sizeRow, sizeCol);
        for(int i = 1; i < p; i++) {
            if(i != 1) {
                 tmp1 = mult;
            }

            tmp1 = tmp1 * tmp2;
            mult = tmp1;
            tmp1 = tmp2;
        }

        for(int ir = 0; ir < sizeRow; ir++) {
            for(int jr = 0; jr < sizeRow; jr++) {
                array[ir][jr] = mult.array[ir][jr];
            }
        }
    }

    return *this;
}

matrix matrix::getExp(float dt)
{
    matrix a(*this);
    matrix ea(sizeRow, sizeCol);
    matrix aa(sizeRow, sizeCol);
    matrix wm(sizeRow, sizeCol);
    matrix w(sizeRow, 1);

    double am, em, emi;
    int i, j, k, ii;
    em = 0;
    for(int ia = 0; ia < sizeRow; ia++) {
        for(int ja = 0; ja < sizeRow; ja++) {
            ea.setElement(ia, ja, 0);
            aa.setElement(ia, ja, 0);
            wm.setElement(ia, ja, 0);
            a.setElement(ia, ja, a.getElement(ia, ja)*dt);
            am = Abs(a.getElement(ia, ja));
            em = Max(am, em);
        }

        ea.setElement(ia, ia, 1);
        aa.setElement(ia, ia, 1);
        wm.setElement(ia, ia, 1);
    }

    emi = 1;
    ii = 0;
    while(emi > EPS) {
        ii++;
        if(ii >= 40) {
            break;
        }

        emi = 0;
        for(int jb = 0; jb < sizeRow; jb++) {
            for(int ib = 0; ib < sizeRow; ib++) {
                w.setElement(ib, 0, wm.getElement(ib, jb));
            }

            for(int ic = 0; ic < sizeRow; ic++) {
                wm.setElement(ic, jb, 0);
                for(int k = 0; k < sizeRow; k++) {
                    wm.setElement(ic, jb, wm.getElement(ic, jb) + (a.getElement(ic, k) * w.getElement(k, 0)));
                }
            }
        }

        for(int id = 0; id < sizeRow; id++) {
            for(int jd = 0; jd < sizeRow; jd++) {
                wm.setElement(id, jd, wm.getElement(id, jd) / (double)ii);
                ea.setElement(id, jd, ea.getElement(id, jd) + wm.getElement(id, jd));
                aa.setElement(id, jd, aa.getElement(id, jd) + (wm.getElement(id, jd) / (double)(ii + 1)));
                am = Abs(wm.getElement(id, jd));
                emi = Max(am, emi);
            }
        }

    }

    for(int it = 0; it < sizeRow; it++) {
        for(int jt = 0; jt < sizeRow; jt++) {
            array[it][jt] = ea.array[it][jt];
        }
    }

    return *this;
}
