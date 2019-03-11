#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    novoMapa = ui->actionNovo_Mapa;
    novoMapa->setShortcut(QKeySequence::New);
    loadMapa = ui->actionCarregar_Mapa;
    loadMapa->setShortcut(QKeySequence::Open);
    saveMapa = ui->actionSalvar_Mapa;
    saveMapa->setShortcut(QKeySequence::Save);
    exportMapa = ui->actionExportar_como_imagem;
    editMapa = ui->actionPropriedades_do_mapa;
    help = ui->actionAjuda;
    help->setShortcut(QKeySequence::HelpContents);
    about = ui->actionSobre;
    setWindowTitle("Mapa De Palco");
    resize(QDesktopWidget().availableGeometry(this).size());
    connect(novoMapa, SIGNAL(triggered()), this, SLOT(criaMapa()));
    connect(loadMapa, SIGNAL(triggered()), this, SLOT(carregaMapa()));
    connect(saveMapa, SIGNAL(triggered()), this, SLOT(salvaMapa()));
    connect(exportMapa, SIGNAL(triggered()), this, SLOT(exportaImagem()));
    connect(editMapa, SIGNAL(triggered()), this, SLOT(propriedadesMapa()));
    connect(help, SIGNAL(triggered()), this, SLOT(showHelp()));
    connect(about, SIGNAL(triggered()), this, SLOT(showAbout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::criaMapa() {
    QDialog* dialog = new QDialog;
    QFormLayout form(dialog);
    QLineEdit* mTitle= new QLineEdit(dialog);
    QLineEdit* mTeam= new QLineEdit(dialog);
    QLineEdit* mCity= new QLineEdit(dialog);
    QDoubleSpinBox* mWidth= new QDoubleSpinBox(dialog);
    QDoubleSpinBox* mHeight = new QDoubleSpinBox(dialog);
    QCheckBox* mRouded = new QCheckBox(dialog);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, dialog);
    form.addRow(QString("Título:"),mTitle);
    form.addRow(QString("Equipe:"),mTeam);
    form.addRow(QString("Cidade:"),mCity);
    form.addRow(QString("Largura:"),mWidth);
    form.addRow(QString("Altura:"),mHeight);
    form.addRow(QString("Frente arredondada:"),mRouded);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    if (dialog->exec() == QDialog::Accepted) {
        Mapa* map = new Mapa(mHeight->value(), mWidth->value(), mRouded->isChecked(), mTitle->text(), mTeam->text(), mCity->text());
        mostraMapa(map);
    }
}

void MainWindow::carregaMapa() {
    Mapa* map;
    QString file_name = QFileDialog::getOpenFileName(nullptr,"Carregar Mapa", "", "Mapas de taiko(*.abt);;Todos os arquivos(*)");
    if ((map=Mapa::carrega_mapa(file_name)) != nullptr) {
        mostraMapa(map);
    }
}

void MainWindow::salvaMapa() {
    if (ui->tabWidget->count() <= 0)
        return;
    QString name = dynamic_cast<MapaWidget*>(ui->tabWidget->currentWidget())->data->titulo;
    QString file_name = QFileDialog::getSaveFileName(nullptr,"Salvar mapa", name+".abt", "Mapas de taiko(*.abt);;Todos os arquivos(*)");
    dynamic_cast<MapaWidget*>(ui->tabWidget->currentWidget())->save(file_name);
}

void MainWindow::exportaImagem() {
    if (ui->tabWidget->count() <= 0)
        return;
    QString name = dynamic_cast<MapaWidget*>(ui->tabWidget->currentWidget())->data->titulo;
    QString file_name = QFileDialog::getSaveFileName(nullptr,"Exportar Para...", name+".png", "Imagens(*.png *.xpm *.jpg);;Todos os arquivos(*)");
    dynamic_cast<MapaWidget*>(ui->tabWidget->currentWidget())->exportToImage(file_name);
}

void MainWindow::propriedadesMapa() {
    if (ui->tabWidget->count() <= 0)
        return;
    dynamic_cast<MapaWidget*>(ui->tabWidget->currentWidget())->edit();
}

void MainWindow::showHelp() {
    QMessageBox* dialog = new QMessageBox;
    dialog->setTextFormat(Qt::RichText);
    dialog->setText("<table>"
                    "<tr>"
                    "<th>Comando</th>"
                    "<th>Ação</th>"
                    "</tr>"
                    "<tr>"
                    "<td>"
                    "Setas/Mouse"
                    "</td>"
                    "<td>"
                    "Move instrumentos"
                    "</td>"
                    "</tr>"
                    "<tr>"
                    "<td>"
                    "Shift+setas/Roda do mouse"
                    "</td>"
                    "<td>"
                    "Rotaciona os instrumentos"
                    "</td>"
                    "</tr>"
                    "<tr>"
                    "<td>"
                    "E"
                    "</td>"
                    "<td>"
                    "Marca/desmarca instrumento como emprestado"
                    "</td>"
                    "</tr>"
                    "<tr>"
                    "<td>"
                    "Ctrl+C"
                    "</td>"
                    "<td>"
                    "Copia"
                    "</td>"
                    "</tr>"
                    "<tr>"
                    "<td>"
                    "Ctrl+V"
                    "</td>"
                    "<td>"
                    "Cola"
                    "</td>"
                    "</tr>"
                    "<tr>"
                    "<td>"
                    "Ctrl+Z"
                    "</td>"
                    "<td>"
                    "Desfaz última ação"
                    "</td>"
                    "</tr>"
                    "<tr>"
                    "<td>"
                    "Ctrl+Shift+Z"
                    "</td>"
                    "<td>"
                    "Refaz ação desfeita"
                    "</td>"
                    "</tr>"
                    "</table>");
    dialog->show();
}

void MainWindow::showAbout() {
    QMessageBox* dialog = new QMessageBox;
    dialog->setTextFormat(Qt::RichText);
    dialog->setText("Software de criação e edição de mapas de palco para taiko.<br>"
                    "<br>"
                    "Criado em parceria de Gabriel Kuribara Lasso com a Associação Brasileira de Taiko (ABT).<br>"
                    "<br>"
                    "Mais iformações em: <a href=https://github.com/GabrielLasso/dondon>https://github.com/GabrielLasso/dondon<\a>");
    dialog->show();
}

void MainWindow::mostraMapa(Mapa* map){
    MapaWidget* mapa = new MapaWidget (nullptr, map);
    ui->tabWidget->addTab(mapa,map->titulo);
    saveMapa->setEnabled(true);
    editMapa->setEnabled(true);
    exportMapa->setEnabled(true);
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
    if (ui->tabWidget->count() <= 0) {
        saveMapa->setEnabled(false);
        editMapa->setEnabled(false);
        exportMapa->setEnabled(false);
    }
}
