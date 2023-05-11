#ifndef CONNECTION_CLIENT_H
#define CONNECTION_CLIENT_H

#include <QListWidgetItem>
#include <QtNetwork/QTcpSocket>
#include "abstracttypes.h"

namespace Connection
{
    class ChatClient : public QObject
    {
        friend class ChatClientItem;

        Q_OBJECT

        QTcpSocket* connection;
        MainWindow& mw;

    public:
        QString Name;

        explicit ChatClient(QTcpSocket* instance, MainWindow& mw);

        void SendPacket(AbstractTcpPacket& pkg);
        void UpdateInformation(QString name);

    private:
        const QString ToString() const;

    private slots:
        void ReceivePacket();
    };

    class ChatClientItem : public QListWidgetItem
    {
        ChatClient * const link;

    public:
        explicit ChatClientItem(ChatClient* link)
            : link(link)
        {
            UpdateText();
        }

        ChatClient * const GetLink() const
        {
            return link;
        }

        void UpdateText()
        {
            setText(link->ToString());
        }
    };
} // namespace Connection

#endif // CONNECTION_CLIENT_H
