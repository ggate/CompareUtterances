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
// This plots in a QT widget the 2 utterances features and the match sample per
// samples between them.

#ifndef MFCCPLOT_H
#define MFCCPLOT_H

#include <QtGui>
#include <QWidget>
#include <boost/numeric/ublas/matrix.hpp>
#include <vector>

namespace ublas = boost::numeric::ublas;
class MfccPlot : public QWidget
{
    Q_OBJECT
public:
    MfccPlot(ublas::matrix<float> *utt1,
             ublas::matrix<float> *utt2,
             std::vector< ublas::matrix<unsigned int> > *path,
             ublas::matrix<float> *dtw);

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void setIndex(int k);

private:
    // Outputs a value that is correlated with how good is the match between
    // utt1 feature vector n°i1 and utt2 feature vector n°j1
    float valleyDeepness(ublas::matrix<float> * dtw,
                         int i1,
                         int j1,
                         int i2,
                         int j2);

    // Pointers to the 2 utterances
    ublas::matrix<float> * fUtt1;
    ublas::matrix<float> * fUtt2;

    // Pointers to the "minimal cost path" in the dtw
    std::vector< ublas::matrix<unsigned int> > * fPath;

    // Stores the mfcc component that is currently plotted
    int fIndex;

    // Pointer to the dtw matrix
    ublas::matrix<float> * fDtw;
};

#endif // DTWPLOT_H
