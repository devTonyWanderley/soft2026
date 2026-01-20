/********************************************************************************
** Form generated from reading UI file 'serial.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERIAL_H
#define UI_SERIAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Serial
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *lbPort;
    QComboBox *cbPort;
    QSpacerItem *horizontalSpacer_3;
    QLabel *lbPar;
    QComboBox *cbPar;
    QLabel *lbBaud;
    QComboBox *cbBaud;
    QSpacerItem *horizontalSpacer_2;
    QLabel *lbStop;
    QComboBox *cbStop;
    QLabel *lbData;
    QComboBox *cbData;
    QSpacerItem *horizontalSpacer;
    QLabel *lbFlow;
    QComboBox *cbFlow;
    QPlainTextEdit *pteMoni;
    QLabel *lbStatus;
    QPushButton *pbContexto;

    void setupUi(QMainWindow *Serial)
    {
        if (Serial->objectName().isEmpty())
            Serial->setObjectName("Serial");
        Serial->resize(800, 514);
        centralwidget = new QWidget(Serial);
        centralwidget->setObjectName("centralwidget");
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName("gridLayout");
        lbPort = new QLabel(groupBox);
        lbPort->setObjectName("lbPort");
        lbPort->setMinimumSize(QSize(120, 0));
        lbPort->setMaximumSize(QSize(120, 16777215));

        gridLayout->addWidget(lbPort, 0, 0, 1, 1);

        cbPort = new QComboBox(groupBox);
        cbPort->setObjectName("cbPort");

        gridLayout->addWidget(cbPort, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(357, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 2, 1, 1);

        lbPar = new QLabel(groupBox);
        lbPar->setObjectName("lbPar");
        lbPar->setMinimumSize(QSize(120, 0));
        lbPar->setMaximumSize(QSize(120, 16777215));

        gridLayout->addWidget(lbPar, 0, 3, 1, 1);

        cbPar = new QComboBox(groupBox);
        cbPar->setObjectName("cbPar");

        gridLayout->addWidget(cbPar, 0, 4, 1, 1);

        lbBaud = new QLabel(groupBox);
        lbBaud->setObjectName("lbBaud");
        lbBaud->setMinimumSize(QSize(120, 0));
        lbBaud->setMaximumSize(QSize(120, 16777215));

        gridLayout->addWidget(lbBaud, 1, 0, 1, 1);

        cbBaud = new QComboBox(groupBox);
        cbBaud->setObjectName("cbBaud");

        gridLayout->addWidget(cbBaud, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        lbStop = new QLabel(groupBox);
        lbStop->setObjectName("lbStop");
        lbStop->setMinimumSize(QSize(120, 0));
        lbStop->setMaximumSize(QSize(120, 16777215));

        gridLayout->addWidget(lbStop, 1, 3, 1, 1);

        cbStop = new QComboBox(groupBox);
        cbStop->setObjectName("cbStop");

        gridLayout->addWidget(cbStop, 1, 4, 1, 1);

        lbData = new QLabel(groupBox);
        lbData->setObjectName("lbData");
        lbData->setMinimumSize(QSize(120, 0));
        lbData->setMaximumSize(QSize(120, 16777215));

        gridLayout->addWidget(lbData, 2, 0, 1, 1);

        cbData = new QComboBox(groupBox);
        cbData->setObjectName("cbData");

        gridLayout->addWidget(cbData, 2, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 2, 1, 1);

        lbFlow = new QLabel(groupBox);
        lbFlow->setObjectName("lbFlow");
        lbFlow->setMinimumSize(QSize(120, 0));
        lbFlow->setMaximumSize(QSize(120, 16777215));

        gridLayout->addWidget(lbFlow, 2, 3, 1, 1);

        cbFlow = new QComboBox(groupBox);
        cbFlow->setObjectName("cbFlow");

        gridLayout->addWidget(cbFlow, 2, 4, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);

        pteMoni = new QPlainTextEdit(centralwidget);
        pteMoni->setObjectName("pteMoni");
        pteMoni->setStyleSheet(QString::fromUtf8("QPlainTextEdit {\n"
"    background-color: #1a1a1a; \n"
"    color: #00ff41; \n"
"    font-family: 'Courier New';\n"
"    border: none\n"
"}\n"
""));

        gridLayout_2->addWidget(pteMoni, 1, 0, 1, 1);

        lbStatus = new QLabel(centralwidget);
        lbStatus->setObjectName("lbStatus");

        gridLayout_2->addWidget(lbStatus, 2, 0, 1, 1);

        pbContexto = new QPushButton(centralwidget);
        pbContexto->setObjectName("pbContexto");

        gridLayout_2->addWidget(pbContexto, 3, 0, 1, 1);

        Serial->setCentralWidget(centralwidget);

        retranslateUi(Serial);

        QMetaObject::connectSlotsByName(Serial);
    } // setupUi

    void retranslateUi(QMainWindow *Serial)
    {
        Serial->setWindowTitle(QCoreApplication::translate("Serial", "Serial", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Serial", " .. --CONFIGURA\303\207\303\203O DE SOFTWERE-- ..", nullptr));
#if QT_CONFIG(tooltip)
        lbPort->setToolTip(QCoreApplication::translate("Serial", "Selecione a porta serial", nullptr));
#endif // QT_CONFIG(tooltip)
        lbPort->setText(QCoreApplication::translate("Serial", "Port", nullptr));
#if QT_CONFIG(tooltip)
        cbPort->setToolTip(QCoreApplication::translate("Serial", "Selecione a porta serial", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        lbPar->setToolTip(QCoreApplication::translate("Serial", "Selecione a porta serial", nullptr));
#endif // QT_CONFIG(tooltip)
        lbPar->setText(QCoreApplication::translate("Serial", "Paridade", nullptr));
#if QT_CONFIG(tooltip)
        cbPar->setToolTip(QCoreApplication::translate("Serial", "Selecione a porta serial", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        lbBaud->setToolTip(QCoreApplication::translate("Serial", "Selecione a velocidade", nullptr));
#endif // QT_CONFIG(tooltip)
        lbBaud->setText(QCoreApplication::translate("Serial", "Baud rate", nullptr));
#if QT_CONFIG(tooltip)
        cbBaud->setToolTip(QCoreApplication::translate("Serial", "Selecione a velocidade", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        lbStop->setToolTip(QCoreApplication::translate("Serial", "Selecione a velocidade", nullptr));
#endif // QT_CONFIG(tooltip)
        lbStop->setText(QCoreApplication::translate("Serial", "Stop bit", nullptr));
#if QT_CONFIG(tooltip)
        cbStop->setToolTip(QCoreApplication::translate("Serial", "Selecione a velocidade", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        lbData->setToolTip(QCoreApplication::translate("Serial", "Selecione o databit", nullptr));
#endif // QT_CONFIG(tooltip)
        lbData->setText(QCoreApplication::translate("Serial", "Data bit", nullptr));
#if QT_CONFIG(tooltip)
        cbData->setToolTip(QCoreApplication::translate("Serial", "Selecione o databit", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        lbFlow->setToolTip(QCoreApplication::translate("Serial", "Selecione o databit", nullptr));
#endif // QT_CONFIG(tooltip)
        lbFlow->setText(QCoreApplication::translate("Serial", "Controle de fluxo", nullptr));
#if QT_CONFIG(tooltip)
        cbFlow->setToolTip(QCoreApplication::translate("Serial", "Selecione o databit", nullptr));
#endif // QT_CONFIG(tooltip)
        lbStatus->setText(QCoreApplication::translate("Serial", "TextLabel", nullptr));
        pbContexto->setText(QCoreApplication::translate("Serial", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Serial: public Ui_Serial {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIAL_H
