#include "widget.h"
#include "ui_widget.h"
#include "nmmatrix.h"
#include <ctime>

/* Init properties */
Widget::Widget(QWidget *parent):QWidget(parent), ui(new Ui::Widget) {
    lastX = 0;
    lastY = 0;
    alpha = 0;
    beta  = 0;

    prismN = 3;
    prismR = 4;
    prismH = 4;

    pyramidR = 4;
    pyramidH = 4;

    selectedFigure = PRISM;

    ui->setupUi(this);
}

Widget::~Widget() {
    delete ui;
}

NMVector prismPoint(double phi, double prismR, double prismH) {
    NMVector result = NMVector();
    result.x = prismR * cos(phi);
    result.y = prismR * sin(phi);
    result.z = prismH;
    return result;
}

NMVector pyramidPoint(double phi, double pyramidR, double pyramidH) {
    NMVector result = NMVector();
    result.x = pyramidR * cos(phi);
    result.y = pyramidR * sin(phi);
    result.z = pyramidH;
    return result;
}

void Widget::paintEvent(QPaintEvent *) {

    double max;
    if (selectedFigure == PRISM) {
        max = std::max(1.5 * prismH,
                       1.5 * prismR);
    }
    else {
        max = std::max(1.5 * pyramidH,
                       1.5 * pyramidR);
    }

    double scale = std::min(height() / (2 * max),
                            width() / (2 * max));

    QPainter painter(this);
    QVector<NMVector> points;

    NMMatrix scaleMatrix = NMMatrix();
    NMMatrix resMatrix   = NMMatrix();

    /* Screen center */
    NMVector centerPoint = NMVector(width() / 2.0,
                                   height() / 2.0 + 30, 
                                   1, 
                                   1);

    NMVector normal   = NMVector();
    NMVector camPoint = NMVector(0, 0, -1, 1);

    painter.setRenderHint(QPainter::Antialiasing, true);

    NMMatrix XZMatrix = NMMatrix();
    NMMatrix YZMatrix = NMMatrix();

    XZMatrix.rotateXZ(beta);
    YZMatrix.rotateYZ(alpha);
    scaleMatrix.setScale(scale);

    resMatrix = XZMatrix * YZMatrix;

    bool bottom = false;
    bool top = false;

    if (selectedFigure == PRISM) {
        
        double step = 2 * M_PI / prismN;
        for (double phi = 0; phi < 2 * M_PI; phi += step) {
             points.push_back(prismPoint(phi, prismR, -prismH / 2));
             points.push_back(prismPoint(phi + step, prismR, -prismH / 2));
             points.push_back(prismPoint(phi + step, prismR, prismH / 2));
             points.push_back(prismPoint(phi, prismR, prismH / 2));
        }

        int size = points.size();
        for (int i = 0; i < size; i++) {
           points[i] = scaleMatrix * points[i];
           points[i] = resMatrix * points[i];
           points[i] = points[i] + centerPoint;
        }

        normal = NMVector::crossProduct(points[5] - points[4], points[1] - points[0]);
        if (NMVector::dotProduct(normal, camPoint) >= 0) {
            bottom = true;
        }

        normal = NMVector::crossProduct(points[3] - points[2], points[7] - points[6]);
        if (NMVector::dotProduct(normal, camPoint) >= 0) {
            top = true;
        }

        for (int i = 0; i < size; i += 4) {

            normal = NMVector::crossProduct(points[i + 1] - points[i],
                                       points[i + 3] - points[i]);
            if (NMVector::dotProduct(normal, camPoint) >= 0) {
                painter.drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
                painter.drawLine(points[i + 1].x, points[i + 1].y, points[i + 2].x, points[i + 2].y);
                painter.drawLine(points[i + 2].x, points[i + 2].y, points[i + 3].x, points[i + 3].y);
                painter.drawLine(points[i + 3].x, points[i + 3].y, points[i].x, points[i].y);
            }

            if (bottom) {
                painter.drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
            }
            if (top) {
                painter.drawLine(points[i + 2].x, points[i + 2].y, points[i + 3].x, points[i + 3].y);
            }
        }
    }

    else if (selectedFigure == PYRAMID) {

        double step = 2 * M_PI / 6;
        for(double phi = 0; phi < 2 * M_PI; phi += step) {
              points.push_back(pyramidPoint(phi, pyramidR, -pyramidH / 2));
              points.push_back(pyramidPoint(phi + step, pyramidR, -pyramidH / 2));
              points.push_back(pyramidPoint(phi + step / 2, 0, pyramidH / 2));
         }

        for(double phi = 0; phi < 2 * M_PI; phi += step) {
              points.push_back(pyramidPoint(phi + step, pyramidR, -pyramidH / 2));
              points.push_back(pyramidPoint(phi, pyramidR, -pyramidH / 2));
              points.push_back(pyramidPoint(phi + step / 2, 0, -pyramidH / 2));
         }

        int size = points.size();
        for (int i = 0; i < size; i++) {
            points[i] = scaleMatrix * points[i];
            points[i] = resMatrix * points[i];
            points[i] = points[i] + centerPoint;
        }
        for (int i = 0; i < size; i += 3) {
            normal = NMVector::crossProduct(points[i + 1] - points[i],
                                  points[i + 2] - points[i]);

            if(NMVector::dotProduct(normal, camPoint) >= 0) {
                painter.drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
                painter.drawLine(points[i + 1].x, points[i + 1].y, points[i + 2].x, points[i + 2].y);
                painter.drawLine(points[i + 2].x, points[i + 2].y, points[i].x, points[i].y);
            }
        }
    }

    /* Draw axis */
    if (showAxis) {
        QVector<NMVector> axis;
        axis.push_back(NMVector(0, 0, 0, 1));
        axis.push_back(NMVector(10, 0, 0, 1));
        axis.push_back(NMVector(0, 10, 0, 1));
        axis.push_back(NMVector(0, 0, 10, 1));

        axis[0] = axis[0] + centerPoint;

        for (int i = 1; i < 4; i++) {
           axis[i] = scaleMatrix * axis[i];
           axis[i] = resMatrix * axis[i];
           axis[i] = axis[i] + centerPoint;
        }

        painter.drawLine(axis[0].x, axis[0].y, axis[1].x, axis[1].y);
        painter.drawLine(axis[0].x, axis[0].y, axis[2].x, axis[2].y);
        painter.drawLine(axis[0].x, axis[0].y, axis[3].x, axis[3].y);
    }
}

