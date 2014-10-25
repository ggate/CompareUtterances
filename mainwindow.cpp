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
#include "mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget * parent) : QWidget(parent)
{

    fLayout = new QGridLayout;

    fUtt1Selection = new QPushButton("Select Utt1", this);
    fUtt2Selection = new QPushButton("Select Utt2", this);

    fUtt1Line = new QLineEdit();
    fUtt2Line = new QLineEdit();

    fDtwWindow = new QPushButton("Display Dtw", this);
    fMfccWindow = new QPushButton("Display Mfccs", this);

    fLayout->addWidget(fUtt1Selection, 0, 0);
    fLayout->addWidget(fUtt1Line, 0, 1);
    fLayout->addWidget(fUtt2Selection, 1, 0);
    fLayout->addWidget(fUtt2Line, 1, 1);
    fLayout->addWidget(fDtwWindow, 2, 0);
    fLayout->addWidget(fMfccWindow, 2, 1);

    connect(fDtwWindow,
            SIGNAL(clicked()),
            this,
            SLOT(displayDtwPlotWindow()));

    connect(fMfccWindow,
            SIGNAL(clicked()),
            this,
            SLOT(displayMfccPlotWindow()));

    connect(fUtt1Selection,
            SIGNAL(clicked()),
            this,
            SLOT(getPathUtt1()));

    connect(fUtt2Selection,
            SIGNAL(clicked()),
            this,
            SLOT(getPathUtt2()));

    connect(fUtt1Line,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(dtwToBeRecomputed()));
    connect(fUtt2Line,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(dtwToBeRecomputed()));

    setLayout(fLayout);
    resize(800,800);
    fIsDtwComputed = false;
    show();
}

MainWindow::~MainWindow()
{
    delete fPlotDTW;
    delete fMfccWin;
    delete fMfccPlot;
    delete fUtt1Line;
    delete fUtt2Line;
    delete fLayout;
    delete fUtt1Selection;
    delete fUtt2Selection;
    delete fDtwWindow;
    delete fMfccWindow;
    delete fLayout2;
    delete fLabel;
    delete fLabelSuccess;
    delete fCombo;
}

void MainWindow::getPathUtt1()
{
    QString path;
    path = QFileDialog::getOpenFileName(this,
                                        tr("Please choose utt1 features file"),
                                        QDir::homePath());
    fUtt1Line->setText(path);
    fIsDtwComputed = false;
}

void MainWindow::getPathUtt2()
{
    QString path;
    path = QFileDialog::getOpenFileName(this,
                                        tr("Please choose utt2 features file"),
                                        QDir::homePath());
    fUtt2Line->setText(path);
    fIsDtwComputed = false;
}

void MainWindow::dtwToBeRecomputed()
{
    fIsDtwComputed = false;
}

void MainWindow::displayDtwPlotWindow()
{
    if(!fIsDtwComputed)
    {
        float score;
        fComparator.compareUtterances(fUtt1Line->text().toStdString(),
                                      fUtt2Line->text().toStdString(),
                                      fUtt1,
                                      fUtt2,
                                      fPath,
                                      fDtw,
                                      score);
        std::cout << "Similarity = " << score/fPath.size() << std::endl;
        fIsDtwComputed = true;
    }

    fPlotDTW = new DtwPlot(&fDtw, &fPath);
    fPlotDTW->resize(800,800);
    fPlotDTW->show();
}

void MainWindow::displayMfccPlotWindow()
{
    if(!fIsDtwComputed)
    {
        float score;
        fComparator.compareUtterances(fUtt1Line->text().toStdString(),
                                      fUtt2Line->text().toStdString(),
                                      fUtt1,
                                      fUtt2,
                                      fPath,
                                      fDtw,
                                      score);

        fResult = score/fPath.size();
        std::cout << "Similarity = " << fResult << std::endl;

        fIsDtwComputed = true;
    }

    std::cout << "value " << fUtt1.size1() << std::endl;

    fMfccPlot = new MfccPlot(&fUtt1, &fUtt2, &fPath, &fDtw);
    fMfccWin = new QWidget;

    fLayout2 = new QGridLayout;

    QString value = QString::number(fResult);
    fLabel = new QLabel("Similarity Score = " + value);
    fLabelSuccess = new QLabel();
    fCombo = new QComboBox();

    if(fResult < 1500)
        fLabelSuccess->setText("Utterances are similar");
    else
        fLabelSuccess->setText("Utterances are different");



    for(unsigned int k = 0 ; k < fUtt1.size2() ; k++)
    {
        fCombo->addItem(QString("Mfcc ") + QString::number(k));
    }

    QObject::connect(fCombo,
                     SIGNAL(activated(int)),
                     fMfccPlot,
                     SLOT(setIndex(int)));

    fLayout2->addWidget(fLabel, 0, 0);
    fLayout2->addWidget(fLabelSuccess, 0, 1);
    fLayout2->addWidget(fCombo, 0, 2);
    fLayout2->addWidget(fMfccPlot, 1, 0, 1, 3);
    fMfccWin->setLayout(fLayout2);
    fMfccWin->resize(1500,800);
    fMfccWin->show();
}
