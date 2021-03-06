#include "drawabletrapezoid.h"
#include <cg3/geometry/intersections2.h>
#include <random>
#include <cg3/viewer/opengl_objects/opengl_objects2.h>

double DrawableTrapezoid::yMin = -1;
double DrawableTrapezoid::yMax = -1;

DrawableTrapezoid::DrawableTrapezoid(const OrderedSegment& t, const OrderedSegment& b, const cg3::Point2d& lp, const cg3::Point2d& rp, bool doCalculateGraphics) : Trapezoid(t,b, lp, rp), hasGraphics(doCalculateGraphics)
{
    if (!doCalculateGraphics) return;

    calculateGraphics();
}

void DrawableTrapezoid::calculateGraphics() {
    this->hasGraphics = true;

    // SETTING THE COLOR
    setRandomColor();

    //  PRE-COMPUTING THE 4 VERTECES THAT MADE UP THE TRAPEZOID INSTEAD OF DOING AT EVERY DRAW CALL
    setVerteces();
}

bool DrawableTrapezoid::isGraphicsCalculated() const {
    return hasGraphics;
}

void DrawableTrapezoid::drawPolygon() const {
    assert(this->isGraphicsCalculated());

    /*source: https://www3.ntu.edu.sg/home/ehchua/programming/opengl/cg_introduction.html*/
    glBegin(GL_POLYGON);            // These vertices form a closed polygon
        if(this->isHighlighted) {
            glColor3f(POLYGON_COLOR_WHEN_HIGHLIGHTED.redF(), POLYGON_COLOR_WHEN_HIGHLIGHTED.greenF(), POLYGON_COLOR_WHEN_HIGHLIGHTED.blueF());
        } else {
            glColor3f(this->polygonColor.redF(), this->polygonColor.greenF(), this->polygonColor.blueF());
        }
        glVertex2f(this->topLeftVertex.x(),     this->topLeftVertex.y());
        glVertex2f(this->topRightVertex.x(),    this->topRightVertex.y());
        glVertex2f(this->bottomRightVertex.x(), this->bottomRightVertex.y());
        glVertex2f(this->bottomLeftVertex.x(),  this->bottomLeftVertex.y());
     glEnd();
}

void DrawableTrapezoid::drawVerticalLines() const {
    assert(this->isGraphicsCalculated() == true);
    cg3::opengl::drawLine2(this->topLeftVertex,  this->bottomLeftVertex, this->segmentColor, this->segmentSize);
    cg3::opengl::drawLine2(this->topRightVertex, this->bottomRightVertex, this->segmentColor, this->segmentSize);
}


void DrawableTrapezoid::setRandomColor() {
    assert(this->polygonColor!=nullptr);

    /* COMMENT THE BELOW CODE AND USE THE OTHER FOR MORE PERFORMANCE */
    // source: https: https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<double> dis(min_random_value, max_random_value);
    this->polygonColor.setRedF(dis(gen));
    this->polygonColor.setBlueF(dis(gen));
    this->polygonColor.setGreenF(dis(gen));


    /* QUICKER VERSION */
//    this->polygonColor.setRed(0);
//    this->polygonColor.setBlue(0);
//    this->polygonColor.setGreen(0);
}

void DrawableTrapezoid::setVerteces() {
    char code;
    const double thres = cg3::CG3_EPSILON;

    /* Create two vertical line as height as the bounding box
     * The first  has the x of the leftpoint of the trapezoid
     * The second has the x of the rightpoint of the trapezoid
     *
     * When we don't know a vertex, we calculate the intersection point between a vertical line and a top/bottom segment
    */
    cg3::Segment2d leftVerticalLine = cg3::Segment2d(
                cg3::Point2d(getLeftp().x(), DrawableTrapezoid::getYMax()),
                cg3::Point2d(getLeftp().x(), DrawableTrapezoid::getYMin())
                );
    cg3::Segment2d rightVerticalLine = cg3::Segment2d(
                    cg3::Point2d(getRightp().x(), DrawableTrapezoid::getYMax()),
                    cg3::Point2d(getRightp().x(), DrawableTrapezoid::getYMin())
                    );

    // flag: is the left point on the top segment?
    bool leftpOnTop = getLeftp() == getTop().getLeftmost();
    // flag: is the right point on the top segment?
    bool leftpOnBottom= getLeftp() == getBottom().getLeftmost();

    /// LEFT VERTECES
    /* DEGENERATIVE CASE */
    if(leftpOnBottom && leftpOnTop) {
        this->topLeftVertex = getLeftp();
        this->bottomLeftVertex = getLeftp();
    }
    /* NORMAL CASE */
    else if(leftpOnBottom) {
        this->bottomLeftVertex = getLeftp();
        cg3::checkSegmentIntersection2(leftVerticalLine,  getTop(), code, thres, topLeftVertex);
        assert(code == 'v' || code == '1'); // assert an intersection has been found
    }
    else if (leftpOnTop) {
        this->topLeftVertex = getLeftp();
        cg3::checkSegmentIntersection2(leftVerticalLine,  getBottom(), code, thres, bottomLeftVertex);
        assert(code == 'v' || code == '1');
    } else {
        cg3::checkSegmentIntersection2(leftVerticalLine,  getTop(), code, thres, topLeftVertex);
        assert(code == 'v' || code == '1');
        cg3::checkSegmentIntersection2(leftVerticalLine,  getBottom(), code, thres, bottomLeftVertex);
        assert(code == 'v' || code == '1');
    }

    /// RIGHT VERTECES
    // flag: is the right point on the top segment?
    bool rightpOnTop = getRightp() == getTop().getRightmost();
    // flag: is the right point on the bottom segment?
    bool rightpOnBottom= getRightp() == getBottom().getRightmost();

    /* DEGENERATIVE CASE */
    if(rightpOnBottom && rightpOnTop) {
        this->bottomRightVertex = getRightp();
        this->topRightVertex = getRightp();
    }
    /* NORMAL CASE */
    else if(rightpOnBottom) {
        this->bottomRightVertex = getRightp();
        cg3::checkSegmentIntersection2(rightVerticalLine,  getTop(), code, thres, topRightVertex);
    }
    else if (rightpOnTop) {
        this->topRightVertex = getRightp();
        cg3::checkSegmentIntersection2(rightVerticalLine,  getBottom(), code, thres, bottomRightVertex);
        assert(code == 'v' || code == '1');

    } else {
        cg3::checkSegmentIntersection2(rightVerticalLine, getTop(), code, thres, topRightVertex);
        assert(code == 'v' || code == '1');

        cg3::checkSegmentIntersection2(rightVerticalLine, getBottom(), code, thres, bottomRightVertex);
        assert(code == 'v' || code== '1');
    }
}

bool DrawableTrapezoid::getIsHighlighted() const
{
    return isHighlighted;
}

void DrawableTrapezoid::setIsHighlighted(bool newIsHighlighted)
{
    isHighlighted = newIsHighlighted;
}

double DrawableTrapezoid::getYMin()
{
    return DrawableTrapezoid::yMin;
}

void DrawableTrapezoid::setYMin(double newYMin)
{
    DrawableTrapezoid::yMin = newYMin;
}

double DrawableTrapezoid::getYMax()
{
    return DrawableTrapezoid::yMax;
}

void DrawableTrapezoid::setYMax(double newYMax)
{
    DrawableTrapezoid::yMax = newYMax;
}
