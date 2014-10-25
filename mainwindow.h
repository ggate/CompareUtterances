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
// Simplistic QT user interface to select the 2 utterances features files and
// to trigger the computation of the match (dtw) and the corresponding displays.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

#include "utterancecomparator.h"
#include "dtwplot.h"
#include "mfccplot.h"

namespace ublas = boost::numeric::ublas;
class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget * parent = 0);
    ~MainWindow();

public slots:
    // Triggers the display of the dtw matrix
    void displayDtwPlotWindow();

    // Triggers the display of the 2 utterances mfcc vectors
    void displayMfccPlotWindow();

    // Update filenames
    void getPathUtt1();
    void getPathUtt2();

    // Reset flag for dtw to be recomputed
    void dtwToBeRecomputed();
private:
    // Stores the dtw matrix
    ublas::matrix<float> fDtw;

    // Stores the optimal path in the dtw matrix
    std::vector< ublas::matrix<unsigned int> > fPath;

    // Stores the 2 utterances feature vectors
    ublas::matrix<float> fUtt1;
    ublas::matrix<float> fUtt2;

    // Instances the comparison class
    UtteranceComparator fComparator;

    // QT widgets
    DtwPlot * fPlotDTW;
    QWidget * fMfccWin;
    MfccPlot * fMfccPlot;
    QLineEdit * fUtt1Line;
    QLineEdit * fUtt2Line;
    QGridLayout *fLayout;
    QPushButton * fUtt1Selection;
    QPushButton * fUtt2Selection;
    QPushButton * fDtwWindow;
    QPushButton * fMfccWindow;
    QGridLayout *fLayout2;
    QLabel *fLabel;
    QLabel *fLabelSuccess;
    QComboBox * fCombo;


    // Flag when dtw needs to be recomputed
    bool fIsDtwComputed;

    // Similarity score
    float fResult;
};

#endif // MAINWINDOW_H
