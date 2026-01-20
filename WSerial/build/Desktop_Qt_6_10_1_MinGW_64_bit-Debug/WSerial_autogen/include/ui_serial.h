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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Serial
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGroupBox *gbHard;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *lbPort;
    QLabel *lbBaud;
    QVBoxLayout *verticalLayout_2;
    QComboBox *cbPort;
    QComboBox *cbBaud;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_3;
    QLabel *lbDtBit;
    QLabel *lbPar;
    QVBoxLayout *verticalLayout_5;
    QComboBox *cbDtBit;
    QComboBox *cbPar;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_4;
    QLabel *lbStop;
    QLabel *lbFlow;
    QVBoxLayout *verticalLayout_6;
    QComboBox *cbStop;
    QComboBox *cbFlow;
    QPlainTextEdit *pteMoni;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lbStatus;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pbMain;
    QToolButton *tbRefresh;

    void setupUi(QMainWindow *Serial)
    {
        if (Serial->objectName().isEmpty())
            Serial->setObjectName("Serial");
        Serial->resize(951, 371);
        centralwidget = new QWidget(Serial);
        centralwidget->setObjectName("centralwidget");
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        gbHard = new QGroupBox(centralwidget);
        gbHard->setObjectName("gbHard");
        gridLayout = new QGridLayout(gbHard);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        lbPort = new QLabel(gbHard);
        lbPort->setObjectName("lbPort");
        lbPort->setMinimumSize(QSize(70, 25));
        lbPort->setMaximumSize(QSize(70, 25));

        verticalLayout->addWidget(lbPort);

        lbBaud = new QLabel(gbHard);
        lbBaud->setObjectName("lbBaud");
        lbBaud->setMinimumSize(QSize(70, 25));
        lbBaud->setMaximumSize(QSize(70, 25));

        verticalLayout->addWidget(lbBaud);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        cbPort = new QComboBox(gbHard);
        cbPort->setObjectName("cbPort");

        verticalLayout_2->addWidget(cbPort);

        cbBaud = new QComboBox(gbHard);
        cbBaud->setObjectName("cbBaud");

        verticalLayout_2->addWidget(cbBaud);


        horizontalLayout->addLayout(verticalLayout_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        lbDtBit = new QLabel(gbHard);
        lbDtBit->setObjectName("lbDtBit");
        lbDtBit->setMinimumSize(QSize(65, 25));
        lbDtBit->setMaximumSize(QSize(65, 25));

        verticalLayout_3->addWidget(lbDtBit);

        lbPar = new QLabel(gbHard);
        lbPar->setObjectName("lbPar");
        lbPar->setMinimumSize(QSize(65, 25));
        lbPar->setMaximumSize(QSize(65, 25));

        verticalLayout_3->addWidget(lbPar);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName("verticalLayout_5");
        cbDtBit = new QComboBox(gbHard);
        cbDtBit->setObjectName("cbDtBit");

        verticalLayout_5->addWidget(cbDtBit);

        cbPar = new QComboBox(gbHard);
        cbPar->setObjectName("cbPar");

        verticalLayout_5->addWidget(cbPar);


        horizontalLayout->addLayout(verticalLayout_5);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        lbStop = new QLabel(gbHard);
        lbStop->setObjectName("lbStop");
        lbStop->setMinimumSize(QSize(120, 25));
        lbStop->setMaximumSize(QSize(120, 25));

        verticalLayout_4->addWidget(lbStop);

        lbFlow = new QLabel(gbHard);
        lbFlow->setObjectName("lbFlow");
        lbFlow->setMinimumSize(QSize(120, 25));
        lbFlow->setMaximumSize(QSize(120, 25));

        verticalLayout_4->addWidget(lbFlow);


        horizontalLayout->addLayout(verticalLayout_4);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName("verticalLayout_6");
        cbStop = new QComboBox(gbHard);
        cbStop->setObjectName("cbStop");

        verticalLayout_6->addWidget(cbStop);

        cbFlow = new QComboBox(gbHard);
        cbFlow->setObjectName("cbFlow");

        verticalLayout_6->addWidget(cbFlow);


        horizontalLayout->addLayout(verticalLayout_6);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        gridLayout_2->addWidget(gbHard, 0, 0, 1, 1);

        pteMoni = new QPlainTextEdit(centralwidget);
        pteMoni->setObjectName("pteMoni");
        pteMoni->setStyleSheet(QString::fromUtf8("QPlainTextEdit {\n"
"    background-color: #121212;      /* Fundo quase preto (estilo Dark Mode) */\n"
"    color: #00FF00;                 /* Texto verde \"Matrix/Terminal\" para destaque */\n"
"    border: 2px solid #333333;      /* Borda discreta */\n"
"    border-radius: 4px;\n"
"    font-family: 'Consolas', 'Monospace', 'Courier New'; /* Fontes monoespa\303\247adas */\n"
"    font-size: 10pt;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"/* Estilizando a barra de rolagem para combinar */\n"
"QScrollBar:vertical {\n"
"    border: none;\n"
"    background: #222222;\n"
"    width: 10px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"    background: #444444;\n"
"    min-height: 20px;\n"
"}"));
        pteMoni->setReadOnly(true);

        gridLayout_2->addWidget(pteMoni, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        lbStatus = new QLabel(centralwidget);
        lbStatus->setObjectName("lbStatus");
        lbStatus->setMinimumSize(QSize(500, 0));

        horizontalLayout_2->addWidget(lbStatus);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        pbMain = new QPushButton(centralwidget);
        pbMain->setObjectName("pbMain");
        pbMain->setMinimumSize(QSize(200, 0));

        horizontalLayout_2->addWidget(pbMain);

        tbRefresh = new QToolButton(centralwidget);
        tbRefresh->setObjectName("tbRefresh");
        tbRefresh->setMinimumSize(QSize(50, 0));

        horizontalLayout_2->addWidget(tbRefresh);


        gridLayout_2->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        Serial->setCentralWidget(centralwidget);

        retranslateUi(Serial);

        QMetaObject::connectSlotsByName(Serial);
    } // setupUi

    void retranslateUi(QMainWindow *Serial)
    {
        Serial->setWindowTitle(QCoreApplication::translate("Serial", "Comunica\303\247\303\243o serial", nullptr));
        gbHard->setTitle(QCoreApplication::translate("Serial", "Configura\303\247\303\243o de hardware:", nullptr));
#if QT_CONFIG(tooltip)
        lbPort->setToolTip(QCoreApplication::translate("Serial", "Selecione porta", nullptr));
#endif // QT_CONFIG(tooltip)
        lbPort->setText(QCoreApplication::translate("Serial", "Port:", nullptr));
#if QT_CONFIG(tooltip)
        lbBaud->setToolTip(QCoreApplication::translate("Serial", "Sugere-se \"19200\" para Leica e \"9600\" para a Geodetic", nullptr));
#endif // QT_CONFIG(tooltip)
        lbBaud->setText(QCoreApplication::translate("Serial", "Baud rate", nullptr));
#if QT_CONFIG(tooltip)
        cbPort->setToolTip(QCoreApplication::translate("Serial", "Selecione porta", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        cbBaud->setToolTip(QCoreApplication::translate("Serial", "Sugere-se \"19200\" para Leica e \"9600\" para a Geodetic", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        lbDtBit->setToolTip(QCoreApplication::translate("Serial", "Normalmente  \"8\"", nullptr));
#endif // QT_CONFIG(tooltip)
        lbDtBit->setText(QCoreApplication::translate("Serial", "Data bits", nullptr));
#if QT_CONFIG(tooltip)
        lbPar->setToolTip(QCoreApplication::translate("Serial", "Sugere-se  \"Nenhuma\"", nullptr));
#endif // QT_CONFIG(tooltip)
        lbPar->setText(QCoreApplication::translate("Serial", "Paridade", nullptr));
#if QT_CONFIG(tooltip)
        cbDtBit->setToolTip(QCoreApplication::translate("Serial", "Normalmente  \"8\"", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        cbPar->setToolTip(QCoreApplication::translate("Serial", "Sugere-se  \"Nenhuma\"", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        lbStop->setToolTip(QCoreApplication::translate("Serial", "Sugere-se \"1\"", nullptr));
#endif // QT_CONFIG(tooltip)
        lbStop->setText(QCoreApplication::translate("Serial", "Stop bit", nullptr));
#if QT_CONFIG(tooltip)
        lbFlow->setToolTip(QCoreApplication::translate("Serial", "Sugere-se \"XON/XOFF\" para Leica e  \"Nenhum\" para a Geodetic", nullptr));
#endif // QT_CONFIG(tooltip)
        lbFlow->setText(QCoreApplication::translate("Serial", "Controle de fluxo", nullptr));
#if QT_CONFIG(tooltip)
        cbStop->setToolTip(QCoreApplication::translate("Serial", "Sugere-se \"1\"", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        cbFlow->setToolTip(QCoreApplication::translate("Serial", "Sugere-se \"XON/XOFF\" para Leica e  \"Nenhum\" para a Geodetic", nullptr));
#endif // QT_CONFIG(tooltip)
        lbStatus->setText(QCoreApplication::translate("Serial", ".. PRONTO ..", nullptr));
#if QT_CONFIG(tooltip)
        pbMain->setToolTip(QCoreApplication::translate("Serial", "Configure o hardware e clkick para conectar", nullptr));
#endif // QT_CONFIG(tooltip)
        pbMain->setText(QCoreApplication::translate("Serial", ".. CONECTAR ..", nullptr));
#if QT_CONFIG(tooltip)
        tbRefresh->setToolTip(QCoreApplication::translate("Serial", "Atualizar", nullptr));
#endif // QT_CONFIG(tooltip)
        tbRefresh->setText(QCoreApplication::translate("Serial", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Serial: public Ui_Serial {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIAL_H
