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

#include "mfccplot.h"

MfccPlot::MfccPlot(ublas::matrix<float> *utt1,
                   ublas::matrix<float> *utt2,
                   std::vector< ublas::matrix<unsigned int> > *path,
                   ublas::matrix<float> *dtw) : QWidget(),
    fUtt1(utt1),
    fUtt2(utt2),
    fPath(path),
    fDtw(dtw),
    fIndex(0)
{

}

void MfccPlot::setIndex(int k)
{
    fIndex = k;    
    update();
}


void MfccPlot::paintEvent(QPaintEvent *event)
{    
    QPainter painter(this);

    QPen myPen(Qt::blue, 2, Qt::SolidLine);
    painter.setPen(myPen);

    unsigned int zeroAxis1 = 200;
    unsigned int zeroAxis2 = 500;
    unsigned int step = 7;
    unsigned int zoom = 5;

    myPen.setColor(Qt::black);
    painter.setPen(myPen);
    painter.drawLine(0, zeroAxis1, 1500, zeroAxis1);
    myPen.setColor(Qt::red);
    painter.setPen(myPen);
    for(unsigned int i = 0 ; i < fUtt1->size1()-1 ; i++)
    {
        painter.drawLine(i*step,
                         zeroAxis1 - zoom*(*fUtt1)(i,fIndex),
                         (i+1)*step,
                         zeroAxis1 - zoom*(*fUtt1)(i+1,fIndex));
    }

    myPen.setColor(Qt::black);
    painter.setPen(myPen);
    painter.drawLine(0, zeroAxis2, 1500, zeroAxis2);
    myPen.setColor(Qt::blue);
    painter.setPen(myPen);
    for(unsigned int i = 0 ; i < fUtt2->size1()-1 ; i++)
    {
        painter.drawLine(i*step,
                         zeroAxis2 - zoom*(*fUtt2)(i,fIndex),
                         (i+1)*step,
                         zeroAxis2 - zoom*(*fUtt2)(i+1,fIndex));
    }

    myPen.setColor(Qt::gray);
    myPen.setWidth(1);
    painter.setPen(myPen);
    for (unsigned int i = 0 ; i < fPath->size() ; i++)
    {
        if(fPath->at(i)(0,0) < fUtt1->size1()
                && fPath->at(i)(0,1) < fUtt2->size1())
        {
            painter.drawLine(fPath->at(i)(0,0)*step,
                             zeroAxis1 - zoom*(*fUtt1)(fPath->at(i)(0,0),fIndex),
                             fPath->at(i)(0,1)*step,
                             zeroAxis2 - zoom*(*fUtt2)(fPath->at(i)(0,1),fIndex));
        }
    }

    float zoom2 = 0.001;
    myPen.setColor(Qt::green);
    myPen.setWidth(2);
    painter.setPen(myPen);
    for (unsigned int i = 0 ; i < fPath->size()-3 ; i++)
    {
        painter.drawLine(fPath->at(i)(0,0)*step,
                         zeroAxis1 - zoom2*valleyDeepness(fDtw, fPath->at(i)(0,0), fPath->at(i)(0,1), fPath->at(i+1)(0,0), fPath->at(i+1)(0,1)),
                         fPath->at(i+1)(0,0)*step,
                         zeroAxis1 - zoom2*valleyDeepness(fDtw, fPath->at(i+1)(0,0), fPath->at(i+1)(0,1), fPath->at(i+2)(0,0), fPath->at(i+2)(0,1)));
    }
}

float MfccPlot::valleyDeepness(ublas::matrix<float> * dtw,
                               int i1, int j1,
                               int i2, int j2)
{
    int radius = 20;
    QPoint tangent;
    tangent.setX(i2-i1);
    tangent.setY(j2-j1);
    QPoint upper;
    QPoint lower;

    upper.setX(i1 - radius*tangent.y());
    upper.setY(j1 + radius*tangent.x());

    lower.setX(i1 + radius*tangent.y());
    lower.setY(j1 - radius*tangent.x());
    if(upper.x() < 5)
        upper.setX(5);
    if(upper.x() >= dtw->size1()-1)
        upper.setX(dtw->size1()-1);

    if(upper.y() < 5)
        upper.setY(5);
    if(upper.y() >= dtw->size2()-1)
        upper.setY(dtw->size2()-1);


    if(lower.x() < 5)
        lower.setX(5);
    if(lower.x() >= dtw->size1())
        lower.setX(dtw->size1()-1);

    if(lower.y() < 5)
        lower.setY(5);
    if(lower.y() >= dtw->size2())
        lower.setY(dtw->size2()-1);

    return (*dtw)(upper.x(), upper.y()) - (*dtw)(i1, j1) +
            (*dtw)(lower.x(), lower.y()) - (*dtw)(i1, j1);
}
