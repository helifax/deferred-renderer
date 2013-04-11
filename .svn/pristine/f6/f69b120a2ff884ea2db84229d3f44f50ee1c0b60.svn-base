// On windows if this is true no console application window is displayed and thus no performance hit from all the console prints
#ifndef DEBUG_CONSOLE_ENABLED
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <QtGui/QApplication>
#include "MainWindow.h"
/* this code runs the basic main window and is created by the Qt Creator app */
int main(int argc, char *argv[])
{
  // make an instance of the QApplication  
  QApplication a(argc, argv);
  // Create a new MainWindow
  MainWindow w;
  // show it
  w.show();
  // hand control over to Qt framework
  return a.exec();
}
