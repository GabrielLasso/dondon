#include "mapascene.h"

MapaScene::MapaScene(QObject* parent):QGraphicsScene(parent)
{
    undo_stack = new QUndoStack(parent);
}

void MapaScene::keyPressEvent(QKeyEvent * keyEvent)
{
    QList<QGraphicsItem*> selectedItems = this->selectedItems();
    switch(keyEvent->key()) {
    case Qt::Key_Up:
        if (!selectedItems.empty()) {
            QGraphicsTaikoItem* item = static_cast<QGraphicsTaikoItem*>(selectedItems.first());
            QSet<QGraphicsItem*> items = QSet<QGraphicsItem*>::fromList(selectedItems);
            emit item->moved(items, 0, -1);
        }
        break;
    case Qt::Key_Down:
        if (!selectedItems.empty()) {
            QGraphicsTaikoItem* item = static_cast<QGraphicsTaikoItem*>(selectedItems.first());
            QSet<QGraphicsItem*> items = QSet<QGraphicsItem*>::fromList(selectedItems);
            emit item->moved(items, 0, 1);
        }
        break;
    case Qt::Key_Left:
        if (keyEvent->modifiers().testFlag(Qt::ShiftModifier)) {
            if (!selectedItems.empty()) {
                QGraphicsTaikoItem* item = static_cast<QGraphicsTaikoItem*>(selectedItems.first());
                QSet<QGraphicsItem*> items = QSet<QGraphicsItem*>::fromList(selectedItems);
                emit item->rotated(items, -5);
            }
        } else {
            if (!selectedItems.empty()) {
                QGraphicsTaikoItem* item = static_cast<QGraphicsTaikoItem*>(selectedItems.first());
                QSet<QGraphicsItem*> items = QSet<QGraphicsItem*>::fromList(selectedItems);
                emit item->moved(items, -1, 0);
            }
        }
        break;
    case Qt::Key_Right:
        if (keyEvent->modifiers().testFlag(Qt::ShiftModifier)) {
            if (!selectedItems.empty()) {
                QGraphicsTaikoItem* item = static_cast<QGraphicsTaikoItem*>(selectedItems.first());
                QSet<QGraphicsItem*> items = QSet<QGraphicsItem*>::fromList(selectedItems);
                emit item->rotated(items, 5);
            }
        } else {
            if (!selectedItems.empty()) {
                QGraphicsTaikoItem* item = static_cast<QGraphicsTaikoItem*>(selectedItems.first());
                QSet<QGraphicsItem*> items = QSet<QGraphicsItem*>::fromList(selectedItems);
                emit item->moved(items, 1, 0);
            }
        }
        break;
    case Qt::Key_Delete:
        foreach(QGraphicsItem* item, selectedItems)
        {
            removeItem(item);
            undo_stack->push(new CommandDelete(this, dynamic_cast<QGraphicsTaikoItem*>(item)));
        }
        updateScene(data, ppm);
    break;
    case Qt::Key_C:
        if (keyEvent->modifiers().testFlag(Qt::ControlModifier)) {
            copy();
        }
    break;
    case Qt::Key_V:
        if (keyEvent->modifiers().testFlag(Qt::ControlModifier)) {
            paste();
        }
    break;
    case Qt::Key_Z:
        if (keyEvent->modifiers().testFlag(Qt::ControlModifier)) {
            if (keyEvent->modifiers().testFlag(Qt::ShiftModifier)) {
                undo_stack->redo();
            } else {
                undo_stack->undo();
            }
            updateScene(data, ppm);
        }
    break;
    case Qt::Key_E:
        foreach(QGraphicsItem* item, selectedItems)
        {
            QGraphicsTaikoItem* taiko = static_cast<QGraphicsTaikoItem*>(item);
            taiko->data.emprestado = !taiko->data.emprestado;
        }
        updateScene(data, ppm);
    break;
    }
}

