/*
 * This class is used to store and retrieve the information for each saccade
 * in the data, including its amplitude, duration, start and end times, 
 * velocity, and its classification (as a primary or corrective saccade).
 * The class contains methods to retrieve all of the information about a 
 * saccade for generating a report for the data.
 */

#ifndef SACCADE 
#define SACCADE

class Saccade
{
	private:

		// milliseconds
		double startTime;
		double endTime;
		double peakVelocityTime;

		// degrees/sec
		double peakVelocity;
		
		// degrees
		double amplitude;

		// 1 for primary, 2 for corrective
		int type;

	public:
		
		/*
		 * Constructor; initializes instance variables with the provided values
		 */
		Saccade( double _startTime, double _endTime, double _peakVelocity,
				double _peakVelocityTime, double _amplitude );

		/*
		 * Copy constructor. Creates a Saccade instance with all of the same 
		 * data as the given Saccade.
		 */
		Saccade( const Saccade& otherSaccade );

		/*
		 * Returns the amplitude of the saccade (in degrees).
		 */
		double getAmplitude() const;

		/*
		 * Returns the duration of the saccade in milliseconds.
		 */
		double getDuration() const;

		/*
		 * Returns the end time of the saccade in seconds.
		 */
		double getEndTime() const;

		/*
		 * Returns the peak velocity of the saccade in degrees/second.
		 */
		double getPeakVelocity() const;

		/*
		 * Returns the time at which the peak velocity of the saccade
		 * occurred in seconds.
		 */
		double getPeakVelocityTime() const;

		/*
		 * Returns the start time of the saccade in seconds.
		 */
		double getStartTime() const;

		/*
		 * Returns the type of saccade. A 1 denotes a primary saccade, and a
		 * 2 denotes a smaller saccade. A -1 indicates that the saccade has not
		 * been classified.
		 */
		int getType() const;

		/*
		 * Sets the type of the saccade; 1 indicates a primary saccade, and 2
		 * indicates a secondary saccade.
		 */
		void setType( int _type );
};

#endif