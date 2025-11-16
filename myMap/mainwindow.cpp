#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include<QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initDatabase();
    loadRoutes();

    connect(ui->btn_nutgiao, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->pageNutGiao);
        drawGraph();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("routes.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Lỗi", "Không mở được database!");
        return;
    }

    QSqlQuery q;
    q.exec("CREATE TABLE IF NOT EXISTS routes ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "name TEXT,"
           "length REAL,"
           "start TEXT,"
           "end TEXT)");
}

void MainWindow::loadRoutes()
{
    QSqlQuery query(db);
    if(query.exec("SELECT * FROM routes")) {
        while(query.next()) {
            QString name = query.value("name").toString();
            qDebug() << "Route:" << name;
        }
    } else {
        qDebug() << "Lỗi truy vấn:" << query.lastError().text();
    }
}

void MainWindow::on_btn_add_clicked()
{
    QString name = ui->txt_name->text();
    double length = ui->spin_length->value();
    QString start = ui->cb_start->currentText();
    QString end = ui->cb_end->currentText();

    QSqlQuery q;
    q.prepare("INSERT INTO routes(name,length,start,end) VALUES (?,?,?,?)");
    q.addBindValue(name);
    q.addBindValue(length);
    q.addBindValue(start);
    q.addBindValue(end);
    q.exec();

    loadRoutes();
}

void MainWindow::on_btn_edit_clicked()
{
    auto row = ui->tableRoutes->currentRow();
    if (row < 0) return;

    int id = ui->tableRoutes->item(row, 0)->text().toInt();

    QSqlQuery q;
    q.prepare("UPDATE routes SET name=?, length=?, start=?, end=? WHERE id=?");
    q.addBindValue(ui->txt_name->text());
    q.addBindValue(ui->spin_length->value());
    q.addBindValue(ui->cb_start->currentText());
    q.addBindValue(ui->cb_end->currentText());
    q.addBindValue(id);
    q.exec();

    loadRoutes();
}

void MainWindow::on_btn_delete_clicked()
{
    // TODO: delete logic here
}


void MainWindow::drawGraph()
{
    scene = new QGraphicsScene(this);
    ui->gv_map->setScene(scene);

    QPointF A(0,0), B(150,50), C(80,140);

    scene->addEllipse(A.x(), A.y(), 20, 20, QPen(Qt::black), QBrush(Qt::blue));
    scene->addEllipse(B.x(), B.y(), 20, 20, QPen(Qt::black), QBrush(Qt::blue));
    scene->addEllipse(C.x(), C.y(), 20, 20, QPen(Qt::black), QBrush(Qt::blue));

    scene->addLine(QLineF(A, B));
    scene->addLine(QLineF(B, C));
    scene->addLine(QLineF(C, A));
}
