#include "widget.h"
#include "ui_widget.h"
#include "input.h"
#include <QString>
#include <QSpinBox>
#include <QFile>
#include "uni.h"
#include <QTextStream>
#include <iostream>
#include <QTextEdit>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QTableWidgetItem* H0 = new QTableWidgetItem ("University");
    QTableWidgetItem* H1 = new QTableWidgetItem ("Faculty");
    QTableWidgetItem* H2 = new QTableWidgetItem ("Specialty");
    QTableWidgetItem* H3 = new QTableWidgetItem ("USE");
    QTableWidgetItem* H4 = new QTableWidgetItem ("Points");
    QTableWidgetItem* H5 = new QTableWidgetItem ("Degree");
    QTableWidgetItem* H6 = new QTableWidgetItem ("Additional exams");

    ui->tableWidget->setHorizontalHeaderItem(0,H0);
    ui->tableWidget->setHorizontalHeaderItem(1,H1);
    ui->tableWidget->setHorizontalHeaderItem(2,H2);
    ui->tableWidget->setHorizontalHeaderItem(3,H3);
    ui->tableWidget->setHorizontalHeaderItem(4,H4);
    ui->tableWidget->setHorizontalHeaderItem(5,H5);
    ui->tableWidget->setHorizontalHeaderItem(6,H6);

    for (int i=0; i<ui->tableWidget->columnCount(); i++)
        if (i!=4)
            ui->tableWidget->setColumnWidth(i,227);
        else
            ui->tableWidget->setColumnWidth(i,67);

    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->hide();
    ui->Sum->hide();
    ui->Form->hide();
    ui->SaveButton->hide();
}


Widget::~Widget()
{
    delete ui;
}

void Widget::on_ButtonSearch_released() //найти ненулевую коробку и лейбл
{

    while (ui->tableWidget->rowCount() > 0)
    {
        ui->tableWidget->removeRow(0);
    }

    int minP;    //Минимальные баллы
    int t=0;     //подсчет непустых спинБоксов
    int good=0;  //подсчет незаваленных экзаменов
    Input Inp;

    // Проверка баллов:

    //Русский и математика:
    if ((ui->BRus->value() >= 36)&&(ui->BMat->value() >= 27)) //если норм баллы
    {
        Inp.rus= ui->BRus->value();
        Inp.math= ui->BMat->value();
    } else //если завален(а) рус/мат
      {
      ui->Message->setText("Unfortunately, you failed your exams. It is impossible to find an educational program for you.");
      return;
      }

    //Экзамены по выбору:
    for (int i=1; i<10; i++)//Перебор всех спинБоксов
    {
        QString SB_Name = "B"+QString::number(i); //формируем имя текущего спинБокса
        QSpinBox* CurrentB = QWidget::findChild <QSpinBox *> (SB_Name); // поиск нужного спинБокса по имени

        if (CurrentB->value()>0)//если в текущем спинБоксе >0
        {
            t++; //1,2
            if (t<=2) //если к этому моменту найдено не больше 2-ух ненулевых спинБоксов
            {
                switch (i) //установка минимального балла для текущего спинБокса
                {
                case 1: minP=40; //ИКТ
                    break;
                case 5: minP=37; //География
                    break;
                case 6: minP=22; //Ин.яз
                    break;
                case 7: minP=32; //История
                    break;
                case 8: minP=42; //Общага
                    break;
                case 9: minP=32; //Лит-ра
                    break;
                default:minP=36; // Все остльное:
                    break;   // Физика, Химия, Биология
                }

                if (CurrentB->value()>=minP) // если введен балл >= минимуму то запомним его
                {
                    good++;
                    Inp.extraP[good-1]=  CurrentB->value();
                    QString L_Name = "L"+QString::number(i);
                    QLabel* CurrentL= QWidget::findChild <QLabel *> (L_Name);
                    Inp.setName(CurrentL->text(),good-1);
                }

            } else
              {
                ui->Message->setText("You chose more than 2 additional exams. Let only 2 of them and try again.");
                return; //принудительно выходим из обработчика
               }
        }
    }//конец перебора

    if ((t>0)&&(good==0)) //
    {
        ui->Message->setText("Unfortunately, you failed additional exams. It is impossible to find an educational program for you.");
        return;
    }
    if (t==0)
    {
        ui->Message->setText("You chose no additional exams. 1-2 exams are needed for search.");
        return;
    }


    Inp.setSum();
    Uni U;
    ui->tableWidget->show(); // делаем таблицу видимой
    ui->AddButton->show(); //появляется кнопка для добавления записей

    ui->Sum->show(); //появляется лейбл с суммой баллов
    ui->Sum->setText("Total points: "+ QString::number(Inp.sum));

    QFile File ("C:/Users/mi air/Desktop/kek/ege.txt");
    File.open(QIODevice::ReadOnly);

    while (!File.atEnd())
    {
       std::string Line = File.readLine().toStdString(); //взяли строчку и сконвертировали ее в std::string
       QString QL = QString::fromStdString(Line); //а теперь из std::string в QString

       U.name = QL.section(";", 0,0); //заполняем поля объекта для сравнения
       U.faculty = QL.section(";", 1,1);
       U.specialty = QL.section(";", 2,2);
       U.ExtExams[0] = QL.section(";", 3,3);
       U.ExtExams[1] = QL.section(";", 4,4);
       U.score = QL.section(";", 5,5);
       U.degree = QL.section(";", 6,6);
       U.comment = QL.section(";", 7,7);


       //сравним Input и Uni
       if ((Inp.getName(0).toStdString()==U.ExtExams[0].toStdString())&&
           (Inp.getName(1).toStdString()==U.ExtExams[1].toStdString()))
           {

           if (((U.comment == "---")&&(U.score.toInt()<=Inp.sum))||
                   ((U.comment!="---")&&(U.score.toInt()-Inp.sum<=100)))
            {
               QString Info;
               ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1); //добавляем строку
               ui->tableWidget->setRowHeight(ui->tableWidget->rowCount()-1,200);
               for (int col=0; col < ui->tableWidget->columnCount(); col++) //цикл по столбцам
               {
                   switch (col)
                   {
                   case 0: Info=U.name;
                       break;
                   case 1: Info=U.faculty;
                       break;
                   case 2: Info=U.specialty;
                       break;
                   case 3: Info="Russian Language\nMathematics\n"+U.ExtExams[0]+"\n"+U.ExtExams[1];
                       break;
                   case 4: Info=U.score;
                       break;
                   case 5: Info=U.degree;
                       break;
                   case 6: if (U.comment=="---") Info=U.comment;
                           else Info=U.comment+"\nMinimum: "+QString::number(U.score.toInt()-Inp.sum);
                       break;
                   }
                   QTableWidgetItem* itm = new QTableWidgetItem(Info);
                   itm->setFlags(Qt::ItemIsEditable);
                   itm->setTextAlignment(Qt::AlignCenter);
                   ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,col,itm);
               }
            }
          }
    }
