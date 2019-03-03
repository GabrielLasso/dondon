#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDoubleSpinBox>
#include <QDialog>
#include <QFormLayout>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QDesktopWidget>
#include <QShortcut>
#include <QKeySequence>
#include "mapa.h"
#include "mapawidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QAction* novoMapa;
    QAction* loadMapa;
    QAction* saveMapa;
    QAction* exportMapa;
    QAction* editMapa;
    void mostraMapa(Mapa* map);

public slots:
    void criaMapa();
    void carregaMapa();
    void salvaMapa();
    void exportaImagem();
    void propriedadesMapa();

private slots:
    void on_tabWidget_tabCloseRequested(int index);
};


#endif // MAINWINDOW_H
