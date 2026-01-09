#ifndef REVJANEIRO_H
#define REVJANEIRO_H

//  revisar tipos:
//      ->Agrupar em um arquivo

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {class revjaneiro;}
QT_END_NAMESPACE

class revjaneiro : public QMainWindow
{
    Q_OBJECT

public:
    revjaneiro(QWidget *parent = nullptr);
    ~revjaneiro();

private:
    Ui::revjaneiro *ui;
};
#endif // REVJANEIRO_H
