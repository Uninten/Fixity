#include "notewidget.h"
#include <QDebug>
#include <QGuiApplication>
#include <QTextCursor>
#include <QMessageBox>

NoteWidget::NoteWidget(QWidget *parent) : QTextEdit(parent)
{
    installEventFilter(this);
    viewport()->installEventFilter(this);
    setMouseTracking(true);
    setAttribute(Qt::WidgetAttribute::WA_Hover, true);
}

/*!
 * \brief CustomDocument::setDocumentPadding
 * We use a custom document for MainWindow::m_textEdit
 * so we can set the document padding without the (upstream Qt) issue
 * where the vertical scrollbar gets padded with the text as well.
 * This way, only the text gets padded, and the vertical scroll bar stays where it is.
 * \param left
 * \param top
 * \param right
 * \param bottom
 */
void NoteWidget::setDocumentPadding(int left, int top, int right, int bottom)
{
    setViewportMargins(left, top, right, bottom);
}

void NoteWidget::resizeEvent(QResizeEvent *event)
{
    QTextEdit::resizeEvent(event);
    emit resized();
}

void NoteWidget::mouseMoveEvent(QMouseEvent *event)
{
    QTextEdit::mouseMoveEvent(event);
    emit mouseMoved();
}

void NoteWidget::contextMenuEvent(QContextMenuEvent *event)
{
    _noteMenu = new ElaMenu(this);
    ElaMenu* _TextTypeMenu = _noteMenu->addMenu(ElaIconType::Brush, "文本格式");
    QAction* boldAction = _TextTypeMenu->addElaIconAction(ElaIconType::Bold, "加粗");
    QAction* tiltAction = _TextTypeMenu->addElaIconAction(ElaIconType::FieldHockeyStickBall, "倾斜");
    QAction* strikethroughAction =_TextTypeMenu->addElaIconAction(ElaIconType::Strikethrough, "删除线");
    QAction* highlighterAction =_TextTypeMenu->addElaIconAction(ElaIconType::Highlighter, "高亮");
    _TextTypeMenu->addSeparator();
    _TextTypeMenu->addElaIconAction(ElaIconType::Code, "代码");
    _TextTypeMenu->addElaIconAction(ElaIconType::Number00, "数学");
    _TextTypeMenu->addElaIconAction(ElaIconType::Percent, "注释");
    _TextTypeMenu->addSeparator();
    _TextTypeMenu->addElaIconAction(ElaIconType::Eraser, "清除格式");

    ElaMenu* _ParagraphSettingMenu = _noteMenu->addMenu(ElaIconType::Paragraph, "段落设置");
    _ParagraphSettingMenu->addElaIconAction(ElaIconType::ListUl, "无序列表");
    _ParagraphSettingMenu->addElaIconAction(ElaIconType::ListOl, "有序列表");
    _ParagraphSettingMenu->addElaIconAction(ElaIconType::ListCheck, "任务列表");
    _ParagraphSettingMenu->addSeparator();
    _ParagraphSettingMenu->addElaIconAction(ElaIconType::H1, "一级标题");
    _ParagraphSettingMenu->addElaIconAction(ElaIconType::H2, "二级标题");
    _ParagraphSettingMenu->addElaIconAction(ElaIconType::H3, "三级标题");
    _ParagraphSettingMenu->addElaIconAction(ElaIconType::H4, "四级标题");
    _ParagraphSettingMenu->addElaIconAction(ElaIconType::H5, "五级标题");
    _ParagraphSettingMenu->addElaIconAction(ElaIconType::H6, "六级标题");
    _ParagraphSettingMenu->addElaIconAction(ElaIconType::BarsStaggered, "正文");
    _ParagraphSettingMenu->addSeparator();
    _ParagraphSettingMenu->addElaIconAction(ElaIconType::QuoteRight, "引用");

    ElaMenu* _InsertMenu = _noteMenu->addMenu(ElaIconType::BlockQuote, "插入");
    _InsertMenu->addElaIconAction(ElaIconType::NoteSticky, "脚注");
    _InsertMenu->addElaIconAction(ElaIconType::Table, "表格");
    _InsertMenu->addElaIconAction(ElaIconType::QuoteRight, "标注");
    _InsertMenu->addElaIconAction(ElaIconType::Dash, "分隔线");
    _InsertMenu->addSeparator();
    _InsertMenu->addElaIconAction(ElaIconType::CodeSimple, "代码块");
    _InsertMenu->addElaIconAction(ElaIconType::Numbe0, "数学块");

    QAction* cutAction = _noteMenu->addElaIconAction(ElaIconType::Scissors, "剪切", QKeySequence::Cut);
    QAction* copyAction = _noteMenu->addElaIconAction(ElaIconType::Copy, "复制", QKeySequence::Copy);
    QAction* pasteAction = _noteMenu->addElaIconAction(ElaIconType::Paste, "粘贴", QKeySequence::Paste);
    QAction* selectallAction = _noteMenu->addElaIconAction(ElaIconType::SquareDashed, "全选", QKeySequence::SelectAll);
    _noteMenu->addSeparator();

    // 连接信号槽（直接调用父类的默认功能）
    connect(boldAction, &QAction::triggered, this, [=]() {
        isbold = !isbold; // 切换状态
        isbold ? fontBlod() : fontcancelBlod();
    });
    connect(tiltAction, &QAction::triggered, this, [=]() {
        istilt = !istilt; // 切换状态
        istilt ? fontTilt() : fontcancelTilt();
    });
    connect(strikethroughAction, &QAction::triggered, this, [=]() {
        isstrikethrough = !isstrikethrough; // 切换状态
        isstrikethrough ? fontStrikethrough() : fontcancelStrikethrough();
    });
    connect(highlighterAction, &QAction::triggered, this, [=]() {
        ishighlighter = !ishighlighter; // 切换状态
        ishighlighter ? fontHighlighter() : fontcancelHighlighter();
    });
    connect(cutAction, &QAction::triggered, this, &QTextEdit::cut);
    connect(copyAction, &QAction::triggered, this, &QTextEdit::copy);
    connect(pasteAction, &QAction::triggered, this, &QTextEdit::paste);
    connect(selectallAction, &QAction::triggered, this, &QTextEdit::selectAll);
    _noteMenu->exec(event->globalPos());
    delete _noteMenu; // 防止内存泄漏
}

