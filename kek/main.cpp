#include "widget.h"
#include <QApplication>
#include "ui_widget.h"

int main(int argc, char *argv[])
{ 
                QApplication a(argc, argv);
                Widget Widget;
                Widget.show();
                return a.exec();
}
