/********************************************************************************
** Form generated from reading UI file 'gpmapview.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GPMAPVIEW_H
#define UI_GPMAPVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GPMapView
{
public:

    void setupUi(QWidget *GPMapView)
    {
        if (GPMapView->objectName().isEmpty())
            GPMapView->setObjectName(QStringLiteral("GPMapView"));
        GPMapView->resize(400, 300);
        GPMapView->setMinimumSize(QSize(50, 50));

        retranslateUi(GPMapView);

        QMetaObject::connectSlotsByName(GPMapView);
    } // setupUi

    void retranslateUi(QWidget *GPMapView)
    {
        GPMapView->setWindowTitle(QApplication::translate("GPMapView", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GPMapView: public Ui_GPMapView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GPMAPVIEW_H