bool NoteWidget::eventFilter(QObject *obj, QEvent *event)
{
    // qDebug() << event->type();

    if (event->type() == QEvent::HoverMove) {
        //当用户将鼠标悬停在文本区域并移动鼠标时，会检查鼠标是否悬停在一个有效的链接上。如果是，并且按下了控制键 (Ctrl)，则将鼠标光标设置为指针手型（表示可以点击）；否则，设置为文本光标。
        if (QGuiApplication::keyboardModifiers() == Qt::ExtraButton24
            && getUrlUnderMouse().isValid()) {
            viewport()->setCursor(Qt::PointingHandCursor);
        } else {
            viewport()->setCursor(Qt::IBeamCursor);
        }
    } else if (event->type() == QEvent::KeyPress) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_Control) {
            // check if mouse is over a link
            auto url = getUrlUnderMouse();
            viewport()->setCursor(url.isValid() ? Qt::PointingHandCursor : Qt::IBeamCursor);
        } else if (keyEvent->modifiers().testFlag(Qt::AltModifier)) {
            // alt + arrow up/down
            if (keyEvent->key() == Qt::Key_Up) {
                moveBlockUp();
                return true;
            } else if (keyEvent->key() == Qt::Key_Down) {
                moveBlockDown();
                return true;
            }
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {

        auto *mouseEvent = static_cast<QMouseEvent *>(event);

        // track `Ctrl + Click` in the text edit
        if ((obj == viewport()) && (mouseEvent->button() == Qt::LeftButton)
            && (QGuiApplication::keyboardModifiers() == Qt::ExtraButton24)) {
            // open the link (if any) at the current position
            // in the noteTextEdit

            viewport()->setCursor(Qt::IBeamCursor);

            openLinkAtCursorPosition();

            return true;
        }
    } else if (event->type() == QEvent::KeyRelease) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);

        // reset cursor if control key was released
        if (keyEvent->key() == Qt::Key_Control) {
            viewport()->setCursor(Qt::IBeamCursor);
        }
    }

    return QTextEdit::eventFilter(obj, event);
}

/**
 * @brief Returns the markdown url at position
 * @param text
 * @param position
 * @return url string
 */
QString NoteWidget::getMarkdownUrlAtPosition(const QString &text, int position)
{
    QString url;

    // get a map of parsed markdown urls with their link texts as key
    const QMap<QString, QString> urlMap = parseMarkdownUrlsFromText(text);
    QMap<QString, QString>::const_iterator i = urlMap.constBegin();
    for (; i != urlMap.constEnd(); ++i) {
        const QString &linkText = i.key();
        const QString &urlString = i.value();

        const int foundPositionStart = text.indexOf(linkText);

        if (foundPositionStart >= 0) {
            // calculate end position of found linkText
            const int foundPositionEnd = foundPositionStart + linkText.size();

            // check if position is in found string range
            if ((position >= foundPositionStart) && (position < foundPositionEnd)) {
                url = urlString;
                break;
            }
        }
    }

    return url;
}

/**
 * @brief Returns the URL under the current mouse cursor
 *
 * @return QUrl
 */
