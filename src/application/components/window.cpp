/**
 * @file window.cpp
 * @authors Peter Zdravecký, Eduard Frlička
 * @version 0.1
 * @date 2021-04-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "window.h"

window::window(int maxMessages, QWidget *parent) : QMainWindow(parent) {
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

    this->maxMessageHistory = maxMessages;
    loadConfig();
}

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
    std::ifstream myfile(fileNames[0].toStdString());

    // write image data to message
    char byte = 0;
    while (myfile.get(byte)) {
        message_data.append(byte);
    }

    // send image
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

    if (this->subscribe_text->text().contains("#")) {
        this->setStatusBarText("Character # is not supported in topic name");
        return;
    }
    if (this->subscribe_text->text().trimmed().size() != 0) {
        addNewTopic(this->subscribe_text->text());
        client.subscribe(this->subscribe_text->text().toStdString());
        this->setStatusBarText("Sucessfully added new topic");
        this->subscribe_text->setText("");
        return;
    }
    this->setStatusBarText("Invalid topic name");
}

void window::addMessage(QByteArray msg, QString topicName, int my_message) {
    QListWidgetItem *item = new QListWidgetItem();
    int i;
    // find tree widget of current topic
    QTreeWidgetItem *last_message = findTopicRecursive(topicName, &i);

    // sets default options
    item->setFlags(Qt::ItemIsEnabled);
    item->setData(Qt::DecorationRole, QPalette::Window);
    item->setData(Qt::DecorationRole, QIcon(":/person.ico"));

    // change background color on my message
    if (my_message) {
        item->setData(Qt::BackgroundRole, QColor(Qt::green));
        item->setData(Qt::ForegroundRole, QColor(Qt::black));
    }

    QDateTime dateTime = dateTime.currentDateTime();
    QString currentTime = dateTime.toString("[HH:mm:ss] ");

    QPixmap img;
    img.loadFromData(msg);

    // try parse message payload to image
    if (!img.toImage().isNull()) {
        // set display text of item for image file
        item->setData(Qt::DisplayRole, currentTime + "[image file ↓]");
        last_message->setData(1, Qt::DisplayRole, "[image file]");
    } else {
        if (QString(msg).split("\n").count() > 1) { // check for multiline message
            item->setData(Qt::DisplayRole, currentTime + QString(msg).split("\n")[0] + " ...\n[multiline message ↓]");
        } else if (msg.length() > MAX_MESSAGE_LINE_LENGTH) { // check for too long message
            item->setData(Qt::DisplayRole, currentTime + msg.left(MAX_MESSAGE_LINE_LENGTH) + " ...\n[long message ↓]");
        } else { // normal message
            item->setData(Qt::DisplayRole, currentTime + msg);
        }

        // show in widget tree only first line of arrived message
        auto message_lines = QString(msg).split("\n");
        if (message_lines.count() > 1)
            last_message->setData(1, Qt::DisplayRole, message_lines[0] + " ... [multiline message]");
        else
            last_message->setData(1, Qt::DisplayRole, msg);
    }

    // set message data payload to custom variable in item
    item->setData(Qt::UserRole, msg);

    // append new message to messages history
    if (messages[topicName].size() + 1 > this->maxMessageHistory)
        messages[topicName].removeFirst();

    messages[topicName].append(item);

    // add item to current wiew
    if (topicName == QByteArray::fromStdString(this->client.currentTopic)) {
        listWidget->addItem(item);
        listWidget->scrollToBottom();
    }

    // all messages view
    if (listWidget_all->count() + 1 > this->maxMessageHistory)
        delete listWidget_all->item(0);

    // append arrived message to list of all messages
    QListWidgetItem *item_to_all = item->clone();
    item_to_all->setData(Qt::DisplayRole, "[" + topicName + "]\n" + item_to_all->data(Qt::DisplayRole).value<QString>());
    item_to_all->setData(Qt::DecorationRole, QIcon(":/person.ico"));
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
    emit this->unsubscribe_signal(QString::fromStdString(this->client.currentTopic));
    unsubscribe_topic(QString::fromStdString(this->client.currentTopic));
}

void window::unsubscribe_topic(QString topicName) {
    int i;
    QTreeWidgetItem *topic = findTopicRecursive(topicName, &i);

    if (this->client.unsubscribe(topicName.toStdString())) {
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

    // clear all messages from listWidget
    while (this->listWidget->count() > 0) {
        this->listWidget->takeItem(0);
    }

    // delete topic messages history
    messages[topicName].clear();

    // disable panel
    unsubscribe_btn->setEnabled(0);
    textEdit->setEnabled(0);
    send_btn->setEnabled(0);
    attachFile_btn->setEnabled(0);
    listWidget->setEnabled(0);
    this->client.setCurrentTopic("");
}

void window::setStatusBarText(QString msg) {
    this->statusBar()->showMessage(msg, 1000);
}

void window::on_actionSnapshot_triggered(bool checked) {

    std::string root_dir = QFileDialog::getExistingDirectory(this, "Snapshot").toStdString();
    // QString file = QFileDialog::getSaveFileName(this, "Snapshot", "payload.txt","",nullptr, QFileDialog::ShowDirsOnly);
    if (root_dir == "")
        return;

    QMap<QString, QList<QListWidgetItem *>>::iterator i;
    std::filesystem::current_path(std::filesystem::path(root_dir));
    for (i = messages.begin(); i != messages.end(); ++i) {
        std::filesystem::create_directory(i.key().toStdString());
        auto curr_dir = root_dir + "/" + i.key().toStdString();
        std::ofstream out(curr_dir + "/payload.txt");
        int cnt = 0;
        for (auto j : i.value()) {
            QPixmap img;
            QByteArray msg = j->data(Qt::UserRole).toByteArray();
            img.loadFromData(msg);
            // obrazok
            if (!img.toImage().isNull()) {
                std::string image_name = curr_dir + "/image" + std::to_string(cnt++);
                out << "[ image ]: " << image_name << std::endl;
                QFile file(QString::fromStdString(image_name));
                file.open(QIODevice::WriteOnly);
                file.write(msg);
                file.close();

            } else {
                out << msg.toStdString() << std::endl;
            }
        }
        out.close();
    }

    this->setStatusBarText("Snapshot sucessfully created");
}

void window::on_addWidget_btn_clicked() {
    QSettings settings;

    QString name = widget_name->text();
    QString topic = widget_topic->text();
    int type = this->widget_combobox->currentIndex();

    if (addWidget(name, topic, type, settings.value("widget_count").toString())) {
        auto widgetID = settings.value("widget_count");
        settings.beginGroup("widget" + widgetID.toString());
        settings.setValue("name", name);
        settings.setValue("topic", topic);
        settings.setValue("type", type);
        settings.endGroup();
        settings.setValue("widget_count", widgetID.toInt() + 1);
    }

    widget_name->setText("");
    widget_topic->setText("");
}

int window::addWidget(QString name, QString topic, int type, QString widgetID) {
    QSettings settings;

    if (name.trimmed().size() == 0 || topic.trimmed().size() == 0) {
        setStatusBarText("Name and topic is required!");
        return false;
    }

    if (topic.contains("#")) {
        this->setStatusBarText("Character # is not supported in topic name");
        return false;
    }

    switch (type) {
    case 0: { // lights
        LightWidget *item = new LightWidget(this, &client, name, topic, widgetID);
        addNewTopic(topic);
        client.subscribe(topic.toStdString());

        this->widgets_layout->insertWidget(0, item);
        break;
    }
    case 1: { // thermostat
        ThermostatWidget *item = new ThermostatWidget(this, &client, name, topic, widgetID);
        addNewTopic(topic);
        client.subscribe(topic.toStdString());

        this->widgets_layout->insertWidget(0, item);
        break;
    }
    case 2: { // thermostat
        CoffeWidget *item = new CoffeWidget(this, &client, name, topic, widgetID);
        addNewTopic(topic);
        client.subscribe(topic.toStdString());

        this->widgets_layout->insertWidget(0, item);
        break;
    }
    case 3: { // thermometer
        ThermometerWidget *item = new ThermometerWidget(this, &client, name, topic, widgetID);
        addNewTopic(topic);
        client.subscribe(topic.toStdString());

        this->widgets_layout->insertWidget(0, item);
        break;
    }
    case 4: { // camera
        CameraWidget *item = new CameraWidget(this, &client, name, topic, widgetID);
        addNewTopic(topic);
        client.subscribe(topic.toStdString());

        this->widgets_layout->insertWidget(0, item);
        break;
    }
    default: {
        return false;
        break;
    }
    }

    this->setStatusBarText("Sucessfully added new widget");
    return true;
}

void window::closeEvent(QCloseEvent *event) {
    this->messages.clear();
    this->listWidget_all->clear();
    this->treeWidget->clear();
    this->client.disconnect();
}

void window::loadConfig() {
    QSettings settings;

    if (!settings.contains("widget_count"))
        settings.setValue("widget_count", 0);

    for (auto group : settings.childGroups()) {
        if (group.contains("widget")) {
            settings.beginGroup(group);
            QRegExp rx("\\d+");
            rx.indexIn(group);
            addWidget(settings.value("name").toString(), settings.value("topic").toString(), settings.value("type").toInt(), rx.capturedTexts()[0]);
            settings.endGroup();
        }
    }
}