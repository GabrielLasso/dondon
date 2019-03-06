#ifndef MAPA_H
#define MAPA_H

#include <QWidget>
#include <QGraphicsView>
#include <QPainter>
#include <QList>
#include <QTextStream>
#include <QString>
#include "instrumento.h"

class Mapa
{
public:
    explicit Mapa(double height = 0, double width = 0, bool roundedStage = false, QString titulo = "",QString equipe = "", QString cidade = "", QList<Instrumento>* taikos = new QList<Instrumento>);
    double height, width;
    bool roundedStage;
    QList<Instrumento>* taikos;
    QString titulo;
    QString equipe;
    QString cidade;
    static Mapa* carrega_mapa(QString file);
signals:

private:

};

#endif // MAPA_H
