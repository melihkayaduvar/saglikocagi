#include <QApplication>
#include "Veri/veritabani.h"
#include "ui/login.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    QColor disabledColor(127, 127, 127);
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, disabledColor);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
    darkPalette.setColor(QPalette::Disabled, QPalette::Base, QColor(35, 35, 35));
    darkPalette.setColor(QPalette::Disabled, QPalette::Button, QColor(45, 45, 45));

    app.setPalette(darkPalette);

    VERITABANI::vt();

    login frm;
    frm.show();

    auto sonuc = app.exec();

    return sonuc;
}
