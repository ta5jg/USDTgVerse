//
//  main.cpp
//  USDTgVerse IDE - Professional Qt Implementation
//
//  Created by USDTgVerse Development Team
//  Copyright © 2024 USDTgVerse. All rights reserved.
//

#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QDir>
#include <QStandardPaths>
#include "Core/USDTgVerseIDE.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("USDTgVerse IDE");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("USDTgVerse");
    app.setOrganizationDomain("usdtgverse.com");
    
    // Set application style
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Set dark theme
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
    app.setPalette(darkPalette);
    
    // Create main window
    USDTgVerseIDE mainWindow;
    mainWindow.show();
    
    return app.exec();
}
