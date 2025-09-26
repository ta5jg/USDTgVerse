//
//  CodeEditor.h
//  USDTgVerse IDE - Professional Code Editor
//
//  Created by USDTgVerse Development Team
//  Copyright © 2024 USDTgVerse. All rights reserved.
//

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QCompleter>
#include <QStringListModel>

class USDTgScriptHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    USDTgScriptHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    
    QVector<HighlightingRule> highlightingRules;
    
    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat operatorFormat;
};

class CodeEditor : public QTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor();

    // File operations
    void openFile(const QString &fileName);
    void saveFile(const QString &fileName);
    bool isModified() const;
    void setModified(bool modified);
    
    // Language support
    void setLanguage(const QString &language);
    QString getLanguage() const;
    
    // Syntax highlighting
    void enableSyntaxHighlighting(bool enable);
    bool isSyntaxHighlightingEnabled() const;
    
    // Auto-completion
    void enableAutoCompletion(bool enable);
    bool isAutoCompletionEnabled() const;
    
    // Line numbers
    void showLineNumbers(bool show);
    bool areLineNumbersVisible() const;
    
    // Code formatting
    void formatCode();
    void indentCode();
    void unindentCode();
    
    // Search and replace
    void findText(const QString &text);
    void replaceText(const QString &text, const QString &replacement);
    
    // Code analysis
    void analyzeCode();
    QStringList getErrors() const;
    QStringList getWarnings() const;
    
    // USDTgScript specific
    void compileUSDTgScript();
    void deployContract();
    void runContract();
    void debugContract();

signals:
    void fileOpened(const QString &fileName);
    void fileSaved(const QString &fileName);
    void fileModified();
    void compilationRequested();
    void deploymentRequested();
    void errorOccurred(const QString &error);

private slots:
    void onTextChanged();
    void onCursorPositionChanged();

private:
    void setupHighlighter();
    void setupCompleter();
    void setupUSDTgScriptKeywords();
    void updateLineNumbers();
    void highlightCurrentLine();
    
    // USDTgScript specific methods
    void validateUSDTgScript();
    void optimizeGasUsage();
    void checkQuantumSafety();

private:
    QString currentLanguage;
    bool syntaxHighlightingEnabled;
    bool autoCompletionEnabled;
    bool lineNumbersVisible;
    
    USDTgScriptHighlighter *highlighter;
    QCompleter *completer;
    QStringListModel *completionModel;
    
    QStringList usdtgKeywords;
    QStringList usdtgFunctions;
    QStringList usdtgTypes;
    
    QStringList errors;
    QStringList warnings;
    
    QTextDocument *document;
};

#endif // CODEEDITOR_H
