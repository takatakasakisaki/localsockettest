#ifndef COMMCLIENT_H
#define COMMCLIENT_H


#include <QObject>
#include <QThread>
#include <QLocalServer>
#include <QLocalSocket>
#include <QSharedPointer>
#define PIPENAME "\\\\.\\pipe\\pipecom"
class commclient: public QObject
{
	Q_OBJECT
public:
	commclient();
	~commclient();
	bool Connect(int tmo=0);
	QSharedPointer<QLocalSocket> m_socket;
	char  m_barray[128];
	int m_wptr;
	int m_msglen;
	bool send(char msg[], int len, int tmo);
	bool recv(char msg[], int len, int tmo);
	void connected();
};

#endif // COMMCLIENT_H
