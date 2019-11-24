#ifndef DREAMIMAGELABEL_H
#define DREAMIMAGELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class DreamImageLabel : public QLabel {
    Q_OBJECT

public:
    explicit DreamImageLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~DreamImageLabel();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);

};


#endif // DREAMIMAGELABEL_H
