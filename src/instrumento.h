#ifndef INSTRUMENTO_H
#define INSTRUMENTO_H

#include <QPixmap>

class Instrumento
{
public:
    Instrumento(double x=0, double y=0, double angle=0, QString filename="", bool emprestado = false);
    qreal x, y, angle;
    QString filename;
    bool emprestado;
};

#endif // INSTRUMENTO_H
