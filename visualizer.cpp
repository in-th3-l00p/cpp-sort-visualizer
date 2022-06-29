#include "visualizer.h"
#include <QSizePolicy>
#include <QPainter>
#include <QLabel>
#include <random>

Visualizer::Visualizer(QWidget *parent, int initialElements, int initialDelay)
    : QWidget(parent), delay(initialDelay)
{
    // ui settings
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(1);
    sizePolicy.setVerticalStretch(1);
    this->setSizePolicy(sizePolicy);
    this->setMinimumSize(Constants::minimumSize);

    // setting up the array
    arr = std::vector<int>(initialElements);
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

    // finding the best dimensions
    double elementWidth = static_cast<double>(canvasWidth) / arr.size();
    double elementUnitHeight = static_cast<double>(canvasHeight) / arr.size();

    // drawing the array's elements
    painter.setBrush(Qt::cyan);
    painter.setPen(Qt::darkBlue);
    for (int i = 0; i < arr.size(); i++) {
        double elementHeight = elementUnitHeight * arr[i];
        QRectF rectangle{
            i * elementWidth,
            canvasHeight - elementHeight,
            elementWidth,
            elementHeight
        };
        painter.drawRect(rectangle);
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
    checkTaskRunning();
    emit taskStart(new Tasks::ShuffleTask(arr, delay));
}

void Visualizer::sort(const QString &algorithm) {
    checkTaskRunning();
    if (algorithm == Constants::bubbleSortID)
        emit taskStart(new Tasks::BubbleSort(arr, delay));
    else if (algorithm == Constants::selectionSortID)
        emit taskStart(new Tasks::SelectionSort(arr, delay));
    else if (algorithm == Constants::insertionSortID)
        emit taskStart(new Tasks::InsertionSort(arr, delay));
    else if (algorithm == Constants::mergeSortID)
        emit taskStart(new Tasks::MergeSort(arr, delay));
    else if (algorithm == Constants::quickSortID)
        emit taskStart(new Tasks::QuickSort(arr, delay));
    else if (algorithm == Constants::radixSortID)
        emit taskStart(new Tasks::RadixSort(arr, delay));
}

void Visualizer::checkTaskRunning() {
    if (currentTask)
        throw std::runtime_error(Constants::taskRunningError);
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

void Visualizer::onArrayResize(int newSize) {
    checkTaskRunning();
    int lastSize = arr.size();
    arr.resize(newSize);
    if (lastSize < newSize)
        for (int i = lastSize; i < arr.size(); i++)
            arr[i] = i + 1;
    repaint();
}

void Tasks::Task::checkIndex(int i) {
    if (i < 0 || i >= arr.size())
        throw std::runtime_error(Constants::indexOutOfBoundsError);
}

void Tasks::Task::set(int i, int val) {
    checkIndex(i);
    arr[i] = val;
    emit changeDone();
    msleep(delay);
}

void Tasks::Task::swap(int i, int j) {
    checkIndex(i);
    checkIndex(j);
    std::swap(arr[i], arr[j]);
    emit changeDone();
    msleep(delay);
}

Tasks::Task::Task(std::vector<int> &array, int &delay)
    : arr(array), delay(delay)
{  }

void Tasks::ShuffleTask::run() {
    std::random_device rd;
    std::mt19937 mt(rd());
    for (int i = arr.size() - 1; i > 0; i--) {
        std::uniform_int_distribution<int> dist(0, i - 1);
        swap(i, dist(mt));
    }
}

void Tasks::BubbleSort::run() {
    for (int i = 1; i < arr.size() - 1; i++)
        for (int j = 0; j < arr.size() - i; j++)
            if (arr[j] > arr[j + 1])
                swap(j, j + 1);
}

void Tasks::SelectionSort::run() {
    for (int i = 0; i < arr.size() - 1; i++) {
        int minValue = arr[i], minIndex = i;
        for (int j = i + 1; j < arr.size(); j++)
            if (arr[j] < minValue) {
                minValue = arr[j];
                minIndex = j;
            }
        swap(i, minIndex);
    }
}

void Tasks::InsertionSort::run() {
    for (int i = 1; i < arr.size(); i++) {
        int key = arr[i], j = i - 1;
        while (j >= 0 && arr[j] > key) {
            set(j + 1, arr[j]);
            j--;
        }

        set(j + 1, key);
        emit changeDone();
    }
}

void Tasks::MergeSort::run() {
    mergeSort(0, arr.size() - 1);
}

void Tasks::MergeSort::mergeSort(int lo, int hi) {
    // divide et impera
    if (hi <= lo)
        return;
    int mid = lo + (hi - lo) / 2;
    mergeSort(lo, mid);
    mergeSort(mid + 1, hi);

    // merge algorithm
    int i = lo, j = mid + 1;
    std::vector<int> merged;
    while (i <= mid && j <= hi)
        if (arr[i] < arr[j])
            merged.push_back(arr[i++]);
        else
            merged.push_back(arr[j++]);
    while (i <= mid)
        merged.push_back(arr[i++]);
    while (j <= hi)
        merged.push_back(arr[j++]);

    // copying
    for (i = lo, j = 0; i <= hi; i++, j++)
        set(i, merged[j]);
}

void Tasks::QuickSort::run() {
    quickSort(0, arr.size() - 1);
}

void Tasks::QuickSort::quickSort(int lo, int hi) {
    if (hi <= lo)
        return;

    // partition
    int pivot = arr[lo];
    int left = lo, right = hi;
    while (left < right) {
        while (pivot >= arr[left])
            left++;
        while (pivot < arr[right])
            right--;
        if (left < right)
            swap(left, right);
    }
    swap(lo, right);

    // divide et impera
    quickSort(lo, right);
    quickSort(right + 1, hi);
}

void Tasks::RadixSort::run() {
    // getting the maximum number
    int maxi = arr[0];
    for (auto& val: arr)
        maxi = std::max(val, maxi);

    // couting sort for every digit
    for (int exp = 1; maxi / exp; exp *= 10) {
        int count[10] = {0};
        for (auto& val: arr)
            count[val / exp % 10]++;
        for (int i = 1; i < 10; i++)
            count[i] += count[i - 1];

        // building the output array
        std::vector<int> output(arr.size());
        for (int i = arr.size() - 1; i >= 0; i--) {
            output[count[arr[i] / exp % 10] - 1] = arr[i];
            count[arr[i] / exp % 10]--;
        }

        for (int i = 0; i < arr.size(); i++)
            set(i, output[i]);
    }
}










