#ifndef MATRIX
#define MATRIX

class Matrix {
    private:
        int w,h;
        float *elements;
    public:
        Matrix(int _w, int _h);
        float get(int indx);
        void init();
};


#endif
