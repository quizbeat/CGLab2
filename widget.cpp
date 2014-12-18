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

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QVector<NMVector> points;

    float scale = 0.05 * std::min(width(), height());
    float w  = width() / 2.0;
    float h = height() / 2.0;

    if(1 == 2) {
        painter.drawLine(w, h, (2 * w - 20), h);
        painter.drawLine(w, h, 20, height() - h / 8);
        painter.drawLine(w, h, w, 60);
    }



    NMMatrix XZMatrix = NMMatrix();
    NMMatrix YZMatrix = NMMatrix();
    NMMatrix SMatrix  = NMMatrix();
    NMMatrix ResMatrix = NMMatrix();

    XZMatrix.rotateXZ(beta);
    YZMatrix.rotateYZ(alpha);
    SMatrix.setScale(scale);

    ResMatrix = XZMatrix * YZMatrix * SMatrix;
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
