#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

#include <QtGui>
#include <QStringList>
#include <QTextEdit>
#include "ElaMenu.h"

class NoteWidget : public QTextEdit
{
    Q_OBJECT
public:
    explicit NoteWidget(QWidget *parent = nullptr);
    void setDocumentPadding(int left, int top, int right, int bottom);
    bool eventFilter(QObject *obj, QEvent *event);
    bool openLinkAtCursorPosition();
    QString getMarkdownUrlAtPosition(const QString &text, int position);
    bool isValidUrl(const QString &urlString);
    void openUrl(const QString &urlString);
    QMap<QString, QString> parseMarkdownUrlsFromText(const QString &text);
    QUrl getUrlUnderMouse();
    void moveBlockUp();
    void moveBlockDown();
    void fontBlod();
    void fontcancelBlod();
    void fontTilt();
    void fontcancelTilt();
    void fontStrikethrough();
    void fontcancelStrikethrough();
    void fontHighlighter();
    void fontcancelHighlighter();
signals:
    void resized();
    void mouseMoved();

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event) override;

    QStringList _ignoredClickUrlSchemata;

private:
    bool isbold{false};
    bool istilt{false};
    bool isstrikethrough{false};
    bool ishighlighter{false};

    ElaMenu* _noteMenu{nullptr};
};

#endif // NOTEWIDGET_H