void MapaScene::updateScene(Mapa* data, int ppm) {
    int i,j;
    QPen pen3(QColor(0,0,0));
    QPen pen5(QColor(0,0,0));
    QList<QGraphicsTaikoItem*> taikos = getTaikoItems();
    this->data = data;
    this->ppm = ppm;

    removeItemsAndClearBackground();
    pen3.setWidth(3);
    pen5.setWidth(5);

    // Header
    QGraphicsTextItem *title = addText(data->titulo);
    QGraphicsTextItem *titleHUD = addText("Nome da música");
    title->setPos((200+ppm+titleHUD->boundingRect().width()-title->boundingRect().width())/2, -(data->height/2+1)*ppm);
    titleHUD->setPos(-data->width/2*ppm, -(data->height/2+1)*ppm);
    addRect(-data->width/2*ppm,-(data->height/2+1)*ppm,data->width*ppm+200+ppm,24,pen3);
    addLine(-data->width/2*ppm+titleHUD->boundingRect().width(), -(data->height/2+1)*ppm+24, -data->width/2*ppm+titleHUD->boundingRect().width(), -(data->height/2+1)*ppm);

    QGraphicsTextItem *team = addText(data->equipe);
    QGraphicsTextItem *teamHUD = addText("Equipe");
    team->setPos(((200+ppm)/2-data->width*ppm/2+teamHUD->boundingRect().width()-team->boundingRect().width())/2, -(data->height/2+1)*ppm-24);
    teamHUD->setPos(-data->width/2*ppm, -(data->height/2+1)*ppm-24);
    addRect(-data->width/2*ppm,-(data->height/2+1)*ppm-24,data->width*ppm/2+(200+ppm)/2,24,pen3);
    addLine(-data->width/2*ppm+teamHUD->boundingRect().width(), -(data->height/2+1)*ppm-24, -data->width/2*ppm+teamHUD->boundingRect().width(), -(data->height/2+1)*ppm);

    QGraphicsTextItem *city = addText(data->cidade);
    QGraphicsTextItem *cityHUD = addText("Cidade");
    city->setPos((200+ppm)/2+cityHUD->boundingRect().width() + ((200+ppm*(1+data->width))/2-cityHUD->boundingRect().width())/2-city->boundingRect().width()/2, -(data->height/2+1)*ppm-24);
    cityHUD->setPos((200+ppm)/2,-(data->height/2+1)*ppm-24);
    addRect((200+ppm)/2,-(data->height/2+1)*ppm-24,data->width*ppm/2+(200+ppm)/2,24,pen3);
    addLine((200+ppm)/2+cityHUD->boundingRect().width(), -(data->height/2+1)*ppm-24, (200+ppm)/2+cityHUD->boundingRect().width(), -(data->height/2+1)*ppm);

    // Grid
    bool roundedStage = data->roundedStage;
    addLine(-data->width/2*ppm+2,0,data->width/2*ppm-2,0,pen5);
    addLine(0,-data->height/2*ppm+2,0,data->height/2*ppm-2,pen5);
    for (i=-int(data->height/2);i<=data->height/2;i++){
        if (roundedStage && i+1 > data->height/2) {
            QPainterPath* path = new QPainterPath;
            path->arcMoveTo(-data->width/2*ppm,(data->height/2-2)*ppm,data->width*ppm,2*ppm,0);
            path->arcTo(-data->width/2*ppm,(data->height/2-2)*ppm,data->width*ppm,2*ppm, 0,-180);
            addPath(*path);
        } else {
            if (i%2==0)
                addLine(-data->width*ppm/2+1,i*ppm,data->width*ppm/2-1,i*ppm, pen3);
            else
                addLine(-data->width*ppm/2,i*ppm,data->width*ppm/2,i*ppm);
        }
    }
    for(j=-int(data->width/2);j<=data->width/2;j++){
        if (roundedStage) {
            if (j%2==0)
                addLine(j*ppm,-data->height*ppm/2+1,j*ppm,(data->height-2)*ppm/2-1, pen3);
            else
                addLine(j*ppm,-data->height*ppm/2,j*ppm,(data->height-2)*ppm/2);
        } else {
            if (j%2==0)
                addLine(j*ppm,-data->height*ppm/2+1,j*ppm,data->height*ppm/2-1, pen3);
            else
                addLine(j*ppm,-data->height*ppm/2,j*ppm,data->height*ppm/2);
        }
    }
    addLine(-data->width/2*ppm,-data->height/2*ppm,data->width/2*ppm,-data->height/2*ppm);
    if (roundedStage) {
        addLine(data->width/2*ppm,-data->height/2*ppm,data->width/2*ppm,data->height/2*ppm-ppm);
        addLine(-data->width/2*ppm,data->height/2*ppm-ppm,data->width/2*ppm,data->height/2*ppm-ppm);
        addLine(-data->width/2*ppm,-data->height/2*ppm,-data->width/2*ppm,data->height/2*ppm-ppm);
    } else {
        addLine(data->width/2*ppm,-data->height/2*ppm,data->width/2*ppm,data->height/2*ppm);
        addLine(-data->width/2*ppm,data->height/2*ppm,data->width/2*ppm,data->height/2*ppm);
        addLine(-data->width/2*ppm,-data->height/2*ppm,-data->width/2*ppm,data->height/2*ppm);
    }

    // Taikos
    foreach (QGraphicsTaikoItem* taiko, taikos) {
        if (taiko->data.emprestado) {
            QGraphicsColorizeEffect* borrowed = new QGraphicsColorizeEffect;
            borrowed->setColor(QColor(128,128,128));
            taiko->setGraphicsEffect(borrowed);
        } else {
            QGraphicsColorizeEffect* owned = new QGraphicsColorizeEffect;
            owned->setColor(QColor(0,0,0));
            taiko->setGraphicsEffect(owned);
        }
        addItem(taiko);
    }

    // Instrument List
    QMap<QString, int> instrument_hash;
    QMap<QString, int> borrowed_instrument_hash;
    addRect((data->width/2+1)*ppm, -data->height/2*ppm,200,data->height*ppm/2);
    QGraphicsTextItem *instrumentos_title = addText("Instrumentos");
    instrumentos_title->setPos((data->width/2+1)*ppm+100-instrumentos_title->boundingRect().width()/2, -data->height/2*ppm);
    addRect((data->width/2+1)*ppm,0,200,data->height*ppm/2);
    QGraphicsTextItem *borrowed_instrumentos_title = addText("Instrumentos emprestados");
    borrowed_instrumentos_title->setPos((data->width/2+1)*ppm+100-borrowed_instrumentos_title->boundingRect().width()/2, 0);
    foreach (QGraphicsTaikoItem* taiko, taikos) {
        if (taiko->data.emprestado) {
            borrowed_instrument_hash.insert(taiko->data.filename,borrowed_instrument_hash.value(taiko->data.filename)+1);
        } else {
            instrument_hash.insert(taiko->data.filename,instrument_hash.value(taiko->data.filename)+1);
        }
    }
    QMapIterator<QString, int> iterator(instrument_hash);
    for (i = 2; iterator.hasNext(); i++) {
        iterator.next();
        QGraphicsTextItem *instrument_name = addText(iterator.key());
        instrument_name->setPos((data->width/2+1)*ppm, -data->height/2*ppm+16*i);
        QGraphicsTextItem *instrument_count = addText(QString::number(iterator.value()));
        instrument_count->setPos((data->width/2+1)*ppm+184, -data->height/2*ppm+16*i);
    }
    QMapIterator<QString, int> borrowed_iterator(borrowed_instrument_hash);
    for (i = 2; borrowed_iterator.hasNext(); i++) {
        borrowed_iterator.next();
        QGraphicsTextItem *instrument_name = addText(borrowed_iterator.key());
        instrument_name->setPos((data->width/2+1)*ppm, 16*i);
        QGraphicsTextItem *instrument_count = addText(QString::number(borrowed_iterator.value()));
        instrument_count->setPos((data->width/2+1)*ppm+184, 16*i);
    }
}

