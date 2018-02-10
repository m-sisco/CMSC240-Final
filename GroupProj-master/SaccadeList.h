/*
 * This class is used to identify and store the saccades of a given eye and
 * dimension of a data file.
 */

#ifndef SACCADELIST 
#define SACCADELIST

#include "Saccade.h"
#include "FileData.h"
#include "option.h"
#include <vector>
using namespace std;

class SaccadeList
{
	public:

		/*
		 * Constructor. Takes an instance of FileData, and integers representing
		 * a selected eye and dimension. Identifies and stores the saccades in
		 * the data of the given eye and dimension. 0 indicates the left eye,
		 * and 1 the right. 2 indicates the x-coordinates and 3 y-coordinates.
		 */
		SaccadeList( FileData* data, int eye, int dimension );

		/*
		 * Takes the index of the saccade to be retrieved.
		 * Returns the saccade at index. Throws an exception if index is < 0
		 * or >= the number of saccades.
		 */
		Saccade getSaccade( int index );

		/*
		 * Returns the number of saccades.
		 */
		int size();

		/*
		 * Returns a std::vector containing all of the saccades in this list.
		 */
		vector<Saccade> toVector();


	private:

		vector<Saccade> saccades;

		/*
		 * Goes through the saccades in the list and determines which saccades
		 * are primary and which are corrective. A saccade is classified as 
		 * primary if its amplitude is above the average amplitude for all
		 * saccades. Otherwise, it is classified as corrective.
		 */
		void classifySaccades();
};

#endif