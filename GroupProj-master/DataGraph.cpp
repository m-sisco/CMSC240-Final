/* Rachel Culpepper */

#include "DataGraph.h"
#include <QtGui>
#include <sstream>
using namespace std;

DataGraph::DataGraph( QWidget* parent, FileData* data, SaccadeList* saccades,
					int eye, int dimension )
	: QDialog( parent )
{
	/* create the dialog */

	layout = new QVBoxLayout;

	// title for the graphs; depends on selected eye and dimension
	stringstream graphTitle;


	if ( eye == leftEye )
	{
		graphTitle << "<b>Left eye, ";
	}
	else if ( eye == rightEye )
	{
		graphTitle << "<b>Right eye, ";
	}

	if ( dimension == xDim )
	{
		graphTitle << "x-coordinate</b>";
	}
	else if ( dimension == yDim )
	{
		graphTitle << "y-coordinate</b>";
	}


	QLabel* titleLabel = new QLabel( graphTitle.str().c_str() );
	titleLabel->setFont( QFont( "Helvetica", 16, QFont::Bold ) );
	titleLabel->setAlignment( Qt::AlignHCenter );
	titleLabel->setFixedHeight( 30 );

	layout->addWidget( titleLabel );

	this->setLayout( layout );

	this->setMinimumSize( 500, 500 );


	/* create the graphs */


	velocityGraph = new QCustomPlot;
	positionGraph = new QCustomPlot;


	/* create position graph */

	int numDataEntries = data->getData().size();

	// data used to create the position graph
	QVector<double> eyePos( numDataEntries );
	QVector<double> eyeTime( numDataEntries );

	// data used to create the velocity graph
	// it just uses eyeTime for time, since the values are the same
	QVector<double> vel( numDataEntries );


	// to start the x-axis from 0
	double startTime = data->getTime(0);

	// get the data for the selected eye and dimension
	if ( eye == leftEye )
	{

		if ( dimension == xDim )
		{
			// left eye, x-coordinate

			// get the selected data
			for (int i = 0; i < numDataEntries; ++i)
			{
				eyePos[i] = data->getLx(i);

				// convert from milliseconds to seconds
				eyeTime[i] = ( data->getTime(i) - startTime ) / 1000.0;

				vel[i] = data->getLXVel(i);
			}
		}
		else if ( dimension == yDim )
		{
			// left eye, y-coordinate

			// get the selected data
			for (int i = 0; i < numDataEntries; ++i)
			{
				eyePos[i] = data->getLy(i);
				
				// convert from milliseconds to seconds
				eyeTime[i] = ( data->getTime(i) - startTime ) / 1000.0;

				vel[i] = data->getLYVel(i);
			}
		}
	}
	else if ( eye == rightEye )
	{
		// right, x-coordinate

		// get the selected data
		if ( dimension == xDim )
		{

			for (int i = 0; i < numDataEntries; ++i)
			{
				eyePos[i] = data->getRx(i);
				
				// convert from milliseconds to seconds
				eyeTime[i] = ( data->getTime(i) - startTime ) / 1000.0;

				vel[i] = data->getRXVel(i);
			}
		}
		else if ( dimension == yDim )
		{
			// right eye, y -coordinate

			// get the selected data
			for (int i = 0; i < numDataEntries; ++i)
			{
				eyePos[i] = data->getRy(i);
				
				// convert from milliseconds to seconds
				eyeTime[i] = ( data->getTime(i) - startTime ) / 1000.0;

				vel[i] = data->getRYVel(i);
			}
		}
	}


	int numTargetMovements = data->getMsgs().size();

	// get target data
	QVector<double> targetPos( numTargetMovements );
	QVector<double> targetTime( numTargetMovements );

	if ( dimension == xDim )
	{
		// x-coordinate
		for (int i = 0; i < numTargetMovements; ++i)
		{
			targetPos[i] = data->getXPos(i);

			// change time to be from the start and convert ms->sec
			targetTime[i] = ( data->getMsgTime(i) - startTime ) / 1000.0;
		}
	}
	else if ( dimension == yDim )
	{
		// y-coordinate
		for (int i = 0; i < numTargetMovements; ++i)
		{
			targetPos[i] = data->getYPos(i);
			
			// change time to be from the start and convert ms->sec
			targetTime[i] = ( data->getMsgTime(i) - startTime ) / 1000.0;
		}
	}


	this->createPositionGraph( eyeTime, eyePos, targetTime, targetPos );


	/* create velocity graph */

	// get the saccade data for the velocity graph

	QVector<double> saccadeVelocities( saccades->size() );
	QVector<double> saccadeTimes( saccades->size() );

	for (int i = 0; i < saccades->size(); ++i)
	{
		saccadeVelocities[i] = saccades->getSaccade(i).getPeakVelocity();
		saccadeTimes[i] = saccades->getSaccade(i).getPeakVelocityTime();
	}

	this->createVelocityGraph( eyeTime, vel, saccadeTimes, saccadeVelocities );
}


