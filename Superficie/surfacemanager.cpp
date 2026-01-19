#include "surfacemanager.h"
#include <QUrl>
#include <QStatusBar>   // Resolve o erro do statusBar()
#include <QToolBar>     // Para a barra de ferramentas
#include <QAction>      // Para os botões de comando
#include <QDockWidget>  // Se for usar a barra lateral
#include <QStyle>
#include <QApplication>

TopoPoint parsePDWLine(const QString &line)
{
    TopoPoint p;
    // Ordem: ID - ATTR - NORTE - ESTE - COTA
    p.id   = line.mid(0, 16).trimmed();   // Coluna 0-10
    p.attr = line.mid(16, 16).trimmed();  // Coluna 10-25
    p.n    = line.mid(32, 12).toDouble() / 10000.0; // Coluna 25-40
    p.e    = line.mid(44, 12).toDouble() / 10000.0; // Coluna 40-55
    p.z    = line.mid(56, 12).toDouble() / 10000.0; // Coluna 55-68
    return p;
}

SurfaceManager::SurfaceManager(QWidget *parent): QMainWindow(parent)
{
    //qDebug() << "1. Iniciando Construtor...";
    QStyle *style = QApplication::style();
    QToolBar *sToolBar = addToolBar("Ferramentas"); // Aqui declaramos sToolBar
    QAction *actTIN = sToolBar->addAction("Gerar Malha");
    QDockWidget *dock = new QDockWidget("Propriedades", this);

    //qDebug() << "Linha 30";

    // Ícone de Lupa (StandardPixmap::SP_FileDialogContentsView ou SP_MessageBoxInformation)
    sActInspect = new QAction("Inspecionar", this);
    sActInspect->setIcon(style->standardIcon(QStyle::SP_FileDialogContentsView));
    QIcon iconInspect = style->standardIcon(QStyle::SP_FileDialogContentsView);
    sActInspect->setIcon(iconInspect);

    //qDebug() << "Linha 36";

    // Ícone de Caneta/Editar (StandardPixmap::SP_DialogReset ou SP_FileIcon)
    QIcon iconDraw = style->standardIcon(QStyle::SP_DialogResetButton);
    sActDraw = new QAction(iconDraw, "Desenhar", this);
    sActDraw->setCheckable(true);
    sActDraw->setIcon(iconDraw);

    // Ícone de Malha/Engrenagem (StandardPixmap::SP_ComputerIcon ou SP_BrowserReload)
    QIcon iconTIN = style->standardIcon(QStyle::SP_BrowserReload);

    qDebug() << "Linha 43";

    sScene = new QGraphicsScene(this);
    sView = new QGraphicsView(sScene, this);

    setCentralWidget(sView); // O gráfico é o rei da tela
    statusBar()->showMessage("Aguardando arquivo PDW...");
    statusBar()->setStyleSheet("background-color: #f0f0f0; border-top: 1px solid #ccc;");
    sToolBar->setMovable(false);
    actTIN->setIcon(iconTIN);
    connect(actTIN, &QAction::triggered, this, &SurfaceManager::generateTIN);

    qDebug() << "Linha 53";

    // Botão Inspeção (Ver dados do ponto)
    sActInspect = sToolBar->addAction("Inspecionar");
    sActInspect->setCheckable(true);
    connect(sActInspect, &QAction::triggered, this, &SurfaceManager::setModeInspect);// Botão Breakline (Ligar pontos)
    sActDraw = sToolBar->addAction("Desenhar Breakline");
    sActDraw->setCheckable(true);
    connect(sActDraw, &QAction::triggered, this, &SurfaceManager::setModeDraw);

    sToolBar->addSeparator();


    // 3. ÁREA DE FERRAMENTAS (TOOLBAR)
    // Cria uma barra no topo ou lateral para os botões (Delaunay, Salvar, etc)
    sToolBar->setMovable(false);
    dock->setAllowedAreas(Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    setAcceptDrops(true);
    setWindowTitle("Superficie - Analisador Topográfico Siderúrgico");
    resize(1280, 720); // Um tamanho de trabalho confortável


    sView->setDragMode(QGraphicsView::ScrollHandDrag);
    sView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    sView->viewport()->installEventFilter(this);
    setCentralWidget(sView); // Faz a área gráfica ocupar a janela toda
    sView->setBackgroundBrush(Qt::darkGray); // Se a tela ficar cinza, o View está lá

    setAcceptDrops(true); // Habilita o arrastar e soltar
    sView->setRenderHint(QPainter::Antialiasing);
    //loadPdwFile("C:/2026/Soft/Instâncias/desenhar.pdw");
    setAcceptDrops(true);
    sView->setAcceptDrops(false); // IMPORTANTE: O View ignora para o sinal subir para a MainWindow
    sView->viewport()->setAcceptDrops(false);
}

void SurfaceManager::loadPdwFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    sPoints.clear(); // Limpa levantamentos anteriores
    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();

        // Validação básica: ignorar linhas vazias ou menores que o padrão
        if (line.length() < 68) continue;
        sPoints.append(parsePDWLine(line));
    }
    file.close();

    // Após carregar, gatilho para desenhar na cena
    // updateGraphics();
}

