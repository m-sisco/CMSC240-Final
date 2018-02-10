/* This class is used to generate and display the dialog box data report of a given data file. This dialog box 
will include the name of the data file, the number of primary saccades, the number of corrective saccades, the number of target movements, 
the value of the measure of how well the gaze track matched the target track for this subject, and the following information for each 
saccade: the saccade number, the saccade type (primary or corrective), the peak velocity time (in seconds), the time of onset of 
the saccade (in seconds), the time of completion of the saccade (in seconds), the amplitude of the saccade, the peak 
velocity (in degrees per second), and the duration of the saccade (in milliseconds). */

#include <QtGui>
#include "DataReport.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;


/* Constructor will call parent class constructor to create a child object (DataReport) of the parent object (QDialog) */
DataReport::DataReport(QWidget *parent, string fileNameIn, vector<Saccade> saccadesIn, int numTargetMovementsIn) : QDialog(parent)
{

	test = false;

	display = new QTabWidget();
	display->setFixedSize(600, 400);

	QVBoxLayout *layout = new QVBoxLayout();
	display->setLayout(layout);
	display->setWindowTitle(tr("Data Report"));

	saveAsFileButton = new QPushButton(tr("Save as file"));
	saveAsFileButton->setEnabled(true);
	saveAsFileButton->setFixedSize(100, 25);
	
	QWidget *generalInfo = new QWidget();
	generalInfo->setLayout(layout);
	display->addTab(generalInfo, *(new QString("General File Info")));

	fileName = fileNameIn;
	saccades = saccadesIn;
	numPrimary = 0;
	numCorrective = 0;

	formatSaccadeInfo();

	string file = "<b>Input File:</b> ";
	file = file + (fileName).c_str();
	fileLabel = new QLabel();
	fileLabel->setText(QString((file).c_str()));
	fileLabel->setFixedHeight(25);
	layout->addWidget(fileLabel);

	if (test) numPrimary = 0;
	string primary = "<b>Number of Primary Saccades:</b> ";
	primary = primary + (to_string(numPrimary)).c_str();
	numPrimaryLabel = new QLabel();
	numPrimaryLabel->setText(QString((primary).c_str()));
	numPrimaryLabel->setFixedHeight(25);
	layout->addWidget(numPrimaryLabel);

	if (test) numCorrective = 0;
	string corrective = "<b>Number of Corrective Saccades:</b> ";
	corrective = corrective + (to_string(numCorrective)).c_str();
	numCorrectiveLabel = new QLabel();
	numCorrectiveLabel->setText(QString((corrective).c_str()));
	numCorrectiveLabel->setFixedHeight(25);
	layout->addWidget(numCorrectiveLabel);

	if (test) numTargetMovements = 0;
	numTargetMovements = numTargetMovementsIn;
	string target = "<b>Number of Target Movements:</b> ";
	target = target + (to_string(numTargetMovements)).c_str();
	numTargetMovementsLabel = new QLabel();
	numTargetMovementsLabel->setText(QString((target).c_str()));
	numTargetMovementsLabel->setFixedHeight(25);
	layout->addWidget(numTargetMovementsLabel);

	if (test) matchValue = 0;
	matchValue = ((double) numCorrective) / ((double) numTargetMovements);
	string match = "<b>Match Value:</b> ";
	match = match + (to_string(matchValue)).c_str();
	matchValueLabel = new QLabel();
	matchValueLabel->setText(QString((match).c_str()));
	matchValueLabel->setFixedHeight(25);
	layout->addWidget(matchValueLabel);
	layout->setAlignment(matchValueLabel, Qt::AlignTop);

	// When the "Save as file" button is clicked, saveAsFileClicked() is called
	QObject::connect(saveAsFileButton, SIGNAL(clicked()), this, SLOT(saveAsFileClicked()));

	layout->addWidget(saveAsFileButton);
	layout->setAlignment(saveAsFileButton, Qt::AlignRight);


}

/* Displays the dialog box containing the above information for the corresponding data file read in */
void DataReport::displayDialogBox()
{

	this->display->show();

}

