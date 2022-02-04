#include "HermiteInterpolation.h"

HermiteInterpolation::HermiteInterpolation(QWidget *parent)
    : QMainWindow(parent)
{
    this->points.append(qMakePair(0.0, 0.5));
    this->points.append(qMakePair(1.0, 0.5));
    this->cacu();
    this->setMouseTracking(true);
}

HermiteInterpolation::~HermiteInterpolation()
{
    if (this->hermite != nullptr) {
        delete this->hermite;
        this->hermite = nullptr;
    }
    if (this->x != nullptr) {
        delete[] this->x;
        this->x = nullptr;
    }
    if (this->y != nullptr) {
        delete[] this->y;
        this->y = nullptr;
    }
}

void HermiteInterpolation::cacu()
{
    if (this->hermite != nullptr) {
        delete this->hermite;
        this->hermite = nullptr;
    }
    if (this->x != nullptr) {
        delete[] this->x;
        this->x = nullptr;
    }
    if (this->y != nullptr) {
        delete[] this->y;
        this->y = nullptr;
    }

    if (this->points.size() < 2) {
        return;
    }

    this->x = new double[this->points.size()];
    this->y = new double[this->points.size()];
    for (uint64_t i = 0; i < this->points.size(); i++) {
        memcpy(&(this->x[i]), &(this->points.at(i).first), sizeof(double));
        memcpy(&(this->y[i]), &(this->points.at(i).second), sizeof(double));
    }

    this->hermite = new Hermite(this->x, this->y, this->points.size());
}

void HermiteInterpolation::refr()
{
    if (this->hermite == nullptr) {
        return;
    }
    if (this->x == nullptr) {
        return;
    }
    if (this->y == nullptr) {
        return;
    }
    for (uint64_t i = 0; i < this->points.size(); i++) {
        memcpy(&(this->x[i]), &(this->points.at(i).first), sizeof(double));
        memcpy(&(this->y[i]), &(this->points.at(i).second), sizeof(double));
    }
    this->hermite->caculate();
}

void HermiteInterpolation::resizeEvent(QResizeEvent* event)
{
    this->pix = QPixmap(this->size());
    this->pix.fill(Qt::GlobalColor::white);

    QPainter painter(&(this->pix));

    QPen pen;
    pen.setColor(Qt::GlobalColor::gray);
    pen.setStyle(Qt::PenStyle::DashDotDotLine);
    pen.setJoinStyle(Qt::PenJoinStyle::RoundJoin);
    pen.setCapStyle(Qt::PenCapStyle::RoundCap);
    pen.setWidth(1);
    painter.setPen(pen);

    for (int i = 0; i <= this->lx; i++) {
        QLine line(i / (double)this->lx * this->width(), 0, i / (double)this->lx * this->width(), this->height());
        painter.drawLine(line);
    }
    for (int i = 0; i <= this->ly; i++) {
        QLine line(0, i / (double)this->ly * this->height(), this->width(), i / (double)this->ly * this->height());
        painter.drawLine(line);
    }

    painter.end();
}

void HermiteInterpolation::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    if (this->hermite == nullptr) {
        return;
    }

    QPainter painter(this);

    painter.drawPixmap(this->rect(), this->pix);

    QPen pen;
    pen.setJoinStyle(Qt::PenJoinStyle::RoundJoin);
    pen.setCapStyle(Qt::PenCapStyle::RoundCap);
    pen.setColor(Qt::GlobalColor::blue);
    pen.setStyle(Qt::PenStyle::SolidLine);
    pen.setWidth(3);
    painter.setPen(pen);

    QPolygon poly;
    for (uint64_t i = 0; i < this->width(); i++) {
        double result = this->hermite->result(i / (double)this->width());
        if (result < 0) {
            result = 0;
        }
        if (result > 1) {
            result = 1;
        }
        QPoint point(i, this->height() * result);
        poly.append(point);
    }
    painter.drawPolyline(poly);

    pen.setColor(Qt::GlobalColor::black);
    pen.setWidth(1);
    painter.setPen(pen);

    QBrush brush;
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    brush.setColor(Qt::GlobalColor::red);
    painter.setBrush(brush);

    for (uint64_t i = 0; i < this->points.size(); i++) {
        QPoint point(this->width() * this->points.at(i).first, this->height() * this->points.at(i).second);
        painter.drawEllipse(point, this->pr, this->pr);
    }
}

