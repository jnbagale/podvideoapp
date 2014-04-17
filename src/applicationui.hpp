#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QThread>
#include <QString>
#include <QDebug>

#include <iostream>
#include <string>


extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <fcntl.h>
#include <packedobjectsd/packedobjectsd.h>
}

// XML namespace and url for xpath
#define NS_PREFIX "xs"
#define NS_URL "http://www.w3.org/2001/XMLSchema"

// XML file and schema location

#define XML_EXPORT "file:///accounts/1000/shared/misc/video.xml"
#define XML_SCHEMA "app/native/video.xsd"
#define XML_DATA "app/native/assets/video.xml"
#define XML_RESPONSE "app/native/assets/response.xml"

using namespace std;

namespace bb
{
    namespace cascades
    {
        class Application;
        class LocaleHandler;
    }
}

class ReceiveThread;
class ResponderThread;
class QTranslator;

/*!
 * @brief Application object
 *
 *
 */

class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    // Constructor
    ApplicationUI(bb::cascades::Application *app);
    // De- Constructor
    virtual ~ApplicationUI() { }

    double max_price;
    string video_title;
    QString searcher_ID;
    QString responder_ID;
    packedobjectsdObject *podObjSearcher;
    packedobjectsdObject *podObjResponder;

    packedobjectsdObject *initialiseSearcher();
    packedobjectsdObject *initialiseResponder();

    Q_INVOKABLE QString getSearcherID();
    Q_INVOKABLE QString getResponderID();

    Q_INVOKABLE int sendSearch(QString videoTitle, double maxPrice);
    Q_INVOKABLE int updateNode(QString originalTitle, QString videoTitle,
    		QString videoGenre, QString videoReleaseDate,
    		QString videoDirector, QString videoPrice);
    Q_INVOKABLE int addNode(QString videoTitle, QString videoGenre,
    		QString videoReleaseDate, QString videoDirector, QString videoPrice);
    Q_INVOKABLE int deleteNode(QString originalTitle);
    Q_INVOKABLE int exportXML();

    Q_PROPERTY(QString size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY(QString size1 READ size1 WRITE setSize1 NOTIFY size1Changed)

public Q_SLOTS:

    // get latest search xml size
    QString size();
    void setSize(QString str);

    // get latest search queries
    QString query();
    void setQuery(QString str);

    // get latest response xml size
    QString size1();
    void setSize1(QString str);
    void setSearchResponse();
    void setRecord();

    Q_SIGNALS:
    void sizeChanged();
    void queryChanged();
    void size1Changed();
    void searchResponseChanged();
    void recordChanged();

private:
    QString str_size;
    QString str_query;
    QString str_size1;

private slots:
    void onSystemLanguageChanged();
private:
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;
};

class ReceiveThread : public QThread
 {
     Q_OBJECT
public:
     ApplicationUI *app_Object;
     packedobjectsdObject *podObj_Searcher;
     ReceiveThread(ApplicationUI *app_object, packedobjectsdObject *podObjSearcher);

public slots:
     void process();
 };

class ResponderThread : public QThread
 {
     Q_OBJECT
public:
     ApplicationUI *app_Object;
     packedobjectsdObject *podObj_Searcher;
     packedobjectsdObject *podObj_Responder;
     ResponderThread(ApplicationUI *appObject, packedobjectsdObject *podObjSearcher, packedobjectsdObject *podObjResponder);


public slots:
     void run_responder();
 };

#endif /* ApplicationUI_HPP_ */