File.close();
return;
}

void Widget::on_AddButton_released()
{
    ui->tableWidget->hide();
    ui->Message->hide();
    ui->Sum->hide();
    ui->AddButton->hide();

    ui->Form->show();
    ui->SaveButton->show();
    ui->Name->clear();
    ui->Faculty->clear();
    ui->Specialty->clear();
    ui->ExtEx1->clear();
    ui->ExtEx2->clear();
    ui->Score->clear();
    ui->Degree->clear();
    ui->AddEx->clear();
}

void Widget::on_SaveButton_released()
{
    int t=0; //считаем все что не нравится
    QString NewLine,QL;
    NewLine = (ui->Name->toPlainText())+';'      //0  формируем строку для файла
            +(ui->Faculty->toPlainText())+';'   //1
            +(ui->Specialty->toPlainText())+';' //2
            +(ui->ExtEx1->toPlainText())+';'    //3
            +(ui->ExtEx2->toPlainText())+';'    //4
            +(QString::number(ui->Score->value()))+';' //5
            +(ui->Degree->toPlainText())+';' //6
            +(ui->AddEx->toPlainText())+';'; //7

    for (int i=0; i<7; i++)
        if ((NewLine.section(";",i,i)=="")&&((i==0)||(i==1)||(i==2)||(i==3)||(i==5)||(i==6))) {
            t++;
        }

    if ((NewLine.section(";",4,4)!="")&&(NewLine.section(";",7,7)!="")&&(ui->Score->value()>500)) {
        t++;
        }

    if ((((NewLine.section(";",4,4)!="")&&(NewLine.section(";",7,7)==""))||
       ((NewLine.section(";",4,4)=="")&&(NewLine.section(";",7,7)!="")))&&(ui->Score->value()>400)){
        t++;
        }

    QFile File ("C:/Users/mi air/Desktop/kek/ege.txt");
    while (!File.atEnd())
    {
      std::string Line = File.readLine().toStdString(); //взяли строчку и превратили в std::string
      if (Line==NewLine.toStdString()){
          t++; //подсчет одинаковых строк
          }
    }

    ui->Form->hide();
    ui->SaveButton->hide();
    ui->AddButton->show();

    if (t==0){
      NewLine=NewLine+ "\r\n";
      File.open(QIODevice::Append);
      File.write(NewLine.toUtf8());

      ui->Message->setText("The record was successfully saved.");
      ui->Message->show();
      File.close();
      return;

   } else {
       ui->Message->setText("Unfortunately, it is impossible to save this record.");
       ui->Message->show();
       return;
   }
}
