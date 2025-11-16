#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_add_clicked();
    void on_btn_edit_clicked();

private:
    void initDatabase();
    void loadRoutes();
    void drawGraph();
    void on_btn_delete_clicked();
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
