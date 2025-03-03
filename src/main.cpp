
// +--------------------------------------------------------------------------------+
// |                                                                                |
// |   main.cpp                                                                     |
// |                                                                                |
// +--------------------------------------------------------------------------------+
// |   Guillaume Plante <codegp@icloud.com>                                         |
// |   Code licensed under the GNU GPL v3.0. See the LICENSE file for details.      |
// +--------------------------------------------------------------------------------+

#include <QApplication>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QTimer>
#include <QCommandLineParser>
#include <QDebug>
#include <QDirIterator>

void listResources(const QString &path) {
    qDebug() << "Listing resources in:" << path;

    QDirIterator it(path, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filePath = it.next();
        qDebug() << filePath;
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // ✅ Set up argument parser
    QCommandLineParser parser;
    parser.setApplicationDescription("Qt System Tray Notification");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOption({{"t", "title"}, "Notification title", "string"});
    parser.addOption({{"m", "message"}, "Notification message", "string"});
    parser.addOption({{"p", "priority"}, "Notification priority (low, normal, high)", "low|normal|high"});
    parser.addOption({{"c", "category"}, "Notification category (system, critical, network)", "system|critical|network"});
    parser.addOption({{"d", "delay"}, "Notification delay in milliseconds", "ms", "3000"});
    parser.addOption({{"i", "icons"}, "List available icons in the embedded resources"});

    parser.process(app);

    // ✅ Handle "--icons" flag
    if (parser.isSet("icons")) {
        listResources(":/");
        return 0;
    }

    // ✅ Extract arguments
    QString title = parser.value("title");
    QString message = parser.value("message");
    QString priority = parser.value("priority").toLower();
    QString category = parser.value("category").toLower();

    int delay = 5000;
    if (parser.isSet("delay")) {
        delay = parser.value("delay").toInt();
    }

    if (title.isEmpty() || message.isEmpty()) {
        qWarning() << "Error: --title and --message are required.";
        return 1;
    }

    qDebug() << "Title:" << title;
    qDebug() << "Message:" << message;
    qDebug() << "Priority:" << priority;
    qDebug() << "Category:" << category;
    qDebug() << "Delay:" << delay << "ms";

    // ✅ Select icon based on category
    QIcon icon;
    if (category == "system") {
        icon = QIcon(":/icons/system.png");   // Ensure this exists in resources.qrc
    } else if (category == "critical") {
        icon = QIcon(":/icons/critical.png"); // Ensure this exists in resources.qrc
    } else if (category == "network") {
        icon = QIcon(":/icons/network.png");  // Ensure this exists in resources.qrc
    } else {
        icon = QIcon(":/icons/warning.png");  // Default fallback
    }

    if (icon.isNull()) {
        qWarning() << "Failed to load icon!";
    }

    // ✅ Ensure system tray is available
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qWarning("System tray is not available! Falling back to notify-send.");
        return 0;
    }

    // ✅ Create and show system tray notification
    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(icon);
    trayIcon.show();
    trayIcon.showMessage(title, message, icon, delay);

    // ✅ Hide tray icon after notification
    QTimer::singleShot(delay + 500, [&]() {
        trayIcon.hide();
        app.quit();
    });

    return app.exec();
}
