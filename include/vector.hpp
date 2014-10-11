#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include <algorithm>
#include <cassert>
#include <utility>

template <typename Scalar>
class Vector {
public:
    Vector(int n=0) : n_(n), is_view_(false) { allocate(); }

    // Copy constructor
    Vector(Vector<Scalar>& that);

    // Move constructor
    Vector(Vector<Scalar>&& that) : Vector() { swap(*this, that); }

    // copy assignment
    Vector<Scalar>& operator=(Vector<Scalar> that);

    friend void swap(Vector<Scalar>& first, Vector<Scalar>& second)  {
        std::swap(first.n_, second.n_);
        std::swap(first.is_view_, second.is_view_);
        std::swap(first.data_, second.data_);
    }

    Vector(Scalar *data, int n) : data_(data), n_(n), is_view_(true) {}

    Vector(int n, Scalar fill);

    ~Vector();

    // Scale the vector by the constant scale.
    void Scale(Scalar scale);

    // Scale the vector to have norm 1
    void Normalize1();

    // Pointer to the raw data.
    Scalar *data() { return data_; }

    // Number of elements in the vector.
    int n() { return n_; }

    // Return the sum of the vector elements.
    double Sum();

    // Normalize the vector by its sum.
    void Normalize();

    // Fill the vector with a value in all entries.
    void Fill(Scalar fill);

    // Access the i-th element.
    const Scalar& operator()(int i) const;
    Scalar& operator()(int i);
  
private:
    void allocate();
    void deallocate();
  
    Scalar *data_;
    int n_;
    bool is_view_;
};


// Copy constructor
template <typename Scalar>
Vector<Scalar>::Vector(Vector<Scalar>& that) {
    n_ = that.n();
    allocate();
    Scalar *that_data = that.data();
    for (int i = 0; i < n_; ++i) {
        data_[i] = that_data[i];
    }
}

// copy assignment
template <typename Scalar>
Vector<Scalar>& Vector<Scalar>::operator=(Vector<Scalar> that) {
    swap(*this, that);
    return *this;
}

template <typename Scalar>
Vector<Scalar>::Vector(int n, Scalar fill) : n_(n) {
    allocate();
    Fill(fill);
}

template <typename Scalar>
Vector<Scalar>::~Vector() {
    if (data_ != NULL && !is_view_ && n_ > 0) {
        deallocate();
    }
}

template <typename Scalar>
void Vector<Scalar>::Scale(Scalar scale) {
    for (int i = 0; i < n_; ++i) {
        data_[i] *= scale;
    }
}

template <typename Scalar>
void Vector<Scalar>::Normalize1() {
    double norm1 = Norm1(*this);
    Scale(1.0 / norm1);
}

template <typename Scalar>
void Vector<Scalar>::allocate() {
    if (n_ > 0) {
        data_ = new Scalar[n_];
        assert(data_ != NULL);
    }
}

template <typename Scalar>
void Vector<Scalar>::deallocate() {
    if (data_ != NULL) {
        delete[] data_;
        data_ = NULL;
    }
}

template <typename Scalar>
double Vector<Scalar>::Sum() {
    double sum = 0.0;
    for (int i = 0; i < n_; ++i) {
        sum += data_[i];
    }
    return sum;
}

template <typename Scalar>
void Vector<Scalar>::Normalize() {
    double sum = Sum();
    for (int i = 0; i < n_; ++i) {
        data_[i] /= sum;
    }
}

template <typename Scalar>
void Vector<Scalar>::Fill(Scalar fill) {
    for (int i = 0; i < n_; ++i) {
        data_[i] = fill;
    }
}

template <typename Scalar>
const Scalar& Vector<Scalar>::operator()(int i) const  {
    assert(i >= 0 && i < n_);
    return data_[i]; 
}

template <typename Scalar>
Scalar& Vector<Scalar>::operator()(int i)  {
    assert(i >= 0 && i < n_);
    return data_[i]; 
}

#endif  // _VECTOR_HPP_
