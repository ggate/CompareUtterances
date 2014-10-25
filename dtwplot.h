// Copyright 2014 Gwennael Gate.
//
// Author: Gwennael Gate (gwennaelgate@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// -----------------------------------------------------------------------------
//
// This plots in a QT widget the dtw matrix and the path that minimizes the $
// total cost.

#ifndef DTWPLOT_H
#define DTWPLOT_H
#include <QtGui>
#include <QWidget>
#include <boost/numeric/ublas/matrix.hpp>
#include <vector>

namespace ublas = boost::numeric::ublas;
class DtwPlot : public QWidget
{
    Q_OBJECT
public:
    DtwPlot(ublas::matrix<float> *mat,
            std::vector< ublas::matrix<unsigned int> > *path);

protected:
    void paintEvent(QPaintEvent *event);

private:
    // This plots 2 surrounding lines around the optimal path used
    // to compute how deep is the valley (and thus how good is
    // the match)
    float plotValley(QPen *pen,
                     QPainter * painter,
                     ublas::matrix<float> * dtw,
                     std::vector< ublas::matrix<unsigned int> > * path);

    // Pointers to the dtw matrix and the optimal path
    ublas::matrix<float> * fDtw;
    std::vector< ublas::matrix<unsigned int> > * fPath;
};

#endif // DTWPLOT_H
