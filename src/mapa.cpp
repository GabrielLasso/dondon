#include "mapa.h"

Mapa::Mapa(double height, double width, QString titulo, QString equipe, QString cidade, QList<Instrumento>* taikos)
{
    this->taikos = taikos;
    this->height = height;
    this->titulo = titulo;
    this->equipe = equipe;
    this->cidade = cidade;
    this->width = width;
}

Mapa* Mapa::carrega_mapa(QString file_name) {
    QFile file(file_name);
    if(!file.open(QIODevice::ReadOnly)) {
        return nullptr;
    }

    double height, width, x, y, alpha;
    bool emprestado;
    QTextStream in(&file);
    QString line, image, titulo, equipe, cidade;
    QStringList fields;
    line = in.readLine();
    titulo = line;
    line = in.readLine();
    equipe = line;
    line = in.readLine();
    cidade = line;
    line = in.readLine();
    fields = line.split(",");
    height = fields[0].toDouble();
    width = fields[1].toDouble();
    QList<Instrumento> *taikos = new QList<Instrumento>;
    while(!in.atEnd()) {
        line = in.readLine();
        fields = line.split(",");
        x = fields[0].toDouble();
        y = fields[1].toDouble();
        alpha = fields[2].toDouble();
        image = fields[3];
        emprestado = false;
        if (fields.size() > 4) {
            emprestado = (fields[4] != "0");
        }
        taikos->append(Instrumento(x, y, alpha, image, emprestado));
    }

    file.close();
    return new Mapa(height, width, titulo, equipe, cidade, taikos);
}
