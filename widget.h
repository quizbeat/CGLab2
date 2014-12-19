#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPointF>
#include <QDebug>
#include <QMouseEvent>
#include <cmath>

namespace Ui {
	class Widget;
}

class Widget : public QWidget {
    Q_OBJECT

public:
    /* Mouse moves properties */
    float lastX, lastY;
    float alpha, beta;

    /* Prism propertes */
    int   prismN;
    float prismR, prismH;

    /* Pyramid properties */
    float pyramidR, pyramidH;

    /* Common attributes */
    explicit Widget(QWidget *parent = 0);
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *mEvent);
    virtual void mouseMoveEvent(QMouseEvent *mEvent);
    ~Widget();

    int selectedFigure;
    bool showAxis;

    typedef enum {
        PRISM,
        PYRAMID
    }figure;

private slots:
    void on_quitButton_clicked();
    void on_prismNParameter_valueChanged(int newValue);
    void on_prismRParameter_valueChanged(double newValue);
    void on_prismHParameter_valueChanged(double newValue);
    void on_pyramidRParameter_valueChanged(double newValue);
    void on_pyramidHParameter_valueChanged(double newValue);
    void on_tabWidget_currentChanged(int index);
    void on_showAxis_toggled(bool checked);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
