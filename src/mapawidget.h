#ifndef MAPAWIDGET_H
#define MAPAWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGraphicsView>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDirIterator>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QPainter>
#include <QImage>
#include <QHash>
#include <QHashIterator>
#include <QCheckBox>
#include "mapascene.h"
#include "mapaview.h"
#include "mapa.h"
#include "qgraphicstaikoitem.h"

namespace Ui {
class MapaWidget;
}

class MapaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapaWidget(QWidget *parent = nullptr, Mapa *data = nullptr);
    ~MapaWidget();
    Mapa* data;
    MapaView* view;
    void save(QString filename);
    QList<Instrumento> getTaikos();
    void edit();
    void exportToImage(QString filename);

private slots:
    void on_horizontalSlider_sliderMoved(int position);

    void on_add_taiko_clicked();

private:
    int ppm;
    Ui::MapaWidget *ui;
    void createScene();
    void loadList();
    MapaScene *scene;
    QListWidget* instrumentos;
};

#endif // MAPAWIDGET_H
