#ifndef COMMSERVER_H
#define COMMSERVER_H

#include <QObject>
#include <QThread>
#include <QLocalServer>
#include <QSharedPointer>
#define PIPENAME "\\\\.\\pipe\\pipecom"
class commserver: public QThread
		
{
	Q_OBJECT
public:
	commserver();
	~commserver();
	void run();
	void slot_reqexit();
	void slot_newConnection();
	void parse();
private slots:
	void on_timer();
	void on_recv();
private:
	QSharedPointer<QLocalServer> m_server;
	QLocalSocket *m_sock;
	char  m_barray[128];
	int m_wptr;
	int m_msglen;
	bool m_exitflag;
	void reqexit()
	{
		m_exitflag = true;
	}
signals:
	void sigexit();
};

#endif // COMMSERVER_H