void HermiteInterpolation::mousePressEvent(QMouseEvent* event)
{
    QPoint&& pos = event->pos();
    if (event->button() == Qt::MouseButton::LeftButton) {
        for (uint64_t i = 0; i < this->points.size(); i++) {
            if (this->distance(QPoint(pos), QPoint(this->points.at(i).first * this->width(), this->points.at(i).second * this->height())) <= this->pr) {
                this->lPressed = true;
                this->pointId = i;
                this->setCursor(Qt::CursorShape::ClosedHandCursor);
                return;
            }
        }
        this->setCursor(Qt::CursorShape::ArrowCursor);
        return;
    }
}

void HermiteInterpolation::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    if (!this->lPressed) {
        for (uint64_t i = 0; i < this->points.size(); i++) {
            if (this->distance(QPoint(pos), QPoint(this->points.at(i).first * this->width(), this->points.at(i).second * this->height())) <= this->pr) {
                this->setCursor(Qt::CursorShape::PointingHandCursor);
                return;
            }
        }
        this->setCursor(Qt::CursorShape::ArrowCursor);
        return;
    }
    else {
        if (this->pointId >= 0 && this->pointId < this->points.size()) {
            if (this->pointId == 0) {
                pos.setX(0);
            }
            else if(this->pointId == this->points.size() - 1){
                pos.setX(this->width());
            }
            else {
                if (pos.x() / (double)this->width() < this->points.at(this->pointId - 1).first) {
                    pos.setX(this->points.at(this->pointId - 1).first * this->width());
                }
                if (pos.x() / (double)this->width() > this->points.at(this->pointId + 1).first) {
                    pos.setX(this->points.at(this->pointId + 1).first * this->width());
                }
            }
            if (pos.y() < 0) {
                pos.setY(0);
            }
            if (pos.y() > this->height()) {
                pos.setY(this->height());
            }
            this->points.replace(this->pointId, qMakePair(pos.x() / (double)this->width(), pos.y() / (double)this->height()));
            this->refr();
            this->update();
        }
        this->setCursor(Qt::CursorShape::ClosedHandCursor);
        return;
    }
}

void HermiteInterpolation::mouseReleaseEvent(QMouseEvent* event)
{
    QPoint&& pos = event->pos();
    if (event->button() == Qt::MouseButton::LeftButton) {
        if (this->lPressed) {
            this->lPressed = false;
            this->pointId = -1;
            this->setCursor(Qt::CursorShape::PointingHandCursor);
            return;
        }
    }
    if (event->button() == Qt::MouseButton::RightButton) {
        if (!this->lPressed) {
            for (uint64_t i = 1; i < this->points.size() - 1; i++) {
                if (this->distance(QPoint(pos), QPoint(this->points.at(i).first * this->width(), this->points.at(i).second * this->height())) <= this->pr) {
                    this->points.removeAt(i);
                    this->cacu();
                    this->update();
                    this->setCursor(Qt::CursorShape::ArrowCursor);
                    return;
                }
            }
        }
    }
}

void HermiteInterpolation::mouseDoubleClickEvent(QMouseEvent* event)
{
    QPoint&& pos = event->pos();
    if (event->button() == Qt::MouseButton::LeftButton) {
        for (uint64_t i = 0; i < this->points.size(); i++) {
            if (this->distance(QPoint(pos), QPoint(this->points.at(i).first * this->width(), this->points.at(i).second * this->height())) <= this->pr) {
                return;
            }
        }
        for (uint64_t i = 0; i < this->points.size() - 1; i++) {
            if (this->points.at(i).first * this->width() <= pos.x() && this->points.at(i + 1).first * this->width() > pos.x()) {
                this->points.insert(i + 1, qMakePair(pos.x() / (double)this->width(), pos.y() / (double)this->height()));
                this->cacu();
                this->update();
                this->setCursor(Qt::CursorShape::PointingHandCursor);
                return;
            }
        }
    }
}

int HermiteInterpolation::distance(QPoint&& p1, QPoint&& p2)
{
    return qSqrt((p2.x() - p1.x()) * (p2.x() - p1.x()) + (p2.y() - p1.y()) * (p2.y() - p1.y()));
}