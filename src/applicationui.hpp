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
#define XML_QUERY "app/native/assets/query.xml"

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

    Q_PROPERTY(QString querySize READ querySize WRITE setquerySize NOTIFY querySizeChanged)
    Q_PROPERTY(QString responseSize READ responseSize WRITE setresponseSize NOTIFY responseSizeChanged)

public Q_SLOTS:

    // get latest search xml size
    QString querySize();
    void setquerySize(QString str);

    // get latest response xml size
    QString responseSize();
    void setresponseSize(QString str);

    void setSearchResponse();
    void setRecord();
    void setQuery();


    Q_SIGNALS:
    void querySizeChanged();
    void responseSizeChanged();
    void queryChanged();
    void recordChanged();
    void searchResponseChanged();
    void responderIDChanged();
    void searcherIDChanged();

private:
    QString str_querySize;
    QString str_responseSize;

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
