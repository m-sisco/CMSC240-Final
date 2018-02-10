#include "SaccadeList.h"

SaccadeList::SaccadeList( FileData* data, int eye, int dimension )
{
	int numDataEntries = data->getData().size();

	vector<double> velocities;
	vector<double> positions;

	// retrieve velocity and position values for the selected eye and dimension

	if ( eye == leftEye )
	{
		if ( dimension == xDim )
		{
			// left eye, x 
			for (int i = 0; i < numDataEntries; ++i)
			{
				velocities.push_back( data->getLXVel(i) );
				positions.push_back( data->getLx(i) );
			}
		}
		else if ( dimension == yDim )
		{
			// left eye, y
			for (int i = 0; i < numDataEntries; ++i)
			{
				velocities.push_back( data->getLYVel(i) );
				positions.push_back( data->getLy(i) );
			}
		}
	}
	else if ( eye == rightEye )
	{
		if ( dimension == xDim )
		{
			// right eye, x
			for (int i = 0; i < numDataEntries; ++i)
			{
				velocities.push_back( data->getRXVel(i) );
				positions.push_back( data->getRx(i) );
			}
		}
		else if ( dimension == yDim )
		{
			// right eye, y
			for (int i = 0; i < numDataEntries; ++i)
			{
				velocities.push_back( data->getRYVel(i) );
				positions.push_back( data->getRy(i) );
			}
		}
	}



	// iterate through the data entries and use velocity to determine saccades

	// keep start time of first entry to use the time since the start
	double overallStart = data->getTime( 0 );

	for (int i = 0; i < velocities.size(); ++i)
	{
		// identify places where there isn't a fixation--
		// where the velocity is > 15 or < -15

		// separates saccades with positive and negative velocity
		// so that a movement that goes from positive velocity to
		// negative velocity doesn't get classified as one saccade
		if ( velocities[i] > 15.0 )
		{
			// this marks the beginning of the saccade
			double saccadeStart = data->getTime(i) - overallStart;

			double peakVelocity = velocities[i];
			double peakVelocityTime = saccadeStart;

			// find the end of the saccade, keeping track of peak velocity
			int j = i + 1;
			while( velocities[j] > 15.0 )
			{
				if ( velocities[j] > peakVelocity )
				{
					peakVelocity = velocities[j];
					peakVelocityTime = data->getTime(j) - overallStart;
				}

				++j;
			}

			// get the information and add the saccade
			double amplitude = positions[j - 1] - positions[i];
			double saccadeEnd = data->getTime(j - 1) - overallStart;


			Saccade newSaccade( saccadeStart, saccadeEnd, peakVelocity, 
				peakVelocityTime, amplitude );

			saccades.push_back( newSaccade );

			// skip to j, since everything in between has been processed
			i = j;	
		}
		else if ( velocities[i] < -15.0 )
		{
			// this marks the beginning of the saccade
			double saccadeStart = data->getTime(i) - overallStart;

			double peakVelocity = velocities[i];
			double peakVelocityTime = saccadeStart;

			// find the end of the saccade, keeping track of peak velocity
			int j = i + 1;
			while( velocities[j] < -15.0 )
			{
				if ( velocities[j] < peakVelocity )
				{
					peakVelocity = velocities[j];
					peakVelocityTime = data->getTime(j) - overallStart;
				}

				++j;
			}

			// get the information and add the saccade
			double amplitude = positions[j - 1] - positions[i];
			double saccadeEnd = data->getTime(j - 1) - overallStart;

			Saccade newSaccade( saccadeStart, saccadeEnd, peakVelocity, 
				peakVelocityTime, amplitude );

			saccades.push_back( newSaccade );

			// skip to j, since everything in between has been processed
			i = j;	
		}
	}

	// determine the type of each saccade
	this->classifySaccades();
}



Saccade SaccadeList::getSaccade( int index )
{
	return saccades[ index ];
}


int SaccadeList::size()
{
	return (int) saccades.size();
}


vector<Saccade> SaccadeList::toVector()
{
	int numSaccades = saccades.size();

	vector<Saccade> returnVector;

	for (int i = 0; i < numSaccades; ++i)
	{
		Saccade copy( saccades[i] );

		returnVector.push_back( copy );
	}


	return returnVector;
}



void SaccadeList::classifySaccades()
{
	// calculate the average amplitude of the saccades

	double avg = 0;

	for (int i = 0; i < saccades.size(); ++i)
	{
		avg += saccades[i].getAmplitude();
	}

	avg = avg / saccades.size();

	// classify each saccade
	// based on whether it's amplitude is above or below average
	for (int i = 0; i < saccades.size(); ++i)
	{
		if ( saccades[i].getAmplitude() >= avg )
		{
			saccades[i].setType( 1 );
		}
		else if ( saccades[i].getAmplitude() < avg )
		{
			saccades[i].setType( 2 );
		}
	}
}