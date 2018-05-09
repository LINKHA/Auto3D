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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "openglwindow.h"

QT_BEGIN_NAMESPACE

class Ui_AutoEditorClass
{
public:
    QWidget *centralWidget;
    OpenGLWindow *widget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
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
        widget = new OpenGLWindow(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(0, 139, 291, 221));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(400, 40, 75, 23));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(400, 90, 75, 23));
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
        pushButton->setText(QApplication::translate("AutoEditorClass", "PushButton", nullptr));
        pushButton_2->setText(QApplication::translate("AutoEditorClass", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AutoEditorClass: public Ui_AutoEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTOEDITOR_H
