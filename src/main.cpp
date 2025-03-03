
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
#include <QMap>



void listResources(const QString &path) {
    qDebug() << "Listing resources in:" << path;
    QDirIterator it(path, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        qDebug() << it.next();
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
    parser.addOption({{"c", "category"}, "Notification category (system, critical, network, etc.)", "string"});
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

    int delay = parser.isSet("delay") ? parser.value("delay").toInt() : 3000;

    if (title.isEmpty() || message.isEmpty()) {
        qWarning() << "Error: --title and --message are required.";
        return 1;
    }

    // ✅ Icon mapping (supports all available icons)
    QMap<QString, QString> iconMap = {
        {"system", ":/icons/alarm.png"},
        {"system64", ":/icons/alarm64.png"},
        {"alert", ":/icons/alert.png"},
        {"critical", ":/icons/flammable.png"},
        {"radiation", ":/icons/non-ionizing-radiation.png"},
        {"notify", ":/icons/notify.png"},
        {"delivery", ":/icons/package-delivered.png"},
        {"plex", ":/icons/plex.png"},
        {"sign", ":/icons/sign.png"},
        {"systray", ":/icons/systray.png"},
        {"urgent", ":/icons/urgent.png"},
        {"vault", ":/icons/vault.png"},
        {"vpn", ":/icons/vpn.png"},
        {"warning", ":/icons/warning.png"},
        {"warning1", ":/icons/warning1.png"},
        {"warning2", ":/icons/warning2.png"},
        {"web", ":/icons/web.png"},
        {"network", ":/icons/wifi.png"},
        {"network2", ":/icons/wifi2.jpg"},
        {"youtube", ":/icons/youtube.png"},
        {"youtube64", ":/icons/youtube64.png"}
    };

    // ✅ Select the correct icon based on the category, or use default
    QString strIconPath = iconMap.value(category, ":/icons/warning.png");

    qDebug() << "Title:" << title;
    qDebug() << "Icon Path:" << strIconPath;
    qDebug() << "Message:" << message;
    qDebug() << "Priority:" << priority;
    qDebug() << "Category:" << category;
    qDebug() << "Delay:" << delay << "ms";

    QIcon *pIcon = new QIcon(strIconPath);
    if (!pIcon || pIcon->isNull()) {
        qWarning() << "Failed to load icon " << strIconPath;
    }

    // ✅ Ensure system tray is available
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qWarning("System tray is not available!");
        return 0;
    }

    const QIcon &icon = *pIcon;
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
