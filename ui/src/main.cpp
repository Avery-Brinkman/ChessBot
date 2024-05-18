#include "BoardModel.h"

#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);
  QQuickView view;

  const int minWidth = 800;
  const int minHeight = 640;
  view.setMinimumSize(QSize(minWidth, minHeight));

  view.rootContext()->setContextProperties(
      {{"boardModel", QVariant::fromValue(new Chess_UI::BoardModel(&view))},
       {"minWidth", minWidth},
       {"minHeight", minHeight}});

  const QUrl url("qrc:/qml/main.qml");
  view.setSource(url);
  view.showMaximized();

  return QGuiApplication::exec();
}
