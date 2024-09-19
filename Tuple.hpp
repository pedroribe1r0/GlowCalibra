#pragma once

#include <iostream>

using namespace std;
namespace Tuples
{
    template <typename TIPO>
    class Tuple
    {
    public:
        TIPO x;
        TIPO y;
        Tuple(TIPO x, TIPO y);
        Tuple();
        ~Tuple();

        // sobrecargas de operadores:

        // basicos ( =, +, -, *, / )

        void operator=(Tuple<TIPO> Tuple);
        void operator+=(Tuple<TIPO> Tuple);
        void operator-=(Tuple<TIPO> Tuple);
        void operator*=(double escalar);
        void operator/=(double escalar);

        void operator()(TIPO x, TIPO y)
        {
            this->x = x;
            this->y = y;
        }

        Tuple<TIPO> operator+(Tuple<TIPO> Tuple);
        Tuple<TIPO> operator-(Tuple<TIPO> Tuple);
        Tuple<TIPO> operator*(Tuple<TIPO> Tuple);
        Tuple<TIPO> operator*(double escalar);
        Tuple<TIPO> operator/(Tuple<TIPO> Tuple);
        Tuple<TIPO> operator/(double escalar);
    };

    typedef Tuple<float> TupleF;
    typedef Tuple<int> TupleI;
    typedef Tuple<unsigned int> TupleU;

    template <typename TIPO>
    Tuple<TIPO>::Tuple(TIPO x, TIPO y) : x(x),
                                         y(y)
    {
    }
    template <typename TIPO>
    Tuple<TIPO>::Tuple()
    {
        this->x = 0;
        this->y = 0;
    }
    template <typename TIPO>
    Tuple<TIPO>::~Tuple()
    {
    }

    template <typename TIPO>
    void Tuple<TIPO>::operator=(Tuple<TIPO> Tuple)
    {
        this->x = Tuple.x;
        this->y = Tuple.y;
    }
    template <typename TIPO>
    Tuple<TIPO> Tuple<TIPO>::operator+(Tuple<TIPO> other)
    {
        return Tuple<TIPO>(this->x + other.x, this->y + other.y);
    }

    template <typename TIPO>
    Tuple<TIPO> Tuple<TIPO>::operator-(Tuple<TIPO> other)
    {
        return Tuple<TIPO>(this->x - other.x, this->y - other.y);
    }

    template <typename TIPO>
    Tuple<TIPO> Tuple<TIPO>::operator/(double escalar)
    {
        return Tuple<TIPO>(this->x / escalar, this->y / escalar);
    }

    template <typename TIPO>
    void Tuple<TIPO>::operator+=(Tuple<TIPO> Tuple)
    {
        this->x += Tuple.x;
        this->y += Tuple.y;
    }

}