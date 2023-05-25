#ifndef CONNECTION_CLIENT_H
#define CONNECTION_CLIENT_H

#include <QListWidgetItem>
#include <QtNetwork/QTcpSocket>
#include "abstracttypes.h"

namespace Connection
{
    /// Basically a TCP socket representing chat client connection
    class ChatClient : public QObject
    {
        friend class ChatClientItem;

        Q_OBJECT

        QTcpSocket* connection;
        MainWindow& mw;

    public:
        QString Name;
        enum class ChatType {
            INCOMING,
            OUTCOMING
        } Type;

        explicit ChatClient(QTcpSocket* instance, MainWindow& mw, ChatType t = ChatType::OUTCOMING);
        ~ChatClient();

        /// Sending TCP packet to peer
        void SendPacket(AbstractTcpPacket& pkg);

        /// Updates client information (name)
        void UpdateInformation(QString name);

    private:
        /// Returns string representation of a chat client
        const QString ToString() const;

    private slots:
        /// Slot which invokes when socket receives a TCP packet
        void ReceivePacket();

        /// Slot for disconnecting
        void CloseConnection();
    };

    /// Chat client representation on a List Widget
    class ChatClientItem : public QListWidgetItem
    {
        ChatClient * const link;

    public:
        explicit ChatClientItem(ChatClient* link)
            : link(link)
        {
            UpdateText();
        }
        ~ChatClientItem() = default;

        /// Returns the only chat client pointer, linked to this item
        ChatClient * const GetLink() const
        {
            return link;
        }

        /// Updates item's text field
        void UpdateText()
        {
            setText(link->ToString());
        }
    };
} // namespace Connection

#endif // CONNECTION_CLIENT_H
