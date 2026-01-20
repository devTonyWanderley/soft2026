/********************************************************************************
** Form generated from reading UI file 'leica.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEICA_H
#define UI_LEICA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Leica
{
public:
    QWidget *centralwidget;
    QLabel *lbBaud;
    QComboBox *cbBaud;
    QLabel *lbPort;
    QComboBox *cbPort;
    QPushButton *pbOp;
    QLabel *lbStatus;

    void setupUi(QMainWindow *Leica)
    {
        if (Leica->objectName().isEmpty())
            Leica->setObjectName("Leica");
        Leica->resize(309, 213);
        centralwidget = new QWidget(Leica);
        centralwidget->setObjectName("centralwidget");
        lbBaud = new QLabel(centralwidget);
        lbBaud->setObjectName("lbBaud");
        lbBaud->setGeometry(QRect(20, 20, 81, 20));
        cbBaud = new QComboBox(centralwidget);
        cbBaud->setObjectName("cbBaud");
        cbBaud->setGeometry(QRect(20, 50, 76, 26));
        lbPort = new QLabel(centralwidget);
        lbPort->setObjectName("lbPort");
        lbPort->setGeometry(QRect(220, 20, 63, 20));
        cbPort = new QComboBox(centralwidget);
        cbPort->setObjectName("cbPort");
        cbPort->setGeometry(QRect(210, 50, 76, 26));
        pbOp = new QPushButton(centralwidget);
        pbOp->setObjectName("pbOp");
        pbOp->setGeometry(QRect(20, 120, 271, 29));
        lbStatus = new QLabel(centralwidget);
        lbStatus->setObjectName("lbStatus");
        lbStatus->setGeometry(QRect(30, 170, 261, 20));
        Leica->setCentralWidget(centralwidget);

        retranslateUi(Leica);

        QMetaObject::connectSlotsByName(Leica);
    } // setupUi

    void retranslateUi(QMainWindow *Leica)
    {
        Leica->setWindowTitle(QCoreApplication::translate("Leica", "Leica", nullptr));
        lbBaud->setText(QCoreApplication::translate("Leica", "Baud rate", nullptr));
        lbPort->setText(QCoreApplication::translate("Leica", "Port", nullptr));
        pbOp->setText(QCoreApplication::translate("Leica", "Abrir porta serial", nullptr));
        lbStatus->setText(QCoreApplication::translate("Leica", "Aguardando usu\303\241rio...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Leica: public Ui_Leica {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEICA_H
