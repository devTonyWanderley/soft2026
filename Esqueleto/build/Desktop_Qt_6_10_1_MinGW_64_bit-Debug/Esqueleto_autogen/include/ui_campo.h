/********************************************************************************
** Form generated from reading UI file 'campo.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMPO_H
#define UI_CAMPO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Campo
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QFrame *qfLed;
    QLabel *lbStatus;
    QPlainTextEdit *pteMonitor;
    QCheckBox *cbEdita;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbCfg;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pbContexto;

    void setupUi(QMainWindow *Campo)
    {
        if (Campo->objectName().isEmpty())
            Campo->setObjectName("Campo");
        Campo->resize(413, 145);
        centralwidget = new QWidget(Campo);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        qfLed = new QFrame(centralwidget);
        qfLed->setObjectName("qfLed");
        qfLed->setMinimumSize(QSize(20, 20));
        qfLed->setMaximumSize(QSize(20, 20));
        qfLed->setStyleSheet(QString::fromUtf8("#qfLed \n"
"{\n"
"background-color: red;\n"
"border: 3px solid darked;\n"
"border-radius: 10px;\n"
"}"));
        qfLed->setFrameShape(QFrame::Shape::StyledPanel);
        qfLed->setFrameShadow(QFrame::Shadow::Raised);

        gridLayout->addWidget(qfLed, 0, 0, 1, 1);

        lbStatus = new QLabel(centralwidget);
        lbStatus->setObjectName("lbStatus");

        gridLayout->addWidget(lbStatus, 0, 1, 1, 1);

        pteMonitor = new QPlainTextEdit(centralwidget);
        pteMonitor->setObjectName("pteMonitor");
        pteMonitor->setStyleSheet(QString::fromUtf8("#pteMonitor\n"
"{\n"
"background-color: black;\n"
"color: #AAFF00;\n"
"font-weight: bold;\n"
"font-size: 17px;\n"
"}"));

        gridLayout->addWidget(pteMonitor, 1, 0, 1, 5);

        cbEdita = new QCheckBox(centralwidget);
        cbEdita->setObjectName("cbEdita");

        gridLayout->addWidget(cbEdita, 2, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(259, 21, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 1, 1, 1);

        pbCfg = new QPushButton(centralwidget);
        pbCfg->setObjectName("pbCfg");

        gridLayout->addWidget(pbCfg, 2, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(258, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 3, 1, 1);

        pbContexto = new QPushButton(centralwidget);
        pbContexto->setObjectName("pbContexto");

        gridLayout->addWidget(pbContexto, 2, 4, 1, 1);

        Campo->setCentralWidget(centralwidget);

        retranslateUi(Campo);

        QMetaObject::connectSlotsByName(Campo);
    } // setupUi

    void retranslateUi(QMainWindow *Campo)
    {
        Campo->setWindowTitle(QCoreApplication::translate("Campo", "Campo", nullptr));
        lbStatus->setText(QCoreApplication::translate("Campo", "Configure a porta serial", nullptr));
        cbEdita->setText(QCoreApplication::translate("Campo", "Edit\303\241vel", nullptr));
        pbCfg->setText(QCoreApplication::translate("Campo", "Configurar porta", nullptr));
        pbContexto->setText(QCoreApplication::translate("Campo", "Contexto", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Campo: public Ui_Campo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMPO_H
