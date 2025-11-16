/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QFrame *sidebar;
    QVBoxLayout *sidebarLayout;
    QPushButton *btn_dashboard;
    QPushButton *btn_tuyenduong;
    QPushButton *btn_nutgiao;
    QPushButton *btn_thongke;
    QSpacerItem *spacer;
    QStackedWidget *stackedWidget;
    QWidget *pageDashboard;
    QVBoxLayout *vboxLayout;
    QLabel *labelHome;
    QWidget *pageTuyenDuong;
    QVBoxLayout *vboxLayout1;
    QLabel *label;
    QTableWidget *tableRoutes;
    QFormLayout *formLayout;
    QLabel *label1;
    QLineEdit *txt_name;
    QLabel *label2;
    QDoubleSpinBox *spin_length;
    QLabel *label3;
    QComboBox *cb_start;
    QLabel *label4;
    QComboBox *cb_end;
    QHBoxLayout *hboxLayout;
    QPushButton *btn_add;
    QPushButton *btn_edit;
    QPushButton *btn_delete;
    QWidget *pageNutGiao;
    QVBoxLayout *vboxLayout2;
    QLabel *label5;
    QGraphicsView *gv_map;
    QWidget *pageThongKe;
    QVBoxLayout *vboxLayout3;
    QLabel *label6;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        sidebar = new QFrame(centralwidget);
        sidebar->setObjectName("sidebar");
        sidebar->setFrameShape(QFrame::StyledPanel);
        sidebarLayout = new QVBoxLayout(sidebar);
        sidebarLayout->setObjectName("sidebarLayout");
        btn_dashboard = new QPushButton(sidebar);
        btn_dashboard->setObjectName("btn_dashboard");

        sidebarLayout->addWidget(btn_dashboard);

        btn_tuyenduong = new QPushButton(sidebar);
        btn_tuyenduong->setObjectName("btn_tuyenduong");

        sidebarLayout->addWidget(btn_tuyenduong);

        btn_nutgiao = new QPushButton(sidebar);
        btn_nutgiao->setObjectName("btn_nutgiao");

        sidebarLayout->addWidget(btn_nutgiao);

        btn_thongke = new QPushButton(sidebar);
        btn_thongke->setObjectName("btn_thongke");

        sidebarLayout->addWidget(btn_thongke);

        spacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sidebarLayout->addItem(spacer);


        horizontalLayout->addWidget(sidebar);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        pageDashboard = new QWidget();
        pageDashboard->setObjectName("pageDashboard");
        vboxLayout = new QVBoxLayout(pageDashboard);
        vboxLayout->setObjectName("vboxLayout");
        labelHome = new QLabel(pageDashboard);
        labelHome->setObjectName("labelHome");

        vboxLayout->addWidget(labelHome);

        stackedWidget->addWidget(pageDashboard);
        pageTuyenDuong = new QWidget();
        pageTuyenDuong->setObjectName("pageTuyenDuong");
        vboxLayout1 = new QVBoxLayout(pageTuyenDuong);
        vboxLayout1->setObjectName("vboxLayout1");
        label = new QLabel(pageTuyenDuong);
        label->setObjectName("label");

        vboxLayout1->addWidget(label);

        tableRoutes = new QTableWidget(pageTuyenDuong);
        if (tableRoutes->columnCount() < 5)
            tableRoutes->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableRoutes->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableRoutes->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableRoutes->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableRoutes->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableRoutes->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        tableRoutes->setObjectName("tableRoutes");

        vboxLayout1->addWidget(tableRoutes);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label1 = new QLabel(pageTuyenDuong);
        label1->setObjectName("label1");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label1);

        txt_name = new QLineEdit(pageTuyenDuong);
        txt_name->setObjectName("txt_name");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, txt_name);

        label2 = new QLabel(pageTuyenDuong);
        label2->setObjectName("label2");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label2);

        spin_length = new QDoubleSpinBox(pageTuyenDuong);
        spin_length->setObjectName("spin_length");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, spin_length);

        label3 = new QLabel(pageTuyenDuong);
        label3->setObjectName("label3");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label3);

        cb_start = new QComboBox(pageTuyenDuong);
        cb_start->setObjectName("cb_start");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, cb_start);

        label4 = new QLabel(pageTuyenDuong);
        label4->setObjectName("label4");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label4);

        cb_end = new QComboBox(pageTuyenDuong);
        cb_end->setObjectName("cb_end");

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, cb_end);


        vboxLayout1->addLayout(formLayout);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName("hboxLayout");
        btn_add = new QPushButton(pageTuyenDuong);
        btn_add->setObjectName("btn_add");

        hboxLayout->addWidget(btn_add);

        btn_edit = new QPushButton(pageTuyenDuong);
        btn_edit->setObjectName("btn_edit");

        hboxLayout->addWidget(btn_edit);

        btn_delete = new QPushButton(pageTuyenDuong);
        btn_delete->setObjectName("btn_delete");

        hboxLayout->addWidget(btn_delete);


        vboxLayout1->addLayout(hboxLayout);

        stackedWidget->addWidget(pageTuyenDuong);
        pageNutGiao = new QWidget();
        pageNutGiao->setObjectName("pageNutGiao");
        vboxLayout2 = new QVBoxLayout(pageNutGiao);
        vboxLayout2->setObjectName("vboxLayout2");
        label5 = new QLabel(pageNutGiao);
        label5->setObjectName("label5");

        vboxLayout2->addWidget(label5);

        gv_map = new QGraphicsView(pageNutGiao);
        gv_map->setObjectName("gv_map");

        vboxLayout2->addWidget(gv_map);

        stackedWidget->addWidget(pageNutGiao);
        pageThongKe = new QWidget();
        pageThongKe->setObjectName("pageThongKe");
        vboxLayout3 = new QVBoxLayout(pageThongKe);
        vboxLayout3->setObjectName("vboxLayout3");
        label6 = new QLabel(pageThongKe);
        label6->setObjectName("label6");

        vboxLayout3->addWidget(label6);

        stackedWidget->addWidget(pageThongKe);

        horizontalLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        btn_dashboard->setText(QCoreApplication::translate("MainWindow", "Trang ch\341\273\247", nullptr));
        btn_tuyenduong->setText(QCoreApplication::translate("MainWindow", "Tuy\341\272\277n \304\221\306\260\341\273\235ng", nullptr));
        btn_nutgiao->setText(QCoreApplication::translate("MainWindow", "N\303\272t giao", nullptr));
        btn_thongke->setText(QCoreApplication::translate("MainWindow", "Th\341\273\221ng k\303\252", nullptr));
        labelHome->setText(QCoreApplication::translate("MainWindow", "Trang ch\341\273\247", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Qu\341\272\243n l\303\275 tuy\341\272\277n \304\221\306\260\341\273\235ng", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableRoutes->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableRoutes->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "T\303\252n tuy\341\272\277n", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableRoutes->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Chi\341\273\201u d\303\240i", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableRoutes->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "\304\220i\341\273\203m \304\221\341\272\247u", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableRoutes->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "\304\220i\341\273\203m cu\341\273\221i", nullptr));
        label1->setText(QCoreApplication::translate("MainWindow", "T\303\252n tuy\341\272\277n:", nullptr));
        label2->setText(QCoreApplication::translate("MainWindow", "Chi\341\273\201u d\303\240i (km):", nullptr));
        label3->setText(QCoreApplication::translate("MainWindow", "\304\220i\341\273\203m \304\221\341\272\247u:", nullptr));
        label4->setText(QCoreApplication::translate("MainWindow", "\304\220i\341\273\203m cu\341\273\221i:", nullptr));
        btn_add->setText(QCoreApplication::translate("MainWindow", "Th\303\252m", nullptr));
        btn_edit->setText(QCoreApplication::translate("MainWindow", "S\341\273\255a", nullptr));
        btn_delete->setText(QCoreApplication::translate("MainWindow", "X\303\263a", nullptr));
        label5->setText(QCoreApplication::translate("MainWindow", "Qu\341\272\243n l\303\275 n\303\272t giao", nullptr));
        label6->setText(QCoreApplication::translate("MainWindow", "Th\341\273\221ng k\303\252", nullptr));
        (void)MainWindow;
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
