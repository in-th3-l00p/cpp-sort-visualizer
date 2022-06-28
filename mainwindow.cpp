#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "visualizer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    visualizer = std::make_shared<Visualizer>();
    ui->centralWidget->layout()->addWidget(visualizer.get());

    // handling signals
    QObject::connect(
        ui->delaySlider, &QSlider::valueChanged,
        visualizer.get(), &Visualizer::onDelayChange
    );
    QObject::connect(
        visualizer.get(), &Visualizer::taskStart,
        this, &MainWindow::onTaskStart
    );
    QObject::connect(
        visualizer.get(), &Visualizer::taskFinished,
        this, &MainWindow::onTaskFinish
    );
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_stopButton_clicked() {
    visualizer->stopTask();
}

void MainWindow::on_shuffleButton_clicked() {
    visualizer->shuffle();
}

void MainWindow::on_sortButton_clicked() {
    visualizer->sort(ui->algorithmComboBox->currentText());
}

void MainWindow::on_delaySlider_valueChanged(int value) {
    ui->delaySpinBox->setValue(value);
}

void MainWindow::on_delaySpinBox_valueChanged(int value) {
    ui->delaySlider->setValue(value);
}

void MainWindow::onTaskStart() {
    ui->stopButton->setEnabled(true);
    ui->shuffleButton->setEnabled(false);
    ui->sortButton->setEnabled(false);
}

void MainWindow::onTaskFinish() {
    ui->stopButton->setEnabled(false);
    ui->shuffleButton->setEnabled(true);
    ui->sortButton->setEnabled(true);
}
