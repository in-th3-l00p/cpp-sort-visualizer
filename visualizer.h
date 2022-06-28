#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QWidget>
#include <QObject>
#include <QPaintEvent>
#include <QSize>
#include <QThread>
#include <QMutex>
#include <vector>

namespace Constants {
    constexpr QSize minimumSize{411, 571};
    constexpr int defaultInitialDelay = 100, defaultInitialElements = 20;
    const inline std::string
        taskRunningError = "There is a task that is currently running",
        indexOutOfBoundsError = "Index is out of bounds";
    const inline QString
        bubbleSortID = "Bubble sort",
        selectionSortID = "Selection sort",
        insertionSortID = "Insertion sort",
        mergeSortID = "Merge sort",
        quickSortID = "Quick sort",
        radixSortID = "Radix sort";
}

namespace Tasks {
    // task parent class
    class Task: public QThread {
        Q_OBJECT

    protected:
        std::vector<int>& arr;
        int& delay;

        void checkIndex(int i);
        void set(int i, int val); // change an element
        void swap(int i, int j); // swaps two elements inside the arr

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

    class BubbleSort: public Task {
    public:
        using Task::Task;
        void run() override;
    };

    class SelectionSort: public Task {
    public:
        using Task::Task;
        void run() override;
    };

    class InsertionSort: public Task {
    public:
        using Task::Task;
        void run() override;
    };

    class MergeSort: public Task {
    public:
        using Task::Task;
        void run() override;
    private:
        void mergeSort(int lo, int hi);
    };

    class QuickSort: public Task {
    public:
        using Task::Task;
        void run() override;
    private:
        void quickSort(int lo, int hi);
    };

    class RadixSort: public Task {
    public:
        using Task::Task;
        void run() override;
    };
}

class Visualizer : public QWidget {
    Q_OBJECT
public:
    explicit Visualizer(
            QWidget *parent = nullptr,
            const int initialElements = Constants::defaultInitialElements,
            int initialDelay = Constants::defaultInitialDelay
    );
    void paintEvent(QPaintEvent* event);

    // implementing tasks
    void stopTask();
    void shuffle();
    void sort(const QString& algorithm);

private:
    std::vector<int> arr;
    int delay;
    QMutex mutex;

    Tasks::Task* currentTask = nullptr;
    void checkTaskRunning(); // throws an exception if there is a task running

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
