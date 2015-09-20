/*
* Copyright (c) 2012-2015 Christian Surlykke, Petr Vanek
*
* This file is part of qt-lightdm-greeter 
* It is distributed under the LGPL 2.1 or later license.
* Please refer to the LICENSE file for a copy of the license.
*/
#include <QtWidgets/QApplication>
#include <QDesktopWidget>
#include <QMap>
#include <QtGlobal>
#include <QtDebug>
#include <QFile>
#include <QSettings>

#include <iostream>

#include "constants.h"
#include "settings.h"
#include "mainwindow.h"

QFile logfile;
QTextStream ts;

void messageHandler(QtMsgType type, const QMessageLogContext&, const QString& msg)
{
    std::cerr << type << ": " << msg.toLatin1().data() << "\n";
}

int main(int argc, char *argv[])
{
    // I have no idea why, but Qt's stock qDebug() output never makes it
    // to /var/log/lightdm/x-0-greeter.log, so we use std::cerr instead..
    qInstallMessageHandler(messageHandler);
    qt_lightdm_greeter_prepare();

    QApplication a(argc, argv);

    QFile styleFile(":/resources/qt-lightdm-greeter.qss");
    styleFile.open(QFile::ReadOnly);
    QString styleSheet = styleFile.readAll();
    a.setStyleSheet(styleSheet);

    MainWindow *focusWindow = 0;
    for (int i = 0; i < QApplication::desktop()->screenCount(); ++i)
    {
        MainWindow *w = new MainWindow(i);
        w->show();
        if (w->showLoginForm())
            focusWindow = w;
    }

    // Ensure we set the primary screen's widget as active when there
    // are more screens
    if (focusWindow)
    {
        focusWindow->setFocus(Qt::OtherFocusReason);
        focusWindow->activateWindow();
    }

    return a.exec();
}
