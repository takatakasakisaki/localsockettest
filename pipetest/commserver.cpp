#include "commserver.h"
#include <QEventLoop>
#include <QTimer>
#include <QLocalSocket>
#include <QDataStream>
#include <algorithm>
#include <cmath>
#include <functional>
#include <QDebug>
#include <stdlib.h>
#include <string.h>

commserver::commserver()
{
	m_exitflag = false;
	m_wptr = 0;
	m_msglen = 0;
}
commserver::~commserver()
{
	if(isRunning()){
		terminate();
	}
}
// parse message
void commserver::parse()
{
	qDebug() << m_msglen << m_wptr;
	QString ss;
	for(int i = 0; i < m_wptr ;i++){
		char buf[4];
		_itoa_s(m_barray[i], buf, sizeof(buf), 16);
		ss += buf;
	}
	qDebug() << ss;
}
// readyread
void commserver::on_recv()
{
	QLocalSocket *socket = (QLocalSocket*)sender();
	if(m_msglen == 0){
		char buf[4]; 
		socket->read(buf, 1);
		m_msglen = uchar(buf[0]);
	}
	if(m_msglen > 0){
		int avail = socket->bytesAvailable();
		if(avail > 0){
			int readlen = std::min(m_msglen, avail); //size to read
			if(readlen + m_wptr > int(sizeof(m_barray))){
				//over size
				readlen = sizeof(m_barray) - m_wptr;
			}
			auto rlen = socket->read(&m_barray[m_wptr], readlen);
			m_wptr += rlen;
			if(m_wptr >= m_msglen){
				//read packet.
				parse();
				m_msglen = 0;
				m_wptr = 0;
			}
			
		}
	}
	
}
void commserver::on_timer()
{
	qDebug() << "tiemr";
}
void commserver::slot_newConnection()
{
	QLocalServer *server = (QLocalServer*)sender();
	auto rv = server ->hasPendingConnections();
	if(rv){
		m_sock = server->nextPendingConnection();
    	connect(m_sock, &QLocalSocket::disconnected,
            m_sock, &QLocalSocket::deleteLater);
		connect(m_sock, &QLocalSocket::readyRead, this, &commserver::on_recv);
		m_msglen = 0;
		m_wptr = 0;
	}
}
void commserver::slot_reqexit()
{
	m_exitflag = true;
}
void commserver::run()
{
	moveToThread(this);
	QTimer timer;
	m_server.reset(new QLocalServer());
	m_server->listen(PIPENAME);
	connect(this, &commserver::sigexit, this, &commserver::slot_reqexit);
	connect(m_server.get(), &QLocalServer::newConnection, this, &commserver::slot_newConnection);
	connect(&timer, &QTimer::timeout, this, &commserver::on_timer);
	QEventLoop evloop;
	timer.start(1000);
	do{
		evloop.processEvents(QEventLoop::WaitForMoreEvents);
	}while(!m_exitflag );
	exit(0);
}
