/* Rachel Culpepper */

#include "GUI.h"
#include <QtGui>
#include "qcustomplot/qcustomplot.h"

GUI::GUI( QWidget* parent )
	: QMainWindow( parent )
{
	// set instance variables for analysis to null
	graph = 0;
	fileData = 0;
	saccades = 0;
	report = 0;


	/* display the name of open file */

	openFileLabel = new QLabel( "No file is open" );
	openFileLabel->setFixedHeight( 20 );


	/* menu */

	menu = new QMenuBar;

	fileMenu = new QMenu( "File" );

	// create and connect menu actions

	open = new QAction( "Open", this );
	connect( open, SIGNAL( triggered() ), this, SLOT( openDataFile() ) );

	close = new QAction( "Close", this );
	connect( close, SIGNAL( triggered() ), this, SLOT( closeCurrentFile() ) );

	// disabled until a file is opened
	close->setEnabled( false );

	quit = new QAction( "Quit", this );
	connect( quit, SIGNAL( triggered() ), this, SLOT( close() ) );

	fileMenu->addAction( open );
	fileMenu->addAction( close );
	fileMenu->addAction( quit );

	menu->addMenu( fileMenu );

	this->setMenuBar( menu );


	/* display options for creating graphs and report */


	// options for which dimension to use

	QVBoxLayout* dimensionLayout = new QVBoxLayout;

	QLabel* dimensionLabel = new QLabel( "Dimension" );

	QRadioButton* xButton = new QRadioButton( "x", dimensionLabel );
	QRadioButton* yButton = new QRadioButton( "y", dimensionLabel );

	// x is selected by default
	xButton->setChecked( true );


	dimensionButtons = new QButtonGroup;

	dimensionButtons->addButton( xButton );
	dimensionButtons->setId( xButton, xDim );

	dimensionButtons->addButton( yButton );
	dimensionButtons->setId( yButton, yDim );


	QHBoxLayout* dimensionOptions  = new QHBoxLayout;
	dimensionOptions->addWidget( xButton );
	dimensionOptions->addWidget( yButton );

	dimensionLayout->addWidget( dimensionLabel );
	dimensionLayout->addLayout( dimensionOptions );


	// options for which eye to use

	QVBoxLayout* eyeLayout = new QVBoxLayout;

	QLabel* eyeLabel = new QLabel( "Eye data" );

	QRadioButton* leftButton = new QRadioButton( "Left", eyeLabel );
	QRadioButton* rightButton = new QRadioButton( "Right", eyeLabel );

	// left is checked by default
	leftButton->setChecked( true );


	eyeButtons = new QButtonGroup;

	eyeButtons->addButton( leftButton );
	eyeButtons->setId( leftButton, leftEye );

	eyeButtons->addButton( rightButton );
	eyeButtons->setId( rightButton, rightEye );


	QHBoxLayout* eyeOptions = new QHBoxLayout;

	eyeOptions->addWidget( leftButton );
	eyeOptions->addWidget( rightButton );

	eyeLayout->addWidget( eyeLabel );
	eyeLayout->addLayout( eyeOptions );


	// display the options for the graph and report

	QVBoxLayout* optionsLayout = new QVBoxLayout;

	QLabel* optionsHeading = new QLabel( "<b>Graph and Report Options</b>" );
	optionsHeading->setWordWrap( true );


	graphButton = new QPushButton( "Generate graph" );
	connect( graphButton, SIGNAL( clicked() ), this, SLOT( displayGraph() ) );

	// disabled until a file is opened
	graphButton->setEnabled( false );


	reportButton = new QPushButton( "Generate data report" );
	connect( reportButton, SIGNAL( clicked() ), this, SLOT( displayReport() ) );

	// disabled until a file is opened
	reportButton->setEnabled( false );


	optionsLayout->addWidget( optionsHeading );
	optionsLayout->addLayout( dimensionLayout );
	optionsLayout->addLayout( eyeLayout );
	optionsLayout->addWidget( graphButton );
	optionsLayout->addWidget( reportButton );

	QLabel* options = new QLabel();
	options->setLayout( optionsLayout );
	options->setFixedSize( 180, 300 );

	
	/* add all components to the main window */

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget( openFileLabel );
	mainLayout->addWidget( options );

	QLabel* mainLabel = new QLabel;
	mainLabel->setLayout( mainLayout );

	this->setCentralWidget( mainLabel );
	this->setMinimumSize( 200, 400 );
}


GUI::~GUI()
{
	delete fileData;
	delete report;
	delete graph;
	delete saccades;
}



void GUI::closeCurrentFile()
{
	// reset the open file name
	openFileLabel->setText( "No file is open" );

	// reset the variables of the open file

	delete fileData;
	fileData = 0;

	if ( report != 0 )
	{
		delete report;
		report = 0;
	}

	if ( graph != 0 )
	{
		delete graph;
		graph = 0;
	}

	if ( saccades != 0 )
	{
		delete saccades;
		saccades = 0;
	}

	// disable buttons
	graphButton->setEnabled( false );
	reportButton->setEnabled( false );
	close->setEnabled( false );
}


void GUI::displayGraph()
{
	// delete any existing graph or saccade list
	if ( graph != 0 )
	{
		delete graph;
	}

	if ( saccades != 0 )
	{
		delete saccades;
	}


	// identify the saccades
	saccades = new SaccadeList( fileData, eyeButtons->checkedId(), 
		dimensionButtons->checkedId() );

	graph = new DataGraph( this, fileData, saccades, eyeButtons->checkedId(),
							dimensionButtons->checkedId() );


	graph->showGraphDialog();
}


void GUI::displayReport()
{
	// delete any existing report and saccade list
	if ( report != 0 )
	{
		delete report;
	}

	if ( saccades != 0 )
	{
		delete saccades;
	}

	// identify saccades for the chosen coordinates
	saccades = new SaccadeList( fileData, eyeButtons->checkedId(), 
		dimensionButtons->checkedId() );

	int numTargetMovements = fileData->getMsgs().size();

	// create and display report
	report = new DataReport( this, openFileLabel->text().toStdString(), 
		saccades->toVector(), numTargetMovements );


	report->displayDialogBox();
}


void GUI::openDataFile()
{
	// close any currently open file
	if ( fileData != 0 )
	{
		this->closeCurrentFile();
	}

	// create a dialog for the user to select a file
	QString filePath = QFileDialog::getOpenFileName( this, tr( "Open data file" ),
		"..", tr( "Data Files (*.asc)" ) );


	// if the user selected cancel
	if ( filePath == "" )
	{
		return;
	}


	fileData = new FileData();
	fileData->parseFile( filePath.toStdString() );

	// parse the file name from the path and set it as the text of the label
	int nameStart = filePath.lastIndexOf( '/' ) + 1;

	QString fileName;
	for (int i = nameStart; i < filePath.length(); ++i)
	{
		fileName += filePath.at( i );
	}

	openFileLabel->setText( fileName );
	

	// enable buttons
	graphButton->setEnabled( true );
	reportButton->setEnabled( true );
	close->setEnabled( true );
}