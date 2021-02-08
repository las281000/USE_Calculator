#ifndef INPUT_H
#define INPUT_H
#include <string>
#include <utility>
#include <vector>
#include <QLabel>
#include <QSpinBox>
#include "ui_widget.h"
#include <QWidget>
#include <widget.h>
#include <QString>

using namespace std;


class Input
{
private:
  int rus=0;
  int math=0;
  int extraP [2] = {0, 0};
  QString name [2] = {"", ""};
  int sum;

public:
   int getPoints(int exam);
   void setName (QString, int);
   QString getName (int);
   void setSum();

friend class Widget;
};

#endif // INPUT_H
