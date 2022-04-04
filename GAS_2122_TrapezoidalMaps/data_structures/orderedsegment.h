#ifndef ORDEREDSEGMENT_H
#define ORDEREDSEGMENT_H

#include "cg3/geometry/segment2.h"
#include "cg3/geometry/point2.h"


// Class created because we need to know the leftmost/rightmost verteces of a segment but I don't want to check it every time a runtime...
class OrderedSegment : private cg3::Segment2d
{
public:
    OrderedSegment(cg3::Segment2d unordered_s);
    OrderedSegment(cg3::Point2d unordered_p1, cg3::Point2d unordered_p2);

    const cg3::Point2d& getLeftmost() const;
    const cg3::Point2d& getRightmost() const ;

private:
    void orderSegment();
};

#endif // ORDEREDSEGMENT_H
