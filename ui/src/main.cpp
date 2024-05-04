#include "BoardModel.h"

#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);
  QQuickView view;

  view.rootContext()->setContextProperty("boardModel", new Chess_UI::BoardModel(&view));

  const QUrl url("qrc:/qml/main.qml");
  view.setSource(url);
  view.show();

  return QGuiApplication::exec();
}
