#include "MainWindow.h"
#include "GLWindow.h"

//----------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(
                       QWidget *_parent
                      ):
                        QMainWindow(_parent)
{
  this->resize(QSize(1366,768));
  m_gl = new GLWindow(this);
  this->setWindowTitle(QString("ngl::Deferred Renderer v.0.3"));
}

//----------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
}


//----------------------------------------------------------------------------------------------------------------------
void MainWindow::keyPressEvent(
                               QKeyEvent *_event
                              )
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  case Qt::Key_Escape : QApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
  case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
  case Qt::Key_F : this->showFullScreen(); break;
  case Qt::Key_G : this->showNormal(); break;
  case Qt::Key_Space : m_gl->toggleAnimation(); break;
	default : break;
  }
		m_gl->processKey(_event);
  // finally update the GLWindow and re-draw
  m_gl->updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::resizeEvent ( QResizeEvent * _event )
{
  m_gl->resize(_event->size());
}