DataGraph::~DataGraph()
{
	delete positionGraph;
	delete velocityGraph;
}



void DataGraph::createPositionGraph( QVector<double> xEye, QVector<double> yEye,
	QVector<double> xTarget, QVector<double> yTarget )
{
	positionGraph->xAxis->setLabel( "time (sec)" );
	positionGraph->yAxis->setLabel( "degrees" );

	// create new graphs with the data
	positionGraph->addGraph();
	positionGraph->graph(0)->setData( xEye, yEye );
	positionGraph->graph(0)->setPen( QPen( Qt::blue ) );

	positionGraph->addGraph();
	positionGraph->graph(1)->setData( xTarget, yTarget );
	positionGraph->graph(1)->setPen( QPen( Qt::green ) );

	// scale the axes so that the entire graphs are shown
	positionGraph->rescaleAxes();

	// add legend
	positionGraph->legend->setVisible( true );
	positionGraph->graph(0)->setName( "Eye position" );
	positionGraph->graph(1)->setName( "Target position" );

	// add title at the top
	positionGraph->plotLayout()->insertRow(0);
	positionGraph->plotLayout()->addElement( 0, 0, 
		new QCPTextElement( positionGraph, "Target and Eye Positions",
		QFont( "Helvetica", 12, QFont::Bold ) ) );


	// show the graph
	positionGraph->replot();
	layout->addWidget( positionGraph );
}


void DataGraph::createVelocityGraph( QVector<double> time, 
	QVector<double> velocity, QVector<double> xSaccade, QVector<double> ySaccade )
{
	velocityGraph->xAxis->setLabel( "time (sec)" );
	velocityGraph->yAxis->setLabel( "degrees/sec" );

	// graph of velocity data
	velocityGraph->addGraph();
	velocityGraph->graph(0)->setData( time, velocity );

	// plot the saccades
	velocityGraph->addGraph();
	velocityGraph->graph(1)->setData( xSaccade, ySaccade );
	velocityGraph->graph(1)->setPen( QPen( Qt::green ) );

	// using disconnected circles instead of a line graph
	velocityGraph->graph(1)->setLineStyle( QCPGraph::lsNone );
	velocityGraph->graph(1)->setScatterStyle( QCPScatterStyle::ssCircle );


	// add legend
	velocityGraph->legend->setVisible( true );
	velocityGraph->graph(0)->setName( "Velocity" );
	velocityGraph->graph(1)->setName( "Saccades" );

	// add title
	velocityGraph->plotLayout()->insertRow(0);
	velocityGraph->plotLayout()->addElement( 0, 0, 
		new QCPTextElement( velocityGraph, "Velocity and detected saccades",
		QFont( "Helvetica", 12, QFont::Bold ) ) );

	// show the graph
	velocityGraph->rescaleAxes();
	layout->addWidget( velocityGraph );
}


void DataGraph::showGraphDialog()
{
	this->show();
}