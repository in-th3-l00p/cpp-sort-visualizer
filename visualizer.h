#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QWidget>
#include <QObject>
#include <QPaintEvent>
#include <QSize>
#include <QThread>
#include <QMutex>
#include <vector>

namespace Properties {
    constexpr QSize minimumSize{411, 571};
}

namespace Tasks {
    // task parent class
    class Task: public QThread {
        Q_OBJECT

    protected:
        std::vector<int>& arr;
        int& delay;

    public:
        Task(std::vector<int>& array, int& delay);
        virtual void run() override {};

    signals:
        void changeDone();
    };

    class ShuffleTask: public Task {
    public:
        using Task::Task;
        void run() override;
    };
}

class Visualizer : public QWidget {
    Q_OBJECT
public:
    explicit Visualizer(QWidget *parent = nullptr, const int elements = 20);
    void paintEvent(QPaintEvent* event);

    // implementing tasks
    void stopTask();
    void shuffle();

private:
    std::vector<int> arr;
    int delay;
    QMutex mutex;

    Tasks::Task* currentTask = nullptr;

signals:
    // using signals and slots so the ui can be updated based on the task system
    void taskStart(Tasks::Task* task);
    void taskFinished();

public slots:
    void onDelayChange(int newDelay);
    void onArrayChange();
    void onTaskStart(Tasks::Task* task);
    void onTaskFinished();
    void emitTaskFinished() { emit taskFinished(); }
};

#endif // VISUALIZER_H
