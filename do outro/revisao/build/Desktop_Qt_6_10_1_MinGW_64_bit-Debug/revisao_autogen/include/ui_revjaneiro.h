/********************************************************************************
** Form generated from reading UI file 'revjaneiro.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REVJANEIRO_H
#define UI_REVJANEIRO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_revjaneiro
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *revjaneiro)
    {
        if (revjaneiro->objectName().isEmpty())
            revjaneiro->setObjectName("revjaneiro");
        revjaneiro->resize(800, 600);
        centralwidget = new QWidget(revjaneiro);
        centralwidget->setObjectName("centralwidget");
        revjaneiro->setCentralWidget(centralwidget);
        menubar = new QMenuBar(revjaneiro);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        revjaneiro->setMenuBar(menubar);
        statusbar = new QStatusBar(revjaneiro);
        statusbar->setObjectName("statusbar");
        revjaneiro->setStatusBar(statusbar);

        retranslateUi(revjaneiro);

        QMetaObject::connectSlotsByName(revjaneiro);
    } // setupUi

    void retranslateUi(QMainWindow *revjaneiro)
    {
        revjaneiro->setWindowTitle(QCoreApplication::translate("revjaneiro", "revjaneiro", nullptr));
    } // retranslateUi

};

namespace Ui {
    class revjaneiro: public Ui_revjaneiro {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REVJANEIRO_H
