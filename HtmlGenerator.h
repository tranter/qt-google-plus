#ifndef HTMLGENERATOR_H
#define HTMLGENERATOR_H

#include <QVariant>

class HtmlGenerator
{
public:
    HtmlGenerator();

    static QString createHtmlForPerson(const QVariant & json);
    static QString createContentForActivity(const QVariant & json);
    static QString createHtmlForActivity(const QVariant & json);
    static QString createHtmlForActivities(const QVariantList & jsonObjects);
    static QString createHtmlForComment(const QVariant & json);
    static QString createHtmlForComments(const QVariantList & jsonObjects);

    static QString loadingHtml();
    static QString emptyInfoHtml();

private:
    static QString templateHtml;
};

#endif // HTMLGENERATOR_H
