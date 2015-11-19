#pragma once

#include <cstring>

namespace vec {

template <class T, int D>
class Vector
{
public:
    Vector();
    virtual ~Vector();

    int size() const;

    const T& operator()(int idx) const;
    T& operator()(int idx);

    Vector<T, D> operator+(const Vector<T, D>& rhs) const;
    Vector<T, D> operator-(const Vector<T, D>& rhs) const;
    Vector<T, D> operator*(T multiplier) const;
    Vector<T, D> operator/(T divisor) const;

    void operator+=(const Vector<T, D>& rhs);
    void operator+=(T val);
    void operator-=(const Vector<T, D>& rhs);
    void operator-=(T val);
    void operator*=(T multiplier);
    void operator/=(T divisor);

    T dot(const Vector<T, D>& rhs) const;

    T length() const;
    T lengthSq() const;

    Vector<T, D> getUnit() const;

protected:
    T m_vec[D];
};

template <class T, int D>
Vector<T, D>::Vector() {
    memset(m_vec, 0, sizeof(m_vec));
}

template <class T, int D>
Vector<T, D>::~Vector() {
}

template <class T, int D>
int Vector<T, D>::size() const {
    return D;
}

template <class T, int D>
const T& Vector<T, D>::operator()(int idx) const {
    return m_vec[idx];
}

template <class T, int D>
T& Vector<T, D>::operator()(int idx) {
    return m_vec[idx];
}

template <class T, int D>
Vector<T, D> Vector<T, D>::operator+(const Vector<T, D>& rhs) const {
    Vector<T, D> result;
    for (int i = 0; i < D; i++) {
        result(i) = m_vec[i] + rhs(i);
    }
    return result;
}

template <class T, int D>
Vector<T, D> Vector<T, D>::operator-(const Vector<T, D>& rhs) const {
    Vector<T, D> result;
    for (int i = 0; i < D; i++) {
        result(i) = m_vec[i] - rhs(i);
    }
    return result;
}

template <class T, int D>
Vector<T, D> Vector<T, D>::operator*(T multiplier) const {
    Vector<T, D> result;
    for (int i = 0; i < D; i++) {
        result(i) = m_vec[i] * multiplier;
    }
    return result;
}

template <class T, int D>
Vector<T, D> Vector<T, D>::operator/(T divisor) const {
    Vector<T, D> result;
    for (int i = 0; i < D; i++) {
        result(i) = m_vec[i] / divisor;
    }
    return result;
}

template <class T, int D>
void Vector<T, D>::operator+=(const Vector<T, D>& rhs) {
    for (int i = 0; i < D; i++) {
        m_vec[i] += rhs(i);
    }
}

template <class T, int D>
void Vector<T, D>::operator+=(T val) {
    for (int i = 0; i < D; i++) {
        m_vec[i] += val;
    }
}

template <class T, int D>
void Vector<T, D>::operator-=(const Vector<T, D>& rhs) {
    for (int i = 0; i < D; i++) {
        m_vec[i] -= rhs(i);
    }
}

template <class T, int D>
void Vector<T, D>::operator-=(T val) {
    for (int i = 0; i < D; i++) {
        m_vec[i] -= val;
    }
}

template <class T, int D>
void Vector<T, D>::operator*=(T multiplier) {
    for (int i = 0; i < D; i++) {
        m_vec[i] *= multiplier;
    }
}

template <class T, int D>
void Vector<T, D>::operator/=(T divisor) {
    for (int i = 0; i < D; i++) {
        m_vec[i] /= divisor;
    }
}

template <class T, int D>
T Vector<T, D>::dot(const Vector<T, D>& rhs) const {
    T result = m_vec[0] * rhs(0);
    for (int i = 1; i < D; i++) {
        result += m_vec[i] * rhs(i);
    }
    return result;
}

template <class T, int D>
T Vector<T, D>::length() const {
    return sqrt(lengthSq());
}

template <class T, int D>
T Vector<T, D>::lengthSq() const {
    T result = m_vec[0] * m_vec[0];
    for (int i = 1; i < D; i++) {
        result += m_vec[i] * m_vec[i];
    }
    return result;
}

template <class T, int D>
Vector<T, D> Vector<T, D>::getUnit() const {
    T len = length();
    if (len != 0) {
        return *this / len;
    }

    Vector<T, D> defaultUnit;
    defaultUnit(0) = (T)1;
    return defaultUnit;
}

} // of namespace vec
