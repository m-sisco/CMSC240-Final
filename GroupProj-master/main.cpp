#include <QApplication>

#include <GUI.h>

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );

	GUI* eyeAnalysis = new GUI;
	eyeAnalysis->show();

	return app.exec();
}