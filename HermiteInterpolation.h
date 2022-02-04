#pragma once

#include <QtWidgets/QMainWindow>
#include <QVector>
#include <QMap>
#include <QPainter>
#include <QMouseEvent>
#include <QPixmap>
#include "Hermite.h"

class HermiteInterpolation final : public QMainWindow
{
    Q_OBJECT

public:
    HermiteInterpolation(QWidget *parent = Q_NULLPTR);
    ~HermiteInterpolation();

private:
    QVector<QPair<double, double>> points;
    Hermite* hermite = nullptr;
    double* x = nullptr;
    double* y = nullptr;

    void cacu();
    void refr();

    const int pr = 4;
    const int lx = 40;
    const int ly = 20;

    bool lPressed = false;
    uint64_t pointId = -1;

    int distance(QPoint&& p1, QPoint&& p2);

    QPixmap pix = QPixmap(1, 1);
protected:
    void resizeEvent(QResizeEvent* event)override;
    void paintEvent(QPaintEvent* event)override;
    void mousePressEvent(QMouseEvent* event)override;
    void mouseMoveEvent(QMouseEvent* event)override;
    void mouseReleaseEvent(QMouseEvent* event)override;
    void mouseDoubleClickEvent(QMouseEvent* event)override;
};
