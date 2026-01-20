/********************************************************************************
** Form generated from reading UI file 'cfgcom.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CFGCOM_H
#define UI_CFGCOM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_cfgCom
{
public:
    QGridLayout *gridLayout;
    QLabel *lbPort;
    QComboBox *cbPort;
    QSpacerItem *horizontalSpacer;
    QLabel *lbBaud;
    QComboBox *cbBaud;
    QLabel *lbData;
    QComboBox *cbData;
    QSpacerItem *horizontalSpacer_2;
    QLabel *lbFlow;
    QComboBox *cbFlow;
    QLabel *lbStop;
    QComboBox *cbStop;
    QSpacerItem *horizontalSpacer_3;
    QLabel *lbPar;
    QComboBox *cbPar;
    QFrame *frame;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pbCancela;
    QPushButton *pbOk;

    void setupUi(QDialog *cfgCom)
    {
        if (cfgCom->objectName().isEmpty())
            cfgCom->setObjectName("cfgCom");
        cfgCom->resize(469, 192);
        gridLayout = new QGridLayout(cfgCom);
        gridLayout->setObjectName("gridLayout");
        lbPort = new QLabel(cfgCom);
        lbPort->setObjectName("lbPort");

        gridLayout->addWidget(lbPort, 0, 0, 1, 1);

        cbPort = new QComboBox(cfgCom);
        cbPort->setObjectName("cbPort");

        gridLayout->addWidget(cbPort, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(105, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        lbBaud = new QLabel(cfgCom);
        lbBaud->setObjectName("lbBaud");

        gridLayout->addWidget(lbBaud, 0, 3, 1, 1);

        cbBaud = new QComboBox(cfgCom);
        cbBaud->setObjectName("cbBaud");

        gridLayout->addWidget(cbBaud, 0, 4, 1, 1);

        lbData = new QLabel(cfgCom);
        lbData->setObjectName("lbData");

        gridLayout->addWidget(lbData, 1, 0, 1, 1);

        cbData = new QComboBox(cfgCom);
        cbData->setObjectName("cbData");

        gridLayout->addWidget(cbData, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(105, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        lbFlow = new QLabel(cfgCom);
        lbFlow->setObjectName("lbFlow");

        gridLayout->addWidget(lbFlow, 1, 3, 1, 1);

        cbFlow = new QComboBox(cfgCom);
        cbFlow->setObjectName("cbFlow");

        gridLayout->addWidget(cbFlow, 1, 4, 1, 1);

        lbStop = new QLabel(cfgCom);
        lbStop->setObjectName("lbStop");

        gridLayout->addWidget(lbStop, 2, 0, 1, 1);

        cbStop = new QComboBox(cfgCom);
        cbStop->setObjectName("cbStop");

        gridLayout->addWidget(cbStop, 2, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(105, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 2, 2, 1, 1);

        lbPar = new QLabel(cfgCom);
        lbPar->setObjectName("lbPar");

        gridLayout->addWidget(lbPar, 2, 3, 1, 1);

        cbPar = new QComboBox(cfgCom);
        cbPar->setObjectName("cbPar");

        gridLayout->addWidget(cbPar, 2, 4, 1, 1);

        frame = new QFrame(cfgCom);
        frame->setObjectName("frame");
        frame->setMinimumSize(QSize(20, 20));
        frame->setMaximumSize(QSize(20, 20));
        frame->setStyleSheet(QString::fromUtf8("#frame\n"
"{\n"
"background-color: red;\n"
"border: 3px solid darked;\n"
"border-radius: 10px;\n"
"}"));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);

        gridLayout->addWidget(frame, 3, 0, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(105, 21, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 3, 1, 1, 1);

        pbCancela = new QPushButton(cfgCom);
        pbCancela->setObjectName("pbCancela");

        gridLayout->addWidget(pbCancela, 3, 2, 1, 2);

        pbOk = new QPushButton(cfgCom);
        pbOk->setObjectName("pbOk");

        gridLayout->addWidget(pbOk, 3, 4, 1, 1);


        retranslateUi(cfgCom);

        QMetaObject::connectSlotsByName(cfgCom);
    } // setupUi

    void retranslateUi(QDialog *cfgCom)
    {
        cfgCom->setWindowTitle(QCoreApplication::translate("cfgCom", "Dialog", nullptr));
        lbPort->setText(QCoreApplication::translate("cfgCom", "PORT", nullptr));
        lbBaud->setText(QCoreApplication::translate("cfgCom", "BAUD RATE", nullptr));
        lbData->setText(QCoreApplication::translate("cfgCom", "DATA BIT", nullptr));
        lbFlow->setText(QCoreApplication::translate("cfgCom", "FLOW CONTROL", nullptr));
        lbStop->setText(QCoreApplication::translate("cfgCom", "STOP BIT", nullptr));
        lbPar->setText(QCoreApplication::translate("cfgCom", "PARIDADE", nullptr));
        pbCancela->setText(QCoreApplication::translate("cfgCom", "CANCELA", nullptr));
        pbOk->setText(QCoreApplication::translate("cfgCom", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class cfgCom: public Ui_cfgCom {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CFGCOM_H
