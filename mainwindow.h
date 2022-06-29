#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "visualizer.h"
#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_delaySlider_valueChanged(int newValue);
    void on_delaySpinBox_valueChanged(int newValue);
    void on_sizeSlider_valueChanged(int newValue);
    void on_sizeSpinBox_valueChanged(int newValue);
    void on_stopButton_clicked();
    void on_shuffleButton_clicked();
    void on_sortButton_clicked();

    void onTaskStart();
    void onTaskFinish();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<Visualizer> visualizer;
};
#endif // MAINWINDOW_H
