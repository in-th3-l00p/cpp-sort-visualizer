#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QWidget>
#include <QObject>
#include <QPaintEvent>
#include <QSize>
#include <vector>

namespace Properties {
    constexpr QSize minimumSize{411, 571};
}

class Visualizer : public QWidget
{
    Q_OBJECT
public:
    explicit Visualizer(QWidget *parent = nullptr, const int elements = 20);
    void paintEvent(QPaintEvent* event);

    //getters and setters


private:
    std::vector<int> arr;
};

#endif // VISUALIZER_H
