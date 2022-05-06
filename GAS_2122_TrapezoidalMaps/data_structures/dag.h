#ifndef DAG_H
#define DAG_H

#include "cg3/geometry/point2.h"
#include "orderedsegment.h"
#include "dagnode.h"

class DAG
{
public:
    DAG();
    void initialize( DrawableTrapezoid* B); // Necessary because the DAG constructor is implicited called at the beginning of the TrapezoidalMap...
    void replaceNodeWithSubtree(DAGNode* nodeToReplace, OrderedSegment& segmentSplitting,
                                DrawableTrapezoid* left=nullptr, DrawableTrapezoid* top=nullptr, DrawableTrapezoid* bottom=nullptr, DrawableTrapezoid* right=nullptr);
    DrawableTrapezoid* query(const cg3::Point2d& q);
    void clear();

private:
    // root
    DAGNode* root = nullptr;
    DrawableTrapezoid* queryRec(const cg3::Point2d& q, DAGNode* root);
    void clearRec(DAGNode* root);
};



#endif // DAG_H