void Widget::mousePressEvent(QMouseEvent *mEvent) {
    lastX = mEvent->x();
    lastY = mEvent->y();
}

void Widget::mouseMoveEvent(QMouseEvent *mEvent) {
    beta  -= (mEvent->x() - lastX) / 111;
    alpha += (mEvent->y() - lastY) / 111;
    lastX = mEvent->x();
    lastY = mEvent->y();
    update();
}

void Widget::on_quitButton_clicked() {
    close();
}

void Widget::on_prismNParameter_valueChanged(int newValue) {
    prismN = newValue;
    update();
}

void Widget::on_prismRParameter_valueChanged(double newValue) {
    prismR = newValue;
    update();
}

void Widget::on_prismHParameter_valueChanged(double newValue) {
    prismH = newValue;
    update();
}

void Widget::on_pyramidRParameter_valueChanged(double newValue) {
    pyramidR = newValue;
    update();
}

void Widget::on_pyramidHParameter_valueChanged(double newValue) {
    pyramidH = newValue;
    update();
}

void Widget::on_tabWidget_currentChanged(int index) {
    if (index == 0) {
        selectedFigure = PRISM;
    }
    else if (index == 1) {
        selectedFigure = PYRAMID;
    }
    else {
        qDebug() << "Figure selection error." << endl;
    }
    update();
}

void Widget::on_showAxis_toggled(bool checked) {
    if (checked) {
        showAxis = true;
    }
    else {
        showAxis = false;
    }
    update();
}