QUrl NoteWidget::getUrlUnderMouse()
{
    // place a temp cursor at the mouse position
    auto pos = viewport()->mapFromGlobal(QCursor::pos());
    QTextCursor cursor = cursorForPosition(pos);
    const int cursorPosition = cursor.position();

    // select the text of the current block
    cursor.movePosition(QTextCursor::StartOfBlock);
    const int indexInBlock = cursorPosition - cursor.position();
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

    // get the correct link from the selected text, or an empty URL if none found
    return QUrl(getMarkdownUrlAtPosition(cursor.selectedText(), indexInBlock));
}

/**
 * @brief Opens the link (if any) at the current cursor position
 */
bool NoteWidget::openLinkAtCursorPosition()
{
    // find out which url in the selected text was clicked
    QUrl const url = getUrlUnderMouse();
    QString const urlString = url.toString();

    const bool isFileUrl = urlString.startsWith(QLatin1String("file://"));

    const bool isLegacyAttachmentUrl = urlString.startsWith(QLatin1String("file://attachments"));
    const bool isLocalFilePath = urlString.startsWith(QLatin1String("/"));

    const bool convertLocalFilepathsToURLs = true;

    if ((url.isValid() && isValidUrl(urlString)) || isFileUrl || isLocalFilePath
        || isLegacyAttachmentUrl) {

        if (_ignoredClickUrlSchemata.contains(url.scheme())) {
            qDebug() << __func__ << "ignored URL scheme:" << urlString;
            return false;
        }

        // ignore non-existent files
        if (isFileUrl) {
            QString trimmed = urlString.mid(7);
            if (!QFile::exists(trimmed)) {
                qDebug() << __func__ << ": File does not exist:" << urlString;
                // show a message box
                QMessageBox::warning(
                        nullptr, tr("File not found"),
                        tr("The file <strong>%1</strong> does not exist.").arg(trimmed));
                return false;
            }
        }

        if (isLocalFilePath && !QFile::exists(urlString)) {
            qDebug() << __func__ << ": File does not exist:" << urlString;
            // show a message box
            QMessageBox::warning(nullptr, tr("File not found"),
                                 tr("The file <strong>%1</strong> does not exist.").arg(urlString));
            return false;
        }

        if (isLocalFilePath && convertLocalFilepathsToURLs) {
            openUrl(QString("file://") + urlString);
        } else {
            openUrl(urlString);
        }

        return true;
    }

    return false;
}

/**
 * Checks if urlString is a valid url
 *
 * @param urlString
 * @return
 */
bool NoteWidget::isValidUrl(const QString &urlString)
{
    const QRegularExpressionMatch match = QRegularExpression(R"(^\w+:\/\/.+)").match(urlString);
    return match.hasMatch();
}

/**
 * Handles clicked urls
 *
 * examples:
 * - <https://www.qownnotes.org> opens the webpage
 * - <file:///path/to/my/file/QOwnNotes.pdf> opens the file
 *   "/path/to/my/file/QOwnNotes.pdf" if the operating system supports that
 *  handler
 */
void NoteWidget::openUrl(const QString &urlString)
{
    qDebug() << "CustomDocument " << __func__ << " - 'urlString': " << urlString;

    QDesktopServices::openUrl(QUrl(urlString));
}

/**
 * @brief Returns a map of parsed markdown urls with their link texts as key
 *
 * @param text
 * @return parsed urls
 */
QMap<QString, QString> NoteWidget::parseMarkdownUrlsFromText(const QString &text)
{
    QMap<QString, QString> urlMap;
    QRegularExpression regex;
    QRegularExpressionMatchIterator iterator;

    // match urls like this: <http://mylink>
    //    re = QRegularExpression("(<(.+?:\\/\\/.+?)>)");
    regex = QRegularExpression(QStringLiteral("(<(.+?)>)"));
    iterator = regex.globalMatch(text);
    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        QString linkText = match.captured(1);
        QString url = match.captured(2);
        urlMap[linkText] = url;
    }

    regex = QRegularExpression(R"((\[.*?\]\((.+?)\)))");
    iterator = regex.globalMatch(text);
    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        QString linkText = match.captured(1);
        QString url = match.captured(2);
        urlMap[linkText] = url;
    }

    // match urls like this: http://mylink
    regex = QRegularExpression(R"(\b\w+?:\/\/[^\s]+[^\s>\)])");
    iterator = regex.globalMatch(text);
    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        QString url = match.captured(0);
        urlMap[url] = url;
    }

    // match urls like this: www.github.com
    regex = QRegularExpression(R"(\bwww\.[^\s]+\.[^\s]+\b)");
    iterator = regex.globalMatch(text);
    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        QString url = match.captured(0);
        urlMap[url] = QStringLiteral("http://") + url;
    }

    // match reference urls like this: [this url][1] with this later:
    // [1]: http://domain
    regex = QRegularExpression(R"((\[.*?\]\[(.+?)\]))");
    iterator = regex.globalMatch(text);
    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        QString linkText = match.captured(1);
        QString referenceId = match.captured(2);

        QRegularExpression refRegExp(QStringLiteral("\\[") + QRegularExpression::escape(referenceId)
                                     + QStringLiteral("\\]: (.+)"));
        QRegularExpressionMatch urlMatch = refRegExp.match(toPlainText());

        if (urlMatch.hasMatch()) {
            QString url = urlMatch.captured(1);
            urlMap[linkText] = url;
        }
    }

    return urlMap;
}

