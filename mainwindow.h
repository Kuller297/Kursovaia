#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;  // Добавлено

private slots:
    void addProduct();
    void deleteProduct();
    void editProduct();
    void showHelp();
    void saveToFile();
    void loadFromFile();

private:
    Ui::MainWindow *ui;
    bool dataChanged = false;

    void markDataChanged();
    void markDataSaved();
};

#endif // MAINWINDOW_H
