#include "widget.h"
#include "ui_widget.h"
#include "nmmatrix.h"
#include <QVector3D>
#include <ctime>

const int R = 5;
const int r = 3;

Widget::Widget(QWidget *parent):QWidget(parent), ui(new Ui::Widget) {
    lastX = 0;
    lastY = 0;
    alpha = 0;
    beta  = 0;
    ui->setupUi(this);
}

Widget::~Widget() {
    delete ui;
}

NMVector torusPoint(float phi, float psi) {
    return NMVector((R + r * cos(phi)) * cos(psi),
                    (R + r * cos(phi)) * sin(psi),
                     r * sin(phi),
                     0);
}

QVector3D trans(NMVector &v) {
    return QVector3D(v.x, v.y, v.z);
}

void Widget::paintEvent(QPaintEvent *) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QVector<NMVector> points;
    float scale = 0.05 * std::min(width(), height());
    float w  = width() / 2.0;
    float h = height() / 2.0;

    NMMatrix XZMatrix = NMMatrix();
    NMMatrix YZMatrix = NMMatrix();
    NMMatrix SMatrix  = NMMatrix();
    NMMatrix ResMatrix = NMMatrix();

    XZMatrix.RotateXZ(beta);
    YZMatrix.RotateYZ(alpha);
    SMatrix.SetScale(scale);

    ResMatrix = XZMatrix * YZMatrix * SMatrix;

    float d = M_PI / 30;

    for (float phi = 0; phi < 2 * M_PI; phi += d) {
        for (float psi = 0; psi < 2 * M_PI; psi += d) {
            points.push_back(ResMatrix * torusPoint(phi, psi));
            points.push_back(ResMatrix * torusPoint(phi + d, psi));
            points.push_back(ResMatrix * torusPoint(phi + d, psi + d));
            points.push_back(ResMatrix * torusPoint(phi, psi + d));
        }
    }
    int size = points.size();
    points.push_back(points[0]);
    points.push_back(points[1]);
    for (int i = 0; i < size; i += 4) {

        QVector3D vec1 = trans(points[i + 1]) - trans(points[i]);
        QVector3D vec2 = trans(points[i + 2]) - trans(points[i]);
        QVector3D n    = QVector3D::crossProduct(vec1, vec2);

        if (QVector3D::dotProduct(n, QVector3D(0, 0, -1)) >= 0) continue;

        painter.drawLine(points[i].x + w, points[i].y + h, points[i + 1].x + w, points[i + 1].y + h);
        painter.drawLine(points[i + 1].x + w, points[i + 1].y + h, points[i + 2].x + w, points[i + 2].y + h);
        painter.drawLine(points[i + 2].x + w, points[i + 2].y + h, points[i + 3].x + w, points[i + 3].y + h);
        painter.drawLine(points[i + 3].x + w, points[i + 3].y + h, points[i].x + w, points[i].y + h);
    }
}

void Widget::on_quitButton_clicked()         { close();  }
void Widget::on_changeParamA_valueChanged()  { update(); }
void Widget::on_changeParamD_valueChanged()  { update(); }
void Widget::on_changeParamK_valueChanged()  { update(); }
void Widget::on_changeParamDb_valueChanged() { update(); }
void Widget::on_changeXY_valueChanged()      { update(); }
void Widget::on_changeXZ_valueChanged()      { update(); }
void Widget::on_changeYZ_valueChanged()      { update(); }
void Widget::on_checkBox_clicked()           { update(); }

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
