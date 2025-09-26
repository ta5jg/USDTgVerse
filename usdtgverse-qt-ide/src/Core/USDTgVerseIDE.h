//
//  USDTgVerseIDE.h
//  USDTgVerse IDE - Professional Qt Implementation
//
//  Created by USDTgVerse Development Team
//  Copyright © 2024 USDTgVerse. All rights reserved.
//

#ifndef USDTGVERSEIDE_H
#define USDTGVERSEIDE_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTreeView>
#include <QSplitter>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QProgressBar>
#include <QTimer>
#include <QFileSystemModel>
#include <QTextDocument>
#include <QSyntaxHighlighter>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE

// Forward declarations
class CodeEditor;
class Terminal;
class AIAssistant;
class FileExplorer;

class USDTgVerseIDE : public QMainWindow
{
    Q_OBJECT

public:
    USDTgVerseIDE(QWidget *parent = nullptr);
    ~USDTgVerseIDE();

private slots:
    // File operations
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    void closeFile();
    
    // Project operations
    void newProject();
    void openProject();
    void saveProject();
    void closeProject();
    
    // Build operations
    void compileProject();
    void deployContract();
    void runProject();
    void debugProject();
    
    // AI operations
    void openAIAssistant();
    void sendAIMessage();
    
    // SSH operations
    void openSSHConnection();
    void connectSSH();
    
    // View operations
    void toggleFileExplorer();
    void toggleTerminal();
    void toggleAIAssistant();
    void toggleDebugPanel();
    
    // Settings
    void showPreferences();
    void showAbout();
    
    // Status updates
    void updateStatus(const QString &message);
    void updateProgress(int value);

private:
    // UI Setup
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupCentralWidget();
    void setupPanels();
    void setupConnections();
    
    // Panel management
    void createLeftPanel();
    void createCenterPanel();
    void createRightPanel();
    void createBottomPanel();
    
    // Theme management
    void applyTheme(const QString &themeName);
    void loadTheme();
    
    // File management
    void loadFile(const QString &filePath);
    void saveCurrentFile();
    bool isFileModified();
    
    // Project management
    void loadProject(const QString &projectPath);
    void saveCurrentProject();
    
    // Compilation
    void startCompilation();
    void handleCompilationResult();
    
    // Deployment
    void startDeployment();
    void handleDeploymentResult();

private:
    // Main UI components
    QWidget *centralWidget;
    QSplitter *mainSplitter;
    QSplitter *leftSplitter;
    QSplitter *rightSplitter;
    QSplitter *bottomSplitter;
    
    // Panels
    CodeEditor *codeEditor;
    Terminal *terminal;
    AIAssistant *aiAssistant;
    FileExplorer *fileExplorer;
    QTextEdit *debugPanel;
    
    // Tab widgets
    QTabWidget *editorTabs;
    QTabWidget *rightTabs;
    QTabWidget *bottomTabs;
    
    // Menu and toolbar
    QMenuBar *menuBar;
    QToolBar *toolBar;
    QStatusBar *statusBar;
    
    // Status elements
    QLabel *statusLabel;
    QProgressBar *progressBar;
    QLabel *lineLabel;
    QLabel *columnLabel;
    
    // Actions
    QAction *newFileAction;
    QAction *openFileAction;
    QAction *saveFileAction;
    QAction *saveAsFileAction;
    QAction *closeFileAction;
    QAction *newProjectAction;
    QAction *openProjectAction;
    QAction *saveProjectAction;
    QAction *closeProjectAction;
    QAction *compileAction;
    QAction *deployAction;
    QAction *runAction;
    QAction *debugAction;
    QAction *aiAssistantAction;
    QAction *sshAction;
    QAction *preferencesAction;
    QAction *aboutAction;
    
    // File management
    QString currentFilePath;
    QString currentProjectPath;
    bool fileModified;
    
    // Theme
    QString currentTheme;
    
    // Compilation
    QTimer *compilationTimer;
    QProcess *compilationProcess;
    
    // Deployment
    QTimer *deploymentTimer;
    QProcess *deploymentProcess;
};

#endif // USDTGVERSEIDE_H
