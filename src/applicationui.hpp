#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QThread>
#include <QString>
#include <iostream>
#include <string>


extern "C" {
#include <packedobjectsd/packedobjectsd.h>
}

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

    string video_title;
    double max_price;
    packedobjectsdObject *pod_object1;
    packedobjectsdObject *pod_object2;

    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(double price READ price WRITE setPrice NOTIFY priceChanged)

    Q_INVOKABLE packedobjectsdObject *initialiseSearcher();
    Q_INVOKABLE int sendSearch(QString videoTitle, double maxPrice);


public Q_SLOTS:
	// set current status of app
    QString status();
    void setStatus(QString str);

    // get latest search response -title
    QString title();
    void setTitle(QString str);

    // get latest search response - price
    double price();
    void setPrice(double val);

    Q_SIGNALS:
    void statusChanged();
    void titleChanged();
    void priceChanged();

private:
    QString str_status;
    QString str_title;
    double dbl_price;


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
     ApplicationUI *app_object1;
     packedobjectsdObject *pod_object3;
     ReceiveThread(ApplicationUI *app_object, packedobjectsdObject *pod_object2);

public slots:
     void process();
 };

class ResponderThread : public QThread
 {
     Q_OBJECT
public:
     ApplicationUI *app_object2;
     packedobjectsdObject *pod_object4;
     packedobjectsdObject *pod_resp_obj;
     ResponderThread(ApplicationUI *app_object, packedobjectsdObject *pod_object2);


public slots:
     void run_responder();
 };

#endif /* ApplicationUI_HPP_ */
