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
// Compute a dtw (dynamic time wrapping) based similarity score between 2 utterances
// represented by 2 sets of mfcc features. These features can be computed
// by Kaldi's "compute-mfcc" tool (http://kaldi.sourceforge.net).
// The mfcc input files should be formated as follows:
//
// mfcc0_t0, mfcc1_t0, mfcc2_t0, ..., mfccN_t0
// mfcc0_t1, mfcc1_t1, mfcc2_t1, ..., mfccN_t1
// ...
// mfcc0_tM, mfcc1_tM, mfcc2_tM, ..., mfccN_tM
//
// with N the number components of the mfcc vector
// and M the number of mfcc vectors contained in the file


#ifndef UTTERANCECOMPARATOR_H
#define UTTERANCECOMPARATOR_H

#include <string>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/filesystem.hpp>

namespace ublas = boost::numeric::ublas;
namespace bf = boost::filesystem;

class UtteranceComparator
{    

public:
    UtteranceComparator(){}
    ~UtteranceComparator(){}

    // Compute DTW between 2 sets of mfcc features provided as inputs
    //
    // Inputs: "pathUtt1" and "pathUtt2" formated as explained above
    //
    // Outputs: "score" between 0 (same utterances) and +inf
    //          "utt1" and "utt2" store features vectors
    //          "minPath" stores the path of minimum cost in the dtw matrix
    //          "dtw" stores the computed dtw matrix
    //
    // All outputs objects should be allocated by the caller.
    void compareUtterances(bf::path pathUtt1,
                           bf::path pathUtt2,
                           ublas::matrix<float> & utt1,
                           ublas::matrix<float> & utt2,
                           std::vector< ublas::matrix<unsigned int> > & minPath,
                           ublas::matrix<float> & dtw,
                           float & score);

private:

    // Compute the "dtw" matrix and a similarity "score" from the 2 input
    // utterances "utt1" and "utt2"
    void computeDTW(ublas::matrix<float> & utt1,
                    ublas::matrix<float> & utt2,
                    ublas::matrix<float> & dtw,
                    float & score);

    // Extract from the "dtw" matrix the path (the match) that minimize the cost
    void computeMinPath(ublas::matrix<float> & dtw,
                        std::vector< ublas::matrix<unsigned int> > & path);

    // Read the features in the "path" file and stores them in the matrix "features"
    void loadFromFile(bf::path path,
                      ublas::matrix<float> & features);

    // Check that the file "path1" is formated as explained above
    void checkFile(bf::path path1,
                   unsigned int & nbOfRow,
                   unsigned int & nbOfColumn);

    // Simple but useful
    float min(float a, float b, float c);
    int argmin(float x, float y, float z);
};

#endif // UTTERANCECOMPARATOR_H
