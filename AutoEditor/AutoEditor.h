#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AutoEditor.h"

class AutoEditor : public QMainWindow
{
	Q_OBJECT

public:
	AutoEditor(QWidget *parent = Q_NULLPTR);

private:
	Ui::AutoEditorClass ui;
};
