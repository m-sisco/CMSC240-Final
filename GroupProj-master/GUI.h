/*
 * Creates a GUI for analyzing eye tracking data. The GUI allows the user to
 * open a data file. Once a file is open, the user is presented with options to
 * create graphs of the data and generate a report of the data, which includes 
 * the number of saccades in the data, information about each saccade, and a 
 * measure of the accuracy of the eye movement to the movement of the target.
 */

#include <QMainWindow>
#include "DataReport.h"
#include "FileData.h"
#include "SaccadeList.h"
#include "DataGraph.h"
#include "option.h"

class QLabel;
class QPushButton;
class QButtonGroup;
class QMenuBar;
class QMenu;
class QAction;
class QTextBrowser;
class QString;


class GUI : public QMainWindow
{
	Q_OBJECT


	public:
		/*
		 * Constructor; initializes and displays GUI elements.
		 */
		GUI( QWidget* parent = 0 );

		/*
		 * Deallocates dynamically-allocated memory, not including the widgets
		 * used for the GUI.
		 */
		~GUI();


	signals:


	private slots:
		/*
		 * Closes the currently open file.
		 */
		void closeCurrentFile();

		/*
 		 * Creates and displays a graph depicting the movement of the selected
 		 * eye in the selected dimension compared to the movement of the target.
		 */
		void displayGraph();

		/*
		 * Creates a displays a report reflecting the selected coordinate and
		 * eye, including the number of saccades, the number of target movements,
		 * information about the primary and corrective saccades in the data, and
		 * the accuracy of the eye movement to the target's position.
		 */
		void displayReport();

		/*
		 * Creates a dialog for the user to select a data file to display
		 * and opens that file, placing its name in openFileLabel and placing 
		 * the contents of the file in openFile. If a file is already open, it
		 * closes the currently open file before opening the new one.
	 	 */
		void openDataFile();


	private:
		/*
		 * Generates and stores a report for the data.
		 */
		DataReport* report;

		/*
		 * Used to read in and store a data file.
		 */
		FileData* fileData;

		/*
		 * Used to identify saccades of the data.
		 */
		SaccadeList* saccades;

		/*
		 * Used to store and display graphs of the data.
		 */
		DataGraph* graph;

		/*
		 * A label that displays the name of the open file.
		 */
		QLabel* openFileLabel;

		/*
		 * Radio buttons that allow the user to choose which dimension
		 * to display in the generated graph.
		 */
		QButtonGroup* eyeButtons;

		/*
		 * Radio buttons that allow the user to choose which eye's data
		 * to display in the generated graph.
		 */
		QButtonGroup* dimensionButtons;

		/*
		 * Buttons to display the graph and report, respectively.
		 */
		QPushButton* graphButton;
		QPushButton* reportButton;

		/*
		 * The menu, which will include options to open a file, close the
		 * current file (if one is open), and exit the program.
		 */
		QMenuBar* menu;
		QMenu* fileMenu;

		QAction* open;
		QAction* close;
		QAction* quit;
};