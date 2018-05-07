/********************************************************************************
** Form generated from reading UI file 'AutoEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTOEDITOR_H
#define UI_AUTOEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AutoEditorClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *AutoEditorClass)
    {
        if (AutoEditorClass->objectName().isEmpty())
            AutoEditorClass->setObjectName(QStringLiteral("AutoEditorClass"));
        AutoEditorClass->resize(600, 400);
        centralWidget = new QWidget(AutoEditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        AutoEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(AutoEditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        AutoEditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(AutoEditorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        AutoEditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(AutoEditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        AutoEditorClass->setStatusBar(statusBar);

        retranslateUi(AutoEditorClass);

        QMetaObject::connectSlotsByName(AutoEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *AutoEditorClass)
    {
        AutoEditorClass->setWindowTitle(QApplication::translate("AutoEditorClass", "AutoEditor", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AutoEditorClass: public Ui_AutoEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTOEDITOR_H