void NoteWidget::moveBlockUp()
{
    QTextCursor cursor = textCursor();

    if (cursor.blockNumber() > 0) {
        QString currentBlock = cursor.block().text();
        const int currentHorizontalPosition = cursor.positionInBlock();

        cursor.movePosition(QTextCursor::StartOfBlock);
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        // also remove empty line
        cursor.deletePreviousChar();

        // cursor is now at end of prev line, move to start
        if (!cursor.movePosition(QTextCursor::StartOfBlock)) {
            // block above is empty, this is fine
        }
        // insert the removed block again
        cursor.insertText(currentBlock);
        cursor.insertBlock();

        // move cursor to previous block
        if (!cursor.movePosition(QTextCursor::PreviousBlock)) {
            qDebug() << "Could not move to previous block";
        }
        const int startPosition = cursor.position();
        cursor.setPosition(startPosition + currentHorizontalPosition);

        setTextCursor(cursor);
    }
}

void NoteWidget::moveBlockDown()
{
    QTextCursor cursor = textCursor();

    QString currentBlock = cursor.block().text();
    const int currentHorizontalPosition = cursor.positionInBlock();

    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    // also remove empty line
    cursor.deleteChar();

    if (!cursor.movePosition(QTextCursor::EndOfBlock)) {
        // block below is empty, this is fine
    }
    // insert the removed block again
    cursor.insertBlock();
    cursor.insertText(currentBlock);

    // move cursor to next block
    if (!cursor.movePosition(QTextCursor::StartOfBlock)) {
        qDebug() << "Could not move to start of next block";
    }
    const int startPosition = cursor.position();
    cursor.setPosition(startPosition + currentHorizontalPosition);

    setTextCursor(cursor);
}

void NoteWidget::fontBlod()
{
    // 获取当前光标
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection()) {
        return; // 没有选中文本，直接返回
    }
    // 创建字符格式并设置加粗
    QTextCharFormat format;
    format.setFontWeight(QFont::Bold); // 加粗
    cursor.mergeCharFormat(format);
}

void NoteWidget::fontcancelBlod()
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection()) {
        return; // 没有选中文本，直接返回
    }

    QTextCharFormat format;
    format.setFontWeight(QFont::Normal); // 取消加粗（Normal=400，Bold=700）
    cursor.mergeCharFormat(format);      // 应用到选中文本
}

void NoteWidget::fontTilt()
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection()) {
        return; // 没有选中文本，直接返回
    }

    QTextCharFormat format;
    format.setFontItalic(true);  // 设置斜体
    cursor.mergeCharFormat(format);      // 应用到选中文本
}

void NoteWidget::fontcancelTilt()
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection()) {
        return; // 没有选中文本，直接返回
    }

    QTextCharFormat format;
    format.setFontItalic(false);  // 设置斜体
    cursor.mergeCharFormat(format);      // 应用到选中文本
}

void NoteWidget::fontStrikethrough()
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection()) {
        return; // 没有选中文本，直接返回
    }

    QTextCharFormat format;
    format.setFontStrikeOut(true);  // 关键方法：设置删除线
    cursor.mergeCharFormat(format);      // 应用到选中文本
}

void NoteWidget::fontcancelStrikethrough()
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection()) {
        return; // 没有选中文本，直接返回
    }

    QTextCharFormat format;
    format.setFontStrikeOut(false);  // 关键方法：设置删除线
    cursor.mergeCharFormat(format);      // 应用到选中文本
}

void NoteWidget::fontHighlighter()
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection()) {
        return; // 没有选中文本，直接返回
    }

    QTextCharFormat format;
    format.setBackground(QBrush(QColor(Qt::yellow)));  // 关键代码：设置背景色
    cursor.mergeCharFormat(format);      // 应用到选中文本
}

void NoteWidget::fontcancelHighlighter()
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection()) {
        return; // 没有选中文本，直接返回
    }

    QTextCharFormat format;
    format.setBackground(Qt::transparent);  // 关键代码：透明背景
    cursor.mergeCharFormat(format);      // 应用到选中文本
}
