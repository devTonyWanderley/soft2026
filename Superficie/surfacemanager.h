#ifndef SURFACEMANAGER_H
#define SURFACEMANAGER_H

#include <QMainWindow>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QGraphicsEllipseItem>
#include <QFile>
#include <QTextStream>


struct TopoPoint
{
    QString id = "";
    QString attr = "";
    double n = 0.0;
    double e = 0.0;
    double z = 0.0;
};

struct Triangle
{
    int p1, p2, p3;
};

class PointItem : public QGraphicsEllipseItem
{
public:
    int pointIndex; // Índice correspondente no QList<TopoPoint> sPoints
    // Ao clicar aqui, o SurfaceManager saberá exatamente qual ponto foi selecionado.
};

class SurfaceManager : public QMainWindow
{
    Q_OBJECT
public:
    SurfaceManager(QWidget *parent = nullptr);
    ~SurfaceManager();
protected:
    // Eventos para o "Plus" de arrastar arquivos
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
private:
    QList<TopoPoint> sPoints;
    QList<Triangle> sTriangles;
    QList<QLine> sActiveBreaklines;
    // Motor Gráfico (Qt)
    QGraphicsScene *sScene;
    QGraphicsView  *sView;

    // Funções de Processamento
    void loadPdwFile(const QString &fileName);
    void generateTIN(); // Onde a mágica da triangulação acontece
    void exportSurface(const QString &fileName);
    void updateGraphics();
};

TopoPoint parsePDWLine(const QString &line);

#endif // SURFACEMANAGER_H