void MapaScene::addInstrument(QString name, qreal x, qreal y, qreal angle, bool emprestado, bool newInstrument) {
    Instrumento taiko(x,y,angle,name, emprestado);
    QGraphicsTaikoItem *t = new QGraphicsTaikoItem(taiko);
    t->setOffset(-t->width/2,-t->height/2);
    t->setFlag(QGraphicsItem::ItemIsSelectable);
    t->setFlag(QGraphicsItem::ItemIsMovable);
    t->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    connect(t, &QGraphicsTaikoItem::moved, this, &MapaScene::onTaikoMoved);
    connect(t, &QGraphicsTaikoItem::rotated, this, &MapaScene::onTaikoRotated);
    if (newInstrument) {
        undo_stack->push(new CommandAdd(this, t));
    } else {
        addItem(t);
    }
}

QList<QGraphicsTaikoItem*> MapaScene::getTaikoItems() {
    QList<QGraphicsTaikoItem*> taikos;
    foreach (QGraphicsItem* taiko, items()){
        if (taiko->type() == QGraphicsTaikoItem::Type) {
            QGraphicsTaikoItem* taiko_item = dynamic_cast<QGraphicsTaikoItem*>(taiko);
            taikos.append(taiko_item);
        }
    }
    return taikos;
}

QList<QGraphicsTaikoItem*> MapaScene::clipboard;

void MapaScene::copy() {
    this->clipboard.clear();
    foreach (QGraphicsItem* taiko, selectedItems()){
        if (taiko->type() == QGraphicsTaikoItem::Type) {
            QGraphicsTaikoItem* taiko_item = new QGraphicsTaikoItem(dynamic_cast<QGraphicsTaikoItem*>(taiko));
            this->clipboard.append(taiko_item);
        }
    }
}


void MapaScene::paste() {
    foreach (QGraphicsTaikoItem* taiko, this->clipboard){
        addInstrument(taiko->data.filename, taiko->data.x, taiko->data.y, taiko->data.angle, taiko->data.emprestado);
    }
    updateScene(data, ppm);
}

void MapaScene::removeItemsAndClearBackground()
{
    foreach (QGraphicsItem* item, items()){
        if (item->type() == QGraphicsTaikoItem::Type) {
            removeItem(item);
        } else {
            delete(item);
        }
    }
}

void MapaScene::onTaikoMoved(QSet<QGraphicsItem *> items, qreal dx, qreal dy)
{
    CommandMove* command = new CommandMove(items, dx, dy);
    undo_stack->push(command);
}

void MapaScene::onTaikoRotated(QSet<QGraphicsItem *> items, qreal da)
{
    CommandRotate* command = new CommandRotate(items, da);
    undo_stack->push(command);
}
