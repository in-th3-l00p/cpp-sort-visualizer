#include "visualizer.h"
#include <QSizePolicy>
#include <QPainter>
#include <QLabel>
#include <random>
#include <cmath>

Visualizer::Visualizer(QWidget *parent, int elements)
    : QWidget(parent)
{
    // ui settings
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(1);
    sizePolicy.setVerticalStretch(1);
    this->setSizePolicy(sizePolicy);
    this->setMinimumSize(Properties::minimumSize);

    // setting up the array
    arr = std::vector<int>(elements);
    for (int i = 0; i < arr.size(); i++)
        arr[i] = i + 1;

    // connecting
    QObject::connect(
        this, &Visualizer::taskStart,
        this, &Visualizer::onTaskStart
    );
    QObject::connect(
        this, &Visualizer::taskFinished,
        this, &Visualizer::onTaskFinished
    );
}

void Visualizer::paintEvent(QPaintEvent *event) {
    QPainter painter{this};
    int canvasWidth = event->rect().width();
    int canvasHeight = event->rect().height();

    // drawing the background
    painter.setBrush(Qt::black);
    painter.setPen(Qt::white);
    painter.drawRect(0, 0, canvasWidth - 1, canvasHeight - 1);

    // drawing the array's elements
    int elementWidth = static_cast<int>(
        ceil(static_cast<double>(canvasWidth) / arr.size())
    );
    int elementUnitHeight = static_cast<int>(
        ceil(static_cast<double>(canvasHeight) / arr.size())
    );

    painter.setBrush(Qt::cyan);
    painter.setPen(Qt::darkBlue);
    for (int i = 0; i < arr.size(); i++) {
        int elementHeight = elementUnitHeight * arr[i];
        painter.drawRect(
            i * elementWidth,
            canvasHeight - elementHeight,
            elementWidth,
            elementHeight
        );
    }
}

void Visualizer::stopTask() {
    mutex.lock();
    currentTask->terminate();
    currentTask->wait();
    mutex.unlock();
    emit taskFinished();
}

void Visualizer::shuffle() {
    if (currentTask)
        throw std::runtime_error("There is a task currently running.");
    emit taskStart(new Tasks::ShuffleTask(arr, delay));
}

void Visualizer::onDelayChange(int newDelay) {
    mutex.lock();
    delay = newDelay;
    mutex.unlock();
}

void Visualizer::onArrayChange() {
    mutex.lock();
    repaint();
    mutex.unlock();
}

void Visualizer::onTaskStart(Tasks::Task* task) {
    mutex.lock();

    currentTask = task;
    QObject::connect(
        currentTask, &Tasks::Task::finished,
        this, &Visualizer::emitTaskFinished
    );
    QObject::connect(
        currentTask, &Tasks::Task::changeDone,
        this, &Visualizer::onArrayChange
    );

    currentTask->start();
    mutex.unlock();
}

void Visualizer::onTaskFinished() {
    mutex.lock();
    delete currentTask;
    currentTask = nullptr;
    mutex.unlock();
}

Tasks::Task::Task(std::vector<int> &array, int &delay)
    : arr(array), delay(delay)
{  }

void Tasks::ShuffleTask::run() {
    std::random_device rd;
    std::mt19937 mt(rd());
    for (int i = arr.size(); i > 0; i--) {
        std::uniform_int_distribution<int> dist(0, i - 1);
        std::swap(arr[i], arr[dist(mt)]);
        emit changeDone();
        msleep(delay);
    }
}












