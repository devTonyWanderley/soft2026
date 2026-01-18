#include "mainwindow.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QColor>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 1. A Cena (O Grafo de Objetos)
    QGraphicsScene *scene = new QGraphicsScene();

    // 2. Gerar uma malha de teste (ex: 100x100 quadrados divididos em 2 triângulos)
    int size = 100;
    int step = 20; // tamanho do lado em pixels
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            // Geometria básica de um triângulo
            QPolygonF poly;
            poly << QPointF(x*step, y*step)
                 << QPointF((x+1)*step, y*step)
                 << QPointF(x*step, (y+1)*step);

            // Criar o item e adicionar à cena
            QGraphicsPolygonItem *tri = new QGraphicsPolygonItem(poly);

            // Estética (Gradiente aleatório para testar cores)
            tri->setBrush(QColor(rand()%255, rand()%255, 255));
            tri->setPen(QPen(Qt::black, 0.5)); // Linha fina

            scene->addItem(tri);
        }
    }

    // 3. A View (A Janela que olha para a cena)
    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing); // Suaviza as linhas
    view->setDragMode(QGraphicsView::ScrollHandDrag); // Permite arrastar com o mouse
    view->setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing);
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    view->setWindowTitle("Hello Malha Qt");
    view->resize(800, 600);
    view->show();

    return a.exec();
}

/*
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

*/
