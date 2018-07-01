/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QCustomPlot *widget_2;
    QCustomPlot *widget;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QLabel *label_2;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout;
    QPushButton *pushButton;
    QCheckBox *checkBox;
    QPushButton *pushButton_2;
    QCheckBox *checkBox_2;
    QPushButton *pushButton_3;
    QCheckBox *checkBox_3;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QTableWidget *tableWidget;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_11;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QPushButton *pushButton_10;
    QLabel *label;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menuAbout;
    QMenu *menubuton1;
    QMenu *menubuton2;
    QMenu *menubuton3;
    QMenu *menubuton4;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(1000, 500);
        MainWindow->setMinimumSize(QSize(1000, 500));
        MainWindow->setLocale(QLocale(QLocale::Russian, QLocale::Ukraine));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setEnabled(true);
        centralWidget->setLayoutDirection(Qt::LeftToRight);
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widget_2 = new QCustomPlot(centralWidget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(2);
        sizePolicy.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy);
        widget_2->setLayoutDirection(Qt::LeftToRight);

        gridLayout->addWidget(widget_2, 2, 1, 1, 1);

        widget = new QCustomPlot(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setLayoutDirection(Qt::LeftToRight);

        gridLayout->addWidget(widget, 2, 0, 1, 1);

        widget_3 = new QWidget(centralWidget);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setMinimumSize(QSize(0, 30));
        widget_3->setMaximumSize(QSize(16777215, 30));
        horizontalLayout_2 = new QHBoxLayout(widget_3);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton_4 = new QPushButton(widget_3);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout_2->addWidget(pushButton_4);

        pushButton_5 = new QPushButton(widget_3);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));

        horizontalLayout_2->addWidget(pushButton_5);

        pushButton_6 = new QPushButton(widget_3);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));

        horizontalLayout_2->addWidget(pushButton_6);

        pushButton_7 = new QPushButton(widget_3);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));

        horizontalLayout_2->addWidget(pushButton_7);


        gridLayout->addWidget(widget_3, 1, 0, 1, 1);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 3, 1, 1, 1);

        widget1 = new QWidget(centralWidget);
        widget1->setObjectName(QStringLiteral("widget1"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(widget1->sizePolicy().hasHeightForWidth());
        widget1->setSizePolicy(sizePolicy1);
        widget1->setMinimumSize(QSize(982, 228));
        widget1->setMaximumSize(QSize(16777215, 228));
        widget1->setLayoutDirection(Qt::LeftToRight);
        horizontalLayout = new QHBoxLayout(widget1);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        groupBox_2 = new QGroupBox(widget1);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(181, 195));
        groupBox_2->setMaximumSize(QSize(181, 195));
        formLayout = new QFormLayout(groupBox_2);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        formLayout->setWidget(0, QFormLayout::LabelRole, pushButton);

        checkBox = new QCheckBox(groupBox_2);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, checkBox);

        pushButton_2 = new QPushButton(groupBox_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, pushButton_2);

        checkBox_2 = new QCheckBox(groupBox_2);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        formLayout->setWidget(1, QFormLayout::FieldRole, checkBox_2);

        pushButton_3 = new QPushButton(groupBox_2);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, pushButton_3);

        checkBox_3 = new QCheckBox(groupBox_2);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));

        formLayout->setWidget(2, QFormLayout::FieldRole, checkBox_3);

        radioButton = new QRadioButton(groupBox_2);
        radioButton->setObjectName(QStringLiteral("radioButton"));

        formLayout->setWidget(3, QFormLayout::SpanningRole, radioButton);

        radioButton_2 = new QRadioButton(groupBox_2);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));

        formLayout->setWidget(4, QFormLayout::SpanningRole, radioButton_2);


        horizontalLayout->addWidget(groupBox_2);

        tableWidget = new QTableWidget(widget1);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        horizontalLayout->addWidget(tableWidget);


        gridLayout->addWidget(widget1, 0, 0, 1, 2);

        widget_4 = new QWidget(centralWidget);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        widget_4->setMinimumSize(QSize(0, 30));
        widget_4->setMaximumSize(QSize(16777215, 30));
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        pushButton_11 = new QPushButton(widget_4);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));

        horizontalLayout_3->addWidget(pushButton_11);

        pushButton_8 = new QPushButton(widget_4);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));

        horizontalLayout_3->addWidget(pushButton_8);

        pushButton_9 = new QPushButton(widget_4);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));

        horizontalLayout_3->addWidget(pushButton_9);

        pushButton_10 = new QPushButton(widget_4);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));

        horizontalLayout_3->addWidget(pushButton_10);


        gridLayout->addWidget(widget_4, 1, 1, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 3, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 21));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        menubuton1 = new QMenu(menuBar);
        menubuton1->setObjectName(QStringLiteral("menubuton1"));
        menubuton2 = new QMenu(menuBar);
        menubuton2->setObjectName(QStringLiteral("menubuton2"));
        menubuton3 = new QMenu(menuBar);
        menubuton3->setObjectName(QStringLiteral("menubuton3"));
        menubuton4 = new QMenu(menuBar);
        menubuton4->setObjectName(QStringLiteral("menubuton4"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuBar->addAction(menubuton1->menuAction());
        menuBar->addAction(menubuton2->menuAction());
        menuBar->addAction(menubuton3->menuAction());
        menuBar->addAction(menubuton4->menuAction());
        menu->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        pushButton_7->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "GroupBox", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        checkBox->setText(QApplication::translate("MainWindow", "CheckBox", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        checkBox_2->setText(QApplication::translate("MainWindow", "CheckBox", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        checkBox_3->setText(QApplication::translate("MainWindow", "CheckBox", Q_NULLPTR));
        radioButton->setText(QApplication::translate("MainWindow", "RadioButton", Q_NULLPTR));
        radioButton_2->setText(QApplication::translate("MainWindow", "RadioButton", Q_NULLPTR));
        pushButton_11->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        pushButton_8->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        pushButton_9->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        pushButton_10->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        menuAbout->setTitle(QApplication::translate("MainWindow", "About", Q_NULLPTR));
        menubuton1->setTitle(QApplication::translate("MainWindow", "buton1", Q_NULLPTR));
        menubuton2->setTitle(QApplication::translate("MainWindow", "buton2", Q_NULLPTR));
        menubuton3->setTitle(QApplication::translate("MainWindow", "buton3", Q_NULLPTR));
        menubuton4->setTitle(QApplication::translate("MainWindow", "buton4", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
