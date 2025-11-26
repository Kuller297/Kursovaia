#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTableWidget;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void addProduct();
    void deleteProduct();
    void editProduct();
    void help();

private:
    void setupUI();
    void setupTable();

    QTableWidget *table;
};

#endif
