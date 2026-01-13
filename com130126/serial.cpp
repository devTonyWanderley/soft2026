#include "serial.h"
#include "ui_serial.h"

Serial::Serial(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Serial)
{
    ui->setupUi(this);
}

Serial::~Serial()
{
    delete ui;
}
