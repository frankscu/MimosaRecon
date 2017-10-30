#ifndef MCLUSTER_H
#define MCLUSTER_H

class seed {
    public:
        seed(unsigned int tmp_x, unsigned int tmp_y, unsigned int tmp_nb,
                unsigned int cp)
            : x(tmp_x), y(tmp_y), neighbours(tmp_nb), p(cp) {
                x = tmp_x;
                y = tmp_y;
                neighbours = tmp_nb;
                p = cp;
            }
        // this operator is needed for the sort algorithm. the first
        // criteria is the number of neighbouring pixels and then the
        // second criteria is the number of fired pixel in a cluster
        // around the seed
        bool operator<(const seed &b) const {
            // return (measuredZ < b.measuredZ);
            bool r = true;
            if (neighbours == b.neighbours) {
                if (p < b.p)
                    r = false;
            } else if (neighbours < b.neighbours)
                r = false;
            return r;
        }
        unsigned int x;          // x coordinate
        unsigned int y;          // y coordinate
        unsigned int neighbours; // number of neighbours
        unsigned int p; // total number of fired pixel in the cluster formed by
        // this seed pixel candidate
};


