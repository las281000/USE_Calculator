#ifndef UNI_H
#define UNI_H

#include<QString>
#include <vector>
using namespace std;

class Uni
{
public:
    QString name;
    QString specialty;
    QString faculty;
    QString ExtExams[2]= {"",""};
    QString score;
    QString degree;
    QString comment;
};

#endif // UNI_H
