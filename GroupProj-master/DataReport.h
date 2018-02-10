/* This class is used to generate and display the dialog box data report of a given data file. This dialog box 
will include the name of the data file, the number of primary saccades, the number of corrective saccades, the number of target movements, 
the value of the measure of how well the gaze track matched the target track for this subject, and the following information for each 
saccade: the saccade number, the saccade type (primary or corrective), the peak velocity time (in seconds), the time of onset of 
the saccade (in seconds), the time of completion of the saccade (in seconds), the amplitude of the saccade, the peak 
velocity (in degrees per second), and the duration of the saccade (in milliseconds). */

#ifndef DATAREPORT_H
#define DATAREPORT_H

#include <QApplication>
#include <QLabel>
#include <QDialog>
#include <string>
#include "Saccade.h"

using namespace std;

class QLabel;
class QComboBox;
class QLineEdit;
class QPushButton;
class QTabWidget;
class QDialogButtonBox;

class DataReport : public QDialog
{

	Q_OBJECT

private:

	bool test;

	QTabWidget* display;
	string fileName;
	int numPrimary;
	int numCorrective;
	int numTargetMovements;
	double matchValue;
	QLabel* fileLabel;
	QLabel* numPrimaryLabel;
	QLabel* numCorrectiveLabel;
	QLabel* numTargetMovementsLabel;
	QLabel* matchValueLabel;
	QLabel* saccadeLabel;
	QLabel* typeLabel;
	QLabel* peakVelocityTimeLabel;
	QLabel* startTimeLabel;
	QLabel* endTimeLabel;
	QLabel* amplitudeLabel;
	QLabel* peakVelocityLabel;
	QLabel* durationLabel;
	QPushButton* saveAsFileButton;
	vector<Saccade> saccades;

	private slots:

		/* Slot for when the "Save as file" button is clicked */
		void saveAsFileClicked();


public:

	/* Constructor will call parent class constructor to create a child object (DataReport) of the parent object (QDialog) */
	DataReport(QWidget *parent, string fileNameIn, vector<Saccade> saccadesIn, int numTargetMovementsIn);

	/* Displays the dialog box containing the above information for the corresponding data file read in */
	void displayDialogBox();

	/* Generates and formats the information for the current Saccade object into the proper format to be displayed in the dialogue box */
	void formatSaccadeInfo();

	/* Destructor */
	~DataReport();

};

#endif

