/*
==============================================
 File:        mainwindow.cpp
 Author:      Irfan Gedik
 Created:     25.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgVerse IDE Main Window
   
   Qt IDE main window implementation featuring:
   - Main window setup and initialization
   - UI component management
   - Window event handling
   - IDE interface management
   - User interface controls

 License:
   MIT License
==============================================
*/

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

