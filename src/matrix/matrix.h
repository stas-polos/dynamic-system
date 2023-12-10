#ifndef MATRIX_H
#define MATRIX_H

class matrix
{
private:
    float** array;
    int sizeRow;
    int sizeCol;
public:
    matrix();
    matrix(matrix &m);
    matrix(int row, int col);
    ~matrix();
    void init(float c);
    void initStat(float *p, int size_r, int size_c);
    void initDynam(float **p, int size_r, int size_c);
    void initMatrix(matrix &m, int size_r, int size_c);
    void setElement(int row, int col, float k);
    float getElement(int row, int col);
    void setSize(int row, int col);
    int getSizeRow();
    int getSizeCol();
    float& operator () (int row, int col);
    float& operator [] (int k);
    matrix& operator = (matrix &m);
    matrix& operator + (matrix &m);
    matrix& operator - (matrix &m);
    matrix& operator * (matrix &m);
    matrix& operator + (float c);
    matrix& operator - (float c);
    matrix& operator * (float c);
    matrix& operator / (float c);
    float getRowSum(int n);
    matrix	getExp(float dt);
    matrix  getInverse();
    matrix	getTranspose();
    matrix  getPow(int p);
    matrix& setSingle(int n);
    bool isNotNegative();
    bool isDiagonalLower();
    bool isProductive();
};
#endif // MATRIX_H
