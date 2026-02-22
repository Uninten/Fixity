#include "T_Note.h"
#include <QInputDialog>
#include <QShortcut>
#include <QScrollBar>

T_Note::T_Note(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    //setWindowTitle("绘图");
    // 顶部元素
    createCustomWidget("");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("笔记");

    QHBoxLayout* noteLayout = new QHBoxLayout();
    noteLayout->setContentsMargins(0, 0, 12, 0);

//    preview = new QTextBrowser(this);
    editor = new NoteWidget(this);
//    editor->setAcceptRichText(false); // 禁用富文本，纯Markdown
    QFont font;
    font.setFamily("Arial");  // 设置字体（可选）
    font.setPointSize(14);    // 设置字体大小（单位：磅）
    editor->setFont(font);
    editor->setMinimumHeight(830);
    editor->setMaximumHeight(QWIDGETSIZE_MAX);
    editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    editor->setStyleSheet("QTextEdit { background: transparent; border: none;}");
    noteLayout->addWidget(editor);

    // 放在中心位置
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addLayout(noteLayout);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
    connect(editor, &QTextEdit::textChanged, this, &T_Note::renderMarkdown);
    // 绑定快捷键切换模式（Ctrl+M）
    auto *shortcut = new QShortcut(QKeySequence("Ctrl+M"), this);
    connect(shortcut, &QShortcut::activated, this, &T_Note::toggleRenderMode);
    // 绑定光标移动信号
    connect(editor, &QTextEdit::cursorPositionChanged,
            this, &T_Note::onCursorPositionChanged);
}

T_Note::~T_Note()
{

}

QString T_Note::markdownToHtml(const QString &markdown)
{
    QString html = markdown;

    html.replace("\\theta", "θ");
    html.replace("\\alpha", "α");
    html.replace("\\beta", "β");
    html.replace("\\gamma", "γ");
    html.replace("\\pi", "π");
    html.replace("\\infty", "∞");
    // 更精确的上标匹配（避免在URL等地方误匹配）
    html.replace(QRegularExpression(R"(([^\s\\])\^\{([^}]+)\})"), "\\1<sup>\\2</sup>");
    html.replace(QRegularExpression(R"(([^\s\\])\^([^\s^_]+))"), "\\1<sup>\\2</sup>");

    // 更精确的下标匹配
    html.replace(QRegularExpression(R"(([^\s\\])\_\{([^}]+)\})"), "\\1<sub>\\2</sub>");
    html.replace(QRegularExpression(R"(([^\s\\])\_([^\s^_]+))"), "\\1<sub>\\2</sub>");
    // 数学公式：块公式 $$
    html.replace(QRegularExpression(R"(\$\$(.*?)\$\$)"), "<div class=\"math-block\">\\1</div>");

    // 数学公式：行内公式 $
    html.replace(QRegularExpression(R"(\$(.*?)\$)"), "<span class=\"math-inline\">\\1</span>");

    // 标题（# → h1, ## → h2, ### → h3, #### → h4, ##### → h5, ###### → h6）
    html.replace(QRegularExpression("^# (.*)$", QRegularExpression::MultilineOption), "<h1>\\1</h1>");
    html.replace(QRegularExpression("^## (.*)$", QRegularExpression::MultilineOption), "<h2>\\1</h2>");
    html.replace(QRegularExpression("^### (.*)$", QRegularExpression::MultilineOption), "<h3>\\1</h3>");
    html.replace(QRegularExpression("^#### (.*)$", QRegularExpression::MultilineOption), "<h4>\\1</h4>");
    html.replace(QRegularExpression("^##### (.*)$", QRegularExpression::MultilineOption), "<h5>\\1</h5>");
    html.replace(QRegularExpression("^###### (.*)$", QRegularExpression::MultilineOption), "<h6>\\1</h6>");

    // 粗体（**text** → <strong>text</strong>）
    html.replace(QRegularExpression("\\*\\*(.*?)\\*\\*"), "<strong>\\1</strong>");

    // 斜体（*text* → <em>text</em>）
    html.replace(QRegularExpression("\\*(.*?)\\*"), "<em>\\1</em>");

    // 列表（- item → <li>item</li>）
    html.replace(QRegularExpression("^- (.*)$", QRegularExpression::MultilineOption), "<li>\\1</li>");
    html.replace(QRegularExpression("(<li>.*</li>)", QRegularExpression::DotMatchesEverythingOption), "<ul>\\1</ul>");

    // 链接（[text](url) → <a href="url">text</a>）
    html.replace(QRegularExpression("\\[(.*?)\\]\\((.*?)\\)"), "<a href=\"\\2\">\\1</a>");

    // 代码块（```code``` → <pre><code>code</code></pre>）
    html.replace(QRegularExpression("```([\\s\\S]*?)```"), "<pre><code>\\1</code></pre>");

//    return "<html><body>" + html + "</body></html>";
    return R"(
            <html>
            <head>
                <script src="https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-mml-chtml.js"></script>
                <script>
                MathJax = {
                    tex: {
                        inlineMath: [['$', '$'], ['\\(', '\\)']]
                    }
                };
                </script>
            </head>
            <body>)" + html + R"(</body>
            </html>
        )";
}

void T_Note::renderMarkdown()
{
    QString markdownText = editor->toPlainText();
    QString html = markdownToHtml(markdownText); // 将Markdown转为HTML
//    preview->setHtml(html);
}

void T_Note::loadFile(const QString &filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString content = file.readAll();
//        editor->setMarkdown(content); // 关键方法：设置Markdown内容
        file.close();
        currentFile = filename;
    }
}

void T_Note::onCursorPositionChanged()
{
    QTextCursor cursor = editor->textCursor();
    QTextCharFormat format = cursor.charFormat();

    // 检查是否在公式元素附近（通过自定义属性或格式）
    bool nearFormula = format.hasProperty(QTextFormat::UserProperty) ||
                      cursor.block().text().contains("$");

    if (nearFormula != isNearFormula) {
        isNearFormula = nearFormula;
        editor->setReadOnly(!isNearFormula);  // 自动切换编辑状态

        // 可选：视觉反馈（如高亮公式区域）
        highlightFormulaArea(cursor.position());
    }
}

void T_Note::toggleRenderMode()
{
    if (isRendered) {
        // 切回编辑模式：显示原始 Markdown
        editor->setPlainText(originalMarkdown);
        isRendered = false;
    } else {
        // 切换到渲染模式：保存原始文本并渲染
        originalMarkdown = editor->toPlainText();
        QString html = markdownToHtml(originalMarkdown);
        editor->setHtml(html);
        isRendered = true;
    }
}

void T_Note::highlightFormulaArea(int pos)
{
    QList<QTextEdit::ExtraSelection> extras;

    if (isNearFormula) {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(QColor(255, 255, 200)); // 浅黄色背景
        selection.cursor = editor->textCursor();
        selection.cursor.select(QTextCursor::WordUnderCursor);
        extras.append(selection);
    }

    editor->setExtraSelections(extras);
}