/* Generates and formats the information for the current Saccade object into the proper format to be displayed in the dialogue box */
void DataReport::formatSaccadeInfo()
{
		
	QVBoxLayout *saccadeLayout;
	QWidget *tempSaccade;

	for (int i=0; i < saccades.size(); i++)
	{
		saccadeLayout = new QVBoxLayout();
		tempSaccade = new QWidget();
		tempSaccade->setLayout(saccadeLayout);
		string tempString = "Saccade ";
		tempString = tempString + to_string(i+1).c_str();
		display->addTab(tempSaccade, *(new QString(tempString.c_str())));

		saccadeLabel = new QLabel();
		tempString = "<b>" + tempString + "</b>";
		saccadeLabel->setText(QString((tempString).c_str()));
		saccadeLabel->setFixedHeight(25);
		saccadeLayout->addWidget(saccadeLabel);
		
		string type = "<b>Type:</b> ";
		if (test) type = type + (to_string(i+1)).c_str();
		{	
			if (saccades[i].getType()==1)
			{
				numPrimary++;
				type = type + "primary";
			}
			if (saccades[i].getType()==2)
			{
				numCorrective++;
				type = type + "secondary";
			}
		}
		typeLabel = new QLabel();
		typeLabel->setText(QString((type).c_str()));
		typeLabel->setFixedHeight(25);
		saccadeLayout->addWidget(typeLabel);

		string peakVelocityTime = "<b>Peak Velocity Time:</b> ";
		if (test) peakVelocityTime = peakVelocityTime + (to_string(i+1)).c_str();
		peakVelocityTime = peakVelocityTime + (to_string(saccades[i].getPeakVelocityTime())).c_str();
		peakVelocityTimeLabel = new QLabel();
		peakVelocityTimeLabel->setText(QString((peakVelocityTime).c_str()));
		peakVelocityTimeLabel->setFixedHeight(25);
		saccadeLayout->addWidget(peakVelocityTimeLabel);

		string peakVelocity = "<b>Peak Velocity:</b> ";
		if (test) peakVelocity = peakVelocity + (to_string(i+1)).c_str();
		peakVelocity = peakVelocity + (to_string(saccades[i].getPeakVelocity())).c_str();
		peakVelocityLabel = new QLabel();
		peakVelocityLabel->setText(QString((peakVelocity).c_str()));
		peakVelocityLabel->setFixedHeight(25);
		saccadeLayout->addWidget(peakVelocityLabel);

		string amplitude = "<b>Amplitude:</b> ";
		if (test) amplitude = amplitude + (to_string(i+1)).c_str();
		amplitude = amplitude + (to_string(saccades[i].getAmplitude())).c_str();
		amplitudeLabel = new QLabel();
		amplitudeLabel->setText(QString((amplitude).c_str()));
		amplitudeLabel->setFixedHeight(25);
		saccadeLayout->addWidget(amplitudeLabel);

		string startTime = "<b>Start Time:</b> ";
		if (test) startTime = startTime + (to_string(i+1)).c_str();
		startTime = startTime + (to_string(saccades[i].getStartTime())).c_str();
		startTimeLabel = new QLabel();
		startTimeLabel->setText(QString((startTime).c_str()));
		startTimeLabel->setFixedHeight(25);
		saccadeLayout->addWidget(startTimeLabel);
		
		string endTime = "<b>End Time:</b> ";
		if (test) endTime = endTime + (to_string(i+1)).c_str();
		endTime = endTime + (to_string(saccades[i].getEndTime())).c_str();
		endTimeLabel = new QLabel();
		endTimeLabel->setText(QString((endTime).c_str()));
		endTimeLabel->setFixedHeight(25);
		saccadeLayout->addWidget(endTimeLabel);

		string duration = "<b>Duration:</b> ";
		if (test) duration = duration + (to_string(i+1)).c_str();
		duration = duration + (to_string(saccades[i].getDuration())).c_str();
		durationLabel = new QLabel();
		durationLabel->setText(QString((duration).c_str()));
		durationLabel->setFixedHeight(25);
		saccadeLayout->addWidget(durationLabel);
		saccadeLayout->setAlignment(durationLabel, Qt::AlignTop);


	}

}

DataReport::~DataReport()
{
}


/* When the "Save as file" button is clicked, save the data to a file */
void DataReport::saveAsFileClicked()
{
	
	ofstream output("Output_File", fstream::out | fstream::trunc);

	string file = "Input File: ";
	file = file + (fileName).c_str();
	output << file << endl;

	if (test) numPrimary = 0;
	string primary = "Number of Primary Saccades: ";
	primary = primary + (to_string(numPrimary)).c_str();
	output << primary << endl;

	if (test) numCorrective = 0;
	string corrective = "Number of Corrective Saccades: ";
	corrective = corrective + (to_string(numCorrective)).c_str();
	output << corrective << endl;

	if (test) numTargetMovements = 0;
	string target = "Number of Target Movements: ";
	target = target + (to_string(numTargetMovements)).c_str();
	output << target << endl;

	if (test) matchValue = 0;
	matchValue = numCorrective/numTargetMovements;
	string match = "Match Value: ";
	match = match + (to_string(matchValue)).c_str();
	output << match << endl;


	for (int i=0; i < saccades.size(); i++)
	{
		output << "\n" << endl;

		string tempString = "Saccade ";
		tempString = tempString + to_string(i+1).c_str();
		tempString = tempString + ":";
		output << tempString << endl;

		string type = "Type: ";
		if (test) type = type + (to_string(i+1)).c_str();
		type = type + (to_string(saccades[i].getType())).c_str();
		output << type << endl;

		string peakVelocityTime = "Peak Velocity Time: ";
		if (test) peakVelocityTime = peakVelocityTime + (to_string(i+1)).c_str();
		peakVelocityTime = peakVelocityTime + (to_string(saccades[i].getPeakVelocityTime())).c_str();
		output << peakVelocityTime << endl;

		string peakVelocity = "Peak Velocity: ";
		if (test) peakVelocity = peakVelocity + (to_string(i+1)).c_str();
		peakVelocity = peakVelocity + (to_string(saccades[i].getPeakVelocity())).c_str();
		output << peakVelocity << endl;

		string amplitude = "Amplitude: ";
		if (test) amplitude = amplitude + (to_string(i+1)).c_str();
		amplitude = amplitude + (to_string(saccades[i].getAmplitude())).c_str();
		output << amplitude << endl;

		string startTime = "Start Time: ";
		if (test) startTime = startTime + (to_string(i+1)).c_str();
		startTime = startTime + (to_string(saccades[i].getStartTime())).c_str();
		output << startTime << endl;
		
		string endTime = "End Time: ";
		if (test) endTime = endTime + (to_string(i+1)).c_str();
		endTime = endTime + (to_string(saccades[i].getEndTime())).c_str();
		output << endTime << endl;

		string duration = "Duration: ";
		if (test) duration = duration + (to_string(i+1)).c_str();
		duration = duration + (to_string(saccades[i].getDuration())).c_str();
		output << duration << endl;

	}

	output.close();


}

