#include "window.h"

window::window(QWidget *parent) : QMainWindow(parent) {
    setupUi(this);

    ServerDialog dialog;
    auto dialog_ret = dialog.exec();
    if (dialog_ret == QDialog::Accepted) {
        client.connect(dialog.getServerAdress().toStdString());
    }
    if (dialog_ret == QDialog::Rejected) {
        exit(0);
    }

    connect(&client, SIGNAL(getMessage(QByteArray, QString)), this, SLOT(addMessage(QByteArray, QString)));
    connect(&client, SIGNAL(sendStatusText(QString)), this, SLOT(setStatusBarText(QString)));
}

// SLOTS

void window::on_send_btn_clicked() {
    auto message = textEdit->toPlainText().toStdString();

    if (message.size() == 0)
        return;

    QByteArray message_data(message.c_str(), message.size());
    client.sendMessage(message_data);
    this->addMessage(message_data, QByteArray::fromStdString(this->client.currentTopic), 1);

    textEdit->setText("");
}

void window::on_attachFile_btn_clicked() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Attach Image"), "./", tr("Images (*.png *.jpg *.jpeg *.bmp *.gif *.pbm *.pgm *.ppm *.xbm *.xpm)"));

    if (fileNames.size() == 0)
        return;

    QByteArray message_data;
    // open file
    std::ifstream myfile(fileNames[0].toStdString());

    // write image data to message
    char byte = 0;
    while (myfile.get(byte)) {
        message_data.append(byte);
    }

    client.sendMessage(message_data);
    this->addMessage(message_data, QByteArray::fromStdString(this->client.currentTopic), 1);
}

void window::on_listWidget_itemClicked(QListWidgetItem *item) {

    if (item->data(Qt::UserRole).isNull())
        return;

    QByteArray msg = item->data(Qt::UserRole).value<QByteArray>();

    QPixmap img;
    img.loadFromData(msg);

    // show image
    if (!img.toImage().isNull()) {
        ImageForm *image = new ImageForm();
        image->SetImage(&img);
        image->setMaximumWidth(img.width());
        image->setMaximumHeight(img.height());

        image->show();
    } else {
        MessageForm *message = new MessageForm();
        message->SetText(msg);
        message->show();
    }
}

void window::on_listWidget_all_itemClicked(QListWidgetItem *item) {

    on_listWidget_itemClicked(item);
}

void window::on_subscribe_btn_clicked() {
    SubscribeDialog dialog;
    if (dialog.exec() == QDialog::Rejected)
        return;

    addNewTopic(dialog.getTopicName());
    client.subscribe(dialog.getTopicName().toStdString());
}

// CLASS FUNCTIONS

void window::addMessage(QByteArray msg, QString topicName, int my_message) {

    QListWidgetItem *item = new QListWidgetItem();
    item->setFlags(Qt::ItemIsEnabled);
    item->setData(Qt::DecorationRole, QPalette::Window);
    if (my_message) {
        item->setData(Qt::BackgroundRole, QColor::fromRgb(209, 252, 149));
        item->setData(Qt::DecorationRole, QColor::fromRgb(209, 252, 149));
    }

    QPixmap img;
    img.loadFromData(msg);

    int i;
    QTreeWidgetItem *last_message = findTopicRecursive(topicName, &i);

    if (!img.toImage().isNull()) {

        item->setData(Qt::DisplayRole, "[image file ↓]");
        item->setData(Qt::UserRole, msg);

        // QtreeWiev LastMessage
        last_message->setData(1, Qt::DisplayRole, "[image file]");
    } else {
        if (QString(msg).split("\n").count() > 1) {
            item->setData(Qt::DisplayRole, QString(msg).split("\n")[0] + " ... [multiline message ↓]");
            item->setData(Qt::UserRole, msg);
        } else if (msg.length() > MAX_MESSAGE_LINE_LENGTH) {
            item->setData(Qt::DisplayRole, msg.left(MAX_MESSAGE_LINE_LENGTH) + " ... [long message ↓]");
            item->setData(Qt::UserRole, msg);
        } else {
            item->setData(Qt::DisplayRole, msg);
        }

        // QtreeWiev LastMessage
        auto message_lines = QString(msg).split("\n");
        if (message_lines.count() > 1)
            last_message->setData(1, Qt::DisplayRole, message_lines[0] + " ... [multiline message]");
        else
            last_message->setData(1, Qt::DisplayRole, msg);
    }

    if (messages[topicName].size() + 1 > MAX_MESSAGE_HISTORY)
        messages[topicName].removeFirst();

    messages[topicName].append(item);

    // Add item to current wiew and scroll down
    if (topicName == QByteArray::fromStdString(this->client.currentTopic)) {
        listWidget->addItem(item);
        listWidget->scrollToBottom();
    }

    // All messages view
    if (listWidget_all->count() + 1 > MAX_MESSAGE_HISTORY)
        delete listWidget_all->item(0);

    QListWidgetItem *item_to_all = item->clone();
    item_to_all->setData(Qt::DisplayRole, "[" + topicName + "] - " + item_to_all->data(Qt::DisplayRole).value<QString>());
    listWidget_all->addItem(item_to_all);
    listWidget_all->scrollToBottom();
}

