/********************************************************************************
** Form generated from reading UI file 'gpdriverselector.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GPDRIVERSELECTOR_H
#define UI_GPDRIVERSELECTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GPDriverSelector
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *comboBox;
    QHBoxLayout *horizontalLayout;
    QCheckBox *cbEnable;
    QCheckBox *debug;
    QLabel *info;

    void setupUi(QWidget *GPDriverSelector)
    {
        if (GPDriverSelector->objectName().isEmpty())
            GPDriverSelector->setObjectName(QStringLiteral("GPDriverSelector"));
        GPDriverSelector->resize(247, 96);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GPDriverSelector->sizePolicy().hasHeightForWidth());
        GPDriverSelector->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(GPDriverSelector);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(GPDriverSelector);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(label);

        comboBox = new QComboBox(GPDriverSelector);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        horizontalLayout_2->addWidget(comboBox);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        cbEnable = new QCheckBox(GPDriverSelector);
        cbEnable->setObjectName(QStringLiteral("cbEnable"));

        horizontalLayout->addWidget(cbEnable);

        debug = new QCheckBox(GPDriverSelector);
        debug->setObjectName(QStringLiteral("debug"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(debug->sizePolicy().hasHeightForWidth());
        debug->setSizePolicy(sizePolicy2);
        debug->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout->addWidget(debug);


        verticalLayout->addLayout(horizontalLayout);

        info = new QLabel(GPDriverSelector);
        info->setObjectName(QStringLiteral("info"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(info->sizePolicy().hasHeightForWidth());
        info->setSizePolicy(sizePolicy3);
        info->setText(QStringLiteral(""));
        info->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        verticalLayout->addWidget(info);


        retranslateUi(GPDriverSelector);

        QMetaObject::connectSlotsByName(GPDriverSelector);
    } // setupUi

    void retranslateUi(QWidget *GPDriverSelector)
    {
        GPDriverSelector->setWindowTitle(QApplication::translate("GPDriverSelector", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("GPDriverSelector", "TextLabel", Q_NULLPTR));
        cbEnable->setText(QApplication::translate("GPDriverSelector", "Enable", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        debug->setToolTip(QApplication::translate("GPDriverSelector", "Show drivers' reachable positions on the map", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        debug->setText(QApplication::translate("GPDriverSelector", "Valid moves", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GPDriverSelector: public Ui_GPDriverSelector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GPDRIVERSELECTOR_H
