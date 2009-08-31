#ifndef MESSAGESENDERTHREAD_H
#define MESSAGESENDERTHREAD_H

#include <QThread>

class MessageSenderThread : public QThread
{
    Q_OBJECT
public:
    MessageSenderThread();

    //void run();
};

#endif // MESSAGESENDERTHREAD_H
