#include <opencv2/opencv.hpp>
#include "Tuple.hpp"
#include <list>

typedef struct point
{
    Tuples::TupleF coordinate;
    bool acessed;
};

class Buble
{
private:
    std::list<point> borderPoints;
    float area;
    bool finished;

public:
    Buble();
    ~Buble();
    void calculateArea();
};

Buble::Buble()
{
}

Buble::~Buble()
{
}

void Buble::calculateArea()
{
    if (finished)
    {
        std::list<point>::iterator it = borderPoints.begin();
        std::list<point>::iterator aux = borderPoints.begin();
        aux++;
        do
        {
            area += ((it->coordinate.x * aux->coordinate.y) - (aux->coordinate.x * it->coordinate.y));

            it++;
            aux++;

        } while (aux != borderPoints.end());

        aux = borderPoints.begin();
        it++;
        area += ((it->coordinate.x * aux->coordinate.y) - (aux->coordinate.x * it->coordinate.y));

        area = abs(area / 2);
    }
}