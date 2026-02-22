#ifndef T_NOTE_H
#define T_NOTE_H

#include <QApplication>
#include <QTextEdit>
#include <QFile>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QTextBrowser>
//#include <QWebEngineView>
#include "Base/T_BasePage.h"
#include "notewidget.h"

class T_Note : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Note(QWidget* parent = nullptr);
    ~T_Note();
    QString markdownToHtml(const QString &markdown);
    void renderMarkdown();
    void loadFile(const QString &filename);
    void onCursorPositionChanged();  // 光标移动处理
    void toggleRenderMode();  // 切换渲染/编辑模式
    void highlightFormulaArea(int pos);

//  void saveFile() {
//      if (currentFile.isEmpty()) {
//          currentFile = QFileDialog::getSaveFileName(this, "保存文件", "", "Markdown文件 (*.md *.markdown)");
//          if (currentFile.isEmpty()) return;
//      }

//      QFile file(currentFile);
//      if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//          file.write(editor->toMarkdown().toUtf8()); // 关键方法：获取Markdown内容
//          file.close();
//      }
//  }

private:
//  void onLoadClicked() {
//      QString filename = QFileDialog::getOpenFileName(this, "打开Markdown文件", "", "Markdown文件 (*.md *.markdown);;所有文件 (*)");
//      if (!filename.isEmpty()) {
//          loadFile(filename);
//      }
//  }

//  void preview() {
//      // 创建预览窗口
//      QDialog previewDialog(this);
//      previewDialog.setWindowTitle("预览");
//      previewDialog.resize(600, 400);

//      QTextEdit *previewEditor = new QTextEdit(&previewDialog);
//      previewEditor->setReadOnly(true);
//      previewEditor->setMarkdown(editor->toMarkdown()); // 将Markdown渲染为HTML显示

//      QVBoxLayout *layout = new QVBoxLayout(&previewDialog);
//      layout->addWidget(previewEditor);

//      previewDialog.exec();
//  }

private:
    NoteWidget *editor{nullptr};
    QString currentFile;
//    QTextBrowser *preview{nullptr};
    bool isRendered{false};
    bool isNearFormula{false};
    QString originalMarkdown;  // 保存原始文本
};

#endif // T_NOTE_H
