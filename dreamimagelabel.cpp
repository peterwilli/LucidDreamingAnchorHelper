#include "dreamimagelabel.h"

DreamImageLabel::DreamImageLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {

}

DreamImageLabel::~DreamImageLabel() {}

void DreamImageLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}
