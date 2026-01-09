#include "revjaneiro.h"
#include "ui_revjaneiro.h"

revjaneiro::revjaneiro(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::revjaneiro)
{
    ui->setupUi(this);
}

revjaneiro::~revjaneiro()
{
    delete ui;
}
