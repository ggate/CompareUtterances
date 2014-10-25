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
// -----------------------------------------------------------------------

#include "dtwplot.h"

DtwPlot::DtwPlot(ublas::matrix<float> * mat,
                 std::vector< ublas::matrix<unsigned int> > * path) : QWidget(),
    fDtw(mat),
    fPath(path)
{
}


void DtwPlot::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rect;
    int width = 4;
    for (int i = 0 ; i < fDtw->size1() ; i++)
        for (int j = 0 ; j < fDtw->size2() ; j++)
        {
            rect.setCoords(j*width,i*width,(j+1)*width,(i+1)*width);
            painter.fillRect(rect, QColor(10,10,10, abs(((*fDtw)(i,j)/1000000)*255)));
        }


    QPen myPen(Qt::red, 2, Qt::SolidLine);
    painter.setPen(myPen);

    for (int i = 0 ; i < fPath->size() ; ++i)
    {
        painter.drawEllipse(fPath->at(i)(0,1)*width,
                            fPath->at(i)(0,0)*width,
                            width,
                            width);
    }

    myPen.setColor(Qt::green);
    myPen.setWidth(2);
    painter.setPen(myPen);

    plotValley(&myPen, &painter, fDtw, fPath);
}

float DtwPlot::plotValley(QPen *pen,
                          QPainter * painter,
                          ublas::matrix<float> * dtw,
                          std::vector< ublas::matrix<unsigned int> > * path)
{
    int width = 4;
    int radius = 20;
    QPoint tangent;
    QPoint upper;
    QPoint lower;

    for (int i = 0 ; i < path->size()-1 ; ++i)
    {
        int i1 = path->at(i)(0,0);
        int j1 = path->at(i)(0,1);

        int i2 = path->at(i+1)(0,0);
        int j2 = path->at(i+1)(0,1);

        tangent.setX(i2-i1);
        tangent.setY(j2-j1);


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

        painter->drawEllipse(upper.y()*width,
                             upper.x()*width,
                             width,
                             width);
        painter->drawEllipse(lower.y()*width,
                             lower.x()*width,
                             width,
                             width);
    }
}