// Aceita o arquivo quando ele entra na área da janela
void SurfaceManager::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "Tentativa de Drag detectada...";
    if (event->mimeData()->hasFormat("text/uri-list") || event->mimeData()->hasUrls())
    {
        qDebug() << "Formato aceito! Mudando ícone do mouse...";
        event->acceptProposedAction();
    } else {
        qDebug() << "Formato RECUSADO. Formatos disponíveis:" << event->mimeData()->formats();
    }
}

// Processa o arquivo quando ele é solto
void SurfaceManager::dropEvent(QDropEvent *event)
{
    qDebug() << "Evento DROP disparado!";

    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        // Pega o caminho do primeiro arquivo solto
        QString filePath = mimeData->urls().at(0).toLocalFile();
        qDebug() << "Caminho capturado:" << filePath;

        // Finaliza a animação de drop no Windows
        event->acceptProposedAction();

        // Dispara o processamento
        loadPdwFile(filePath);
        updateGraphics();
    } else {
        qDebug() << "Drop recusado: Sem URLs válidas.";
    }
}

bool SurfaceManager::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == sView->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);

        // Define o ponto do mouse como âncora para o zoom
        sView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

        double scaleFactor = 1.15; // Velocidade do zoom
        if (wheelEvent->angleDelta().y() > 0)
        {
            sView->scale(scaleFactor, scaleFactor);
        } else {
            sView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }
        return true; // Bloqueia o evento original para não rolar a barra lateral
    }
    return QMainWindow::eventFilter(obj, event);
}

void SurfaceManager::updateGraphics() {
    if (sPoints.isEmpty()) return;

    sScene->clear(); // Remove tudo que existia antes

    // 1. Encontrar limites para o offset
    double minE = sPoints[0].e;
    double maxN = sPoints[0].n;
    for (const auto& p : sPoints)
    {
        if (p.e < minE) minE = p.e;
        if (p.n > maxN) maxN = p.n;
    }

    // 2. Adicionar itens à cena
    for (const auto& p : sPoints)
    {
        double x = p.e - minE;
        double y = -(p.n - maxN);

        // Vamos desenhar um círculo maior (raio 10.0) para garantir que seja visível
        auto *item = sScene->addEllipse(x-3.0, y-3.0, 6.0, 6.0, QPen(Qt::red), QBrush(Qt::yellow));
        item->setZValue(1); // Garante que fique acima de qualquer fundo}
        item->setPos(x, y);
        item->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        //qDebug() << p.id << '\t' << p.attr << '\t' << p.e << '\t' << p.n << '\t' << p.z;
        item->setToolTip(QString("Attr: %1\nZ: %2").arg(p.attr).arg(QString::number(p.z, 'f', 4))); // 4 casas decimais

    // 3. Atualizar a visualização
        sScene->setSceneRect(sScene->itemsBoundingRect());
        sView->setSceneRect(sScene->sceneRect()); // Sincroniza view e scene
        sView->fitInView(sScene->sceneRect(), Qt::KeepAspectRatio);

        sView->viewport()->update();
        sView->show();
    }
}

void SurfaceManager::generateTIN()
{
    statusBar()->showMessage("Processando Triangulação de Delaunay...");
    // Aqui chamaremos seu motor numérico depois
}

void SurfaceManager::setModeDraw()
{
    sCurrentMode = InteractionMode::DrawBreakline;
    sActInspect->setChecked(false);
    statusBar()->showMessage("Modo: Desenho de Breaklines (Clique em dois pontos)");
}

void SurfaceManager::setModeInspect()
{
    sCurrentMode = InteractionMode::Inspect;
    sActDraw->setChecked(false);
    statusBar()->showMessage("Modo: Inspeção de Pontos");
}

SurfaceManager::~SurfaceManager() {}
