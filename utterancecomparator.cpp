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

#include <math.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <exception>
#include "utterancecomparator.h"

void UtteranceComparator::compareUtterances(boost::filesystem::path pathUtt1,
                                            boost::filesystem::path pathUtt2,
                                            ublas::matrix<float> & utt1,
                                            ublas::matrix<float> & utt2,
                                            std::vector< ublas::matrix<unsigned int> > & minPath,
                                            ublas::matrix<float> & dtw,
                                            float & score)
{
    loadFromFile(pathUtt1, utt1);
    loadFromFile(pathUtt2, utt2);
    computeDTW(utt1, utt2, dtw, score);
    computeMinPath(dtw, minPath);
}


// Explanations about the DTW algorithm can be found here
// (http://en.wikipedia.org/wiki/Dynamic_time_warping)
void UtteranceComparator::computeDTW(ublas::matrix<float> & utt1,
                                     ublas::matrix<float> & utt2,
                                     ublas::matrix<float> & dtw,
                                     float & score)
{
    if(utt1.size2() != utt2.size2())
    {
        std::stringstream errMsg;
        errMsg << "computeDTW: Utt1 features vector size is " << utt1.size2()
               << " while Utt2 features vector size is " << utt2.size2();
        throw std::runtime_error(errMsg.str().c_str());
    }

    // Initializing the dtw matrix
    dtw.resize(utt1.size1()+1, utt2.size1()+1);

    for (unsigned int i=1; i <= utt1.size1(); i++)
    {
        dtw(i,0) = 1000000;
    }
    for (unsigned int i=1; i <= utt2.size1(); i++)
    {
        dtw(0,i) = 1000000;
    }
    dtw(0,0) = 0;

    // Computing from each cell of the matrix a consolidated cost
    for (unsigned int i=1; i<=utt1.size1(); i++)
    {
        for (unsigned int j=1; j<=utt2.size1(); j++)
        {
            float cost = 0;
            for(unsigned int k = 0 ; k < utt1.size2() ; ++k)
            {
                cost += std::fabs(utt2(j-1,k) - utt1(i-1,k))
                        *std::fabs(utt2(j-1,k) - utt1(i-1,k));
            }

            dtw(i,j) = cost + min(dtw(i-1,j),
                                  dtw(i,j-1),
                                  dtw(i-1,j-1));
        }
    }

    // The global match of utt1 and utt2 is correlated to this value
    score = dtw(utt1.size1(), utt2.size1());
}

// To retrieve the optimal sample to sample match between the 2 utterances
// we compute the sequence of cells in the dtw matrix that sums
// up to the lowest cost from the last cell dtw(utt1.size1(), utt2.size1())
void UtteranceComparator::computeMinPath(ublas::matrix<float> & dtw,
                                         std::vector<ublas::matrix<unsigned int> > &path)
{
    int i = dtw.size1()-1;
    int j = dtw.size2()-1;

    ublas::matrix<unsigned int> p(1,2);

    while(i != 0 && j != 0)
    {
        switch(argmin(dtw(i-1,j), dtw(i,j-1), dtw(i-1,j-1)))
        {
        case 1:
            p(0,0) = i-1;
            p(0,1) = j;
            path.push_back(p);
            i--;
            break;
        case 2:
            p(0,0) = i;
            p(0,1) = j-1;
            path.push_back(p);
            j--;
            break;
        case 3:
            p(0,0) = i-1;
            p(0,1) = j-1;
            path.push_back(p);
            i--;
            j--;
            break;
        default:
            throw std::runtime_error("computeMinPath: "
                                     "argmin has unexpected outputs");
        }
    }
}

// A greedy way to check that the file is well formated (all lines have same
// size as first line) and to compute
// the size of the features vectors & the number of vectors per files.
// Better implementations do exist!
void UtteranceComparator::checkFile(bf::path path1,
                                    unsigned int & nbOfRow,
                                    unsigned int & nbOfColumn)
{
    std::string line;
    nbOfRow = 0;
    nbOfColumn = 0;
    bool isFirstLineChecked = false;
    std::ifstream myfile(path1.c_str());
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            ++nbOfRow;
            std::istringstream iss (line);
            std::string value;
            unsigned int j = 0;
            while (getline(iss, value, ';'))
                j++;

            if(isFirstLineChecked)
            {
                if(j != nbOfColumn)
                {
                    std::stringstream errMsg;
                    errMsg << "checkFile: A vector of size " << j
                           << " has been found at line " << nbOfRow
                           << " whereas the first vector of the file is of size "
                           << nbOfColumn;
                    throw std::runtime_error(errMsg.str().c_str());
                }
            }
            else
            {
                nbOfColumn = j;
                isFirstLineChecked = true;
            }
        }
    }
    else
    {
        std::stringstream errMsg;
        errMsg << "checkFile: Could not open input file '" << path1.string() << "'";
        throw std::runtime_error(errMsg.str().c_str());
    }
    myfile.close();

    std::cout << nbOfRow << " vectors of size " << nbOfColumn
              << " have been detected in file "
              << path1.string()
              << std::endl;
}


void UtteranceComparator::loadFromFile(bf::path path1,
                                       ublas::matrix<float> & features)
{
    unsigned int nbOfRow, nbOfColumn;
    checkFile(path1, nbOfRow, nbOfColumn);

    features.resize(nbOfRow, nbOfColumn);

    std::string line;
    std::ifstream myfile1(path1.string().c_str());
    if (myfile1.is_open())
    {
        unsigned int i = 0;
        unsigned int j = 0;
        while (std::getline(myfile1, line))
        {
            std::istringstream iss (line);
            std::string value;
            j = 0;
            while (getline(iss, value, ';'))
            {
                features(i,j) = std::atof(value.c_str());
                j++;
            }
            i++;
        }
    }
    else
    {
        std::stringstream errMsg;
        errMsg << "loadFromFile: Could not open input file '" << path1.string() << "'";
        throw std::runtime_error(errMsg.str().c_str());
    }
    myfile1.close();
}

float UtteranceComparator::min(float x,
                               float y,
                               float z)
{
    if( ( x <= y ) && ( x <= z ) ) return x;
    if( ( y <= x ) && ( y <= z ) ) return y;
    if( ( z <= x ) && ( z <= y ) ) return z;
}

int UtteranceComparator::argmin(float x,
                                float y,
                                float z)
{
    if( ( x <= y ) && ( x <= z ) ) return 1;
    if( ( y <= x ) && ( y <= z ) ) return 2;
    if( ( z <= x ) && ( z <= y ) ) return 3;
    return -1;
}