QString getFullTopicName(QStringList list, int end) {
    QString res = "";
    for (int i = 0; i <= end; i++)
        res += (res == "" ? "" : "/") + list[i];
    return res;
}

QString getFullTopicName(QStringList list) {
    return getFullTopicName(list, list.count() - 1);
}

QTreeWidgetItem *window::findTopic(QString topicName) {
    QString server = QString::fromStdString(client.client->get_server_uri());

    QStringList topicList = topicName.split('/');

    auto leaf = topicList.last();

    QList<QTreeWidgetItem *> find_res = treeWidget->findItems(leaf, Qt::MatchExactly | Qt::MatchRecursive);
    for (auto child : find_res) {
        QJsonObject data = child->data(0, Qt::UserRole).value<QJsonObject>();
        QString child_path = data["topicName"].toString();
        if (topicName == child_path)
            return child;
    }

    return NULL;
}

QTreeWidgetItem *window::findTopicRecursive(QString topicName, int *i) {
    QStringList topicList = topicName.split('/');
    for (*i = topicList.count() - 1; *i >= 0; --*i) {
        auto res = findTopic(getFullTopicName(topicList, *i));
        if (res)
            return res;
    }
    return NULL;
}

void window::addNewTopic(QString topicName) {
    QStringList topicList = topicName.split('/');
    topicName = getFullTopicName(topicList);

    int i;
    QTreeWidgetItem *last = findTopicRecursive(topicName, &i);
    i++;
    for (; i < topicList.count(); i++) {
        auto new_topic = new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(topicList[i]));
        QJsonObject data{{"topicName", getFullTopicName(topicList, i)}, {"isSubscribed", false}};
        new_topic->setData(0, Qt::UserRole, data);

        new_topic->setFlags(Qt::ItemIsEnabled);
        if (last != NULL)
            last->addChild(new_topic);
        else
            treeWidget->addTopLevelItem(new_topic);
        last = new_topic;
    }

    QJsonObject data = last->data(0, Qt::UserRole).value<QJsonObject>();
    data["isSubscribed"] = true;
    last->setData(0, Qt::UserRole, data);
}

void window::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column) {
    if (column != 0)
        return;

    QJsonObject data = item->data(column, Qt::UserRole).value<QJsonObject>();
    while (this->listWidget->count() > 0) {
        this->listWidget->takeItem(0);
    }
    if (data["isSubscribed"].toBool()) {
        client.setCurrentTopic(data["topicName"].toString().toStdString());
        for (auto item : messages[data["topicName"].toString()])
            this->listWidget->addItem(item);
        // Enable panel
        textEdit->setEnabled(1);
        send_btn->setEnabled(1);
        attachFile_btn->setEnabled(1);
        listWidget->setEnabled(1);
        unsubscribe_btn->setEnabled(1);
    } else {
        client.setCurrentTopic("");
        // Disable panel
        textEdit->setEnabled(0);
        send_btn->setEnabled(0);
        attachFile_btn->setEnabled(0);
        listWidget->setEnabled(0);
        unsubscribe_btn->setEnabled(0);
    }
}

void window::on_unsubscribe_btn_clicked() {
    int i;
    QTreeWidgetItem *topic = findTopicRecursive(QString::fromStdString(this->client.currentTopic), &i);

    if (this->client.unsubscribe(this->client.currentTopic)) {
        unsubscribe_btn->setDisabled(1);
        return;
    }

    // change status to unsbuscribe
    QJsonObject data = topic->data(0, Qt::UserRole).value<QJsonObject>();
    data["isSubscribed"] = false;
    topic->setData(0, Qt::UserRole, data);
    topic->setData(1, Qt::DisplayRole, "");
    if (topic->childCount() == 0)
        delete topic;

    // clear all
    while (this->listWidget->count() > 0) {
        this->listWidget->takeItem(0);
    }
    messages[QString::fromStdString(this->client.currentTopic)].clear();
    unsubscribe_btn->setEnabled(0);
    textEdit->setEnabled(0);
    send_btn->setEnabled(0);
    attachFile_btn->setEnabled(0);
    listWidget->setEnabled(0);
    this->client.setCurrentTopic("");
}

void window::setStatusBarText(QString msg) {
    this->statusBar()->showMessage(msg, 10);
}