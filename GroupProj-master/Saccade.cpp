#include "Saccade.h"

Saccade::Saccade( double _startTime, double _endTime, double _peakVelocity,
				double _peakVelocityTime, double _amplitude )
{
	startTime = _startTime;
	endTime = _endTime;
	peakVelocity = _peakVelocity;
	peakVelocityTime = _peakVelocityTime;
	amplitude = _amplitude;

	// denotes that the saccade is unclassified
	type = -1;
}


Saccade::Saccade( const Saccade& otherSaccade )
{
	// sec to ms
	startTime = otherSaccade.getStartTime() * 1000;
	endTime = otherSaccade.getEndTime() * 1000;
	peakVelocityTime = otherSaccade.getPeakVelocityTime() * 1000;

	peakVelocity = otherSaccade.getPeakVelocity();
	amplitude = otherSaccade.getAmplitude();
	type = otherSaccade.getType();
}


double Saccade::getAmplitude() const
{
	return amplitude;
}


double Saccade::getDuration() const
{
	// convert from milliseconds to seconds	
	return ( endTime - startTime ) / 1000.0;
}


double Saccade::getEndTime() const
{
	// convert from milliseconds to seconds
	return endTime / 1000.0;
}


double Saccade::getPeakVelocity() const
{
	return peakVelocity;
}


double Saccade::getPeakVelocityTime() const
{
	// convert from milliseconds to seconds
	return peakVelocityTime / 1000.0;
}


double Saccade::getStartTime() const
{
	// convert from milliseconds to seconds
	return startTime / 1000.0;
}


int Saccade::getType() const
{
	return type;
}


void Saccade::setType( int _type )
{
	type = _type;
}
