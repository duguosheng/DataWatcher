#include "highlighter.h"

/**
 * @brief Highlighter::Highlighter 文本高亮支持
 * @param parent 父类
 */
Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::red);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bTIMESTAMP\\b" << "\\bINT\\b" << "\\bBIGINT\\b"
                    << "\\bFLOAT\\b" << "\\bDOUBLE\\b" << "\\bBINARY\\b"
                    << "\\bSMALLINT\\b" << "\\bTINYINT\\b" << "\\bBOOL\\b"
                    << "\\bNCHAR\\b" << "\\bCREATE\\b" << "\\bSHOW\\b"
                    << "\\bUSE\\b" << "\\bDROP\\b" << "\\bALTER\\b"
                    << "\\bCOMP\\b" << "\\bREPLICA\\b" << "\\bKEEP\\b"
                    << "\\bQUORUM\\b" << "\\bUSING\\b" << "\\bTAGS\\b"
                    << "\\bDESCRIBE\\b" << "\\bSTABLE\\b" << "\\bINSERT INTO\\b"
                    << "\\bSELECT\\b" << "\\bFROM\\b" << "\\bWHERE\\b"
                    << "\\bINTERVAL\\b" << "\\bSLIDING\\b" << "\\bFILL\\b"
                    << "\\bGROUP BY\\b" << "\\bORDER BY\\b" << "\\bDESC\\b"
                    << "\\bASC\\b" << "\\bSLIMIT\\b" << "\\bSOFFSET\\b"
                    << "\\bLIMIT\\b" << "\\bOFFSET\\b" << ">{2}" << "\\bSOURCE\\b"
                    << "\\bDISTINCT\\b" << "\\bAS\\b" << "\\bfirst\\b"
                    << "\\blast\\b" << "\\blast_row\\b" << "\\bNOW\\b";
    foreach (const QString &pattern, keywordPatterns) {
        //设置不区分大小写
        rule.pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    numberFormat.setForeground(Qt::darkYellow);
    rule.pattern = QRegularExpression("\\b\\d+\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(QColor::fromRgb(60, 179, 113));
    singleLineCommentFormat.setFontItalic(true);
    rule.pattern = QRegularExpression("--[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
    multiLineCommentFormat.setForeground(QColor::fromRgb(46, 139, 87));
    multiLineCommentFormat.setFontItalic(true);

    quotationFormat.setForeground(QColor::fromRgb(255, 165, 0));
    rule.pattern = QRegularExpression("\'.*\'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

/**
 * @brief Highlighter::highlightBlock 实现高亮必须重写此函数
 * @param text 文本
 */
void Highlighter::highlightBlock(const QString &text) {
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
