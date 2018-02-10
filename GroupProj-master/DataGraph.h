/*
 * This class creates a dialog that displays graphs for eye and velocity data.
 */

#ifndef DATAGRAPH 
#define DATAGRAPH

#include <QDialog>
#include <QVector>
#include <QVBoxLayout>
#include "qcustomplot/qcustomplot.h"
#include "FileData.h"
#include "SaccadeList.h"
#include "option.h"
#include <string>
using namespace std;

class QPushButton;

class DataGraph : QDialog
{
	Q_OBJECT

	public:

		/*
		 * Constructor. Takes an instance of FileData, a selected eye and 
		 * dimension, and a corresponding list of saccades. Creates graphs for
		 * the position of the eye in the chosen dimension and for the eye's 
		 * velocity, with saccades marking peak velocities.
		 * A 0 corresponds to the left eye, and a 1 to the right eye. A 3 
		 * corresponds to the x-coordinates, and a 4 to the y-coordinates.
		 */
		DataGraph( QWidget* parent, FileData* data, SaccadeList* saccades,
			int eye, int dimension );

		/*
		 * Destructor
		 */
		~DataGraph();

		/*
		 * Makes the dialog visible.
		 */
		void showGraphDialog();

	private:

		QCustomPlot* positionGraph;
		QCustomPlot* velocityGraph;

		/* layout of the dialog */
		QVBoxLayout* layout;

		/*
		 * Takes in vectors of x- and y-coordinates for the eye position and the
		 * target position, with x in seconds and y in degrees.
		 * Plots the positions of the eye and the target over time. The given
		 * string is used as the title of the graph.
		 */
		void createPositionGraph( QVector<double> xEye, QVector<double> yEye,
			QVector<double> xTarget, QVector<double> yTarget );

		/*
		 * Plots the velocity of the eye over time 
		 * and shows the detected saccades.
		 */
		void createVelocityGraph( QVector<double> time, QVector<double> velocity,
			QVector<double> xSaccade, QVector<double> ySaccade );
};

#endif