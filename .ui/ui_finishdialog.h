/********************************************************************************
** Form generated from reading UI file 'finishdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINISHDIALOG_H
#define UI_FINISHDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FinnishDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *table;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *abortButton;

    void setupUi(QDialog *FinnishDialog)
    {
        if (FinnishDialog->objectName().isEmpty())
            FinnishDialog->setObjectName(QStringLiteral("FinnishDialog"));
        FinnishDialog->resize(299, 88);
        verticalLayout = new QVBoxLayout(FinnishDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        table = new QWidget(FinnishDialog);
        table->setObjectName(QStringLiteral("table"));
        gridLayout = new QGridLayout(table);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(table);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(table);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        label_3 = new QLabel(table);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        label_4 = new QLabel(table);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 0, 3, 1, 1);

        label_5 = new QLabel(table);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 0, 4, 1, 1);


        verticalLayout->addWidget(table);

        abortButton = new QPushButton(FinnishDialog);
        abortButton->setObjectName(QStringLiteral("abortButton"));

        verticalLayout->addWidget(abortButton);


        retranslateUi(FinnishDialog);
        QObject::connect(abortButton, SIGNAL(clicked(bool)), FinnishDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(FinnishDialog);
    } // setupUi

    void retranslateUi(QDialog *FinnishDialog)
    {
        FinnishDialog->setWindowTitle(QApplication::translate("FinnishDialog", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("FinnishDialog", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Rank</span></p></body></html>", Q_NULLPTR));
        label_2->setText(QApplication::translate("FinnishDialog", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Driver</span></p></body></html>", Q_NULLPTR));
        label_3->setText(QApplication::translate("FinnishDialog", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Hops</span></p></body></html>", Q_NULLPTR));
        label_4->setText(QApplication::translate("FinnishDialog", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Gas</span></p></body></html>", Q_NULLPTR));
        label_5->setText(QApplication::translate("FinnishDialog", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Boosts</span></p></body></html>", Q_NULLPTR));
        abortButton->setText(QApplication::translate("FinnishDialog", "Ok", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FinnishDialog: public Ui_FinnishDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINISHDIALOG_H
