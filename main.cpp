#include <QtGui/QApplication>
#include "dialog.h"
#include "server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Server server;
    server.listen( QHostAddress::Any, 9876 );
    Dialog w(&server);
    w.show();
    return a.exec();
}
