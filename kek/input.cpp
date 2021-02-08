#include "input.h"
#include <QString>
#include <vector>
using namespace std;

int Input:: getPoints(int exam){
    return exam;
}

void Input:: setName (QString str, int index){
    name[index] = str;
}

QString Input:: getName (int index){
    return name[index];
}

void Input::setSum(){
    sum = rus+math+extraP[0]+extraP[1];
}
