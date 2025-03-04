
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
#include "version.h"

#if defined(QT_NO_DEBUG_OUTPUT)
#  undef qDebug
#  define qDebug QT_NO_QDEBUG_MACRO
#endif

#ifdef DEBUG_MODE
    #pragma message ("Compiling in DEBUG MODE")
    #define DEBUG_LOG QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).debug
#else
    #define NO_LOG_QDEBUG_HACK while (false) QMessageLogger().noDebug
    #define DEBUG_LOG QT_NO_QDEBUG_MACRO
    #pragma message ("Compiling in RELEASE MODE")
#endif

void listResources(const QString &path) {
    DEBUG_LOG()  << "Listing resources in " << path;
    QDirIterator it(path, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        DEBUG_LOG() << it.next();
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // ✅ Set up argument parser
    QCommandLineParser parser;
    parser.setApplicationDescription("Qt System Tray Notification");
    parser.addHelpOption();
    parser.addVersionOption();

    std::string app_version = version::GetAppVersion(true);
    QCoreApplication::setApplicationVersion(app_version.c_str());

    parser.addOption({{"t", "title"}, "Notification title", "string"});
    parser.addOption({{"m", "message"}, "Notification message", "string"});
    parser.addOption({{"p", "preset"}, "Preset (1-5)", "int"});
    parser.addOption({{"c", "category"}, "Notification category (system, critical, network, etc.)", "string"});
    parser.addOption({{"d", "delay"}, "Notification delay in milliseconds", "ms", "3000"});
    parser.addOption({{"i", "icons"}, "List available icons in the embedded resources"});

    parser.process(app);

    DEBUG_LOG() << "Debug mode is enabled!";

    // ✅ Handle "--icons" flag
    if (parser.isSet("icons")) {
        listResources(":/");
        return 0;
    }

    // ✅ Extract arguments
    QString title = parser.value("title");
    QString message = parser.value("message");
    int preset = parser.isSet("preset") ? parser.value("preset").toInt() : 0;
    QString category = parser.value("category").toLower();
    int delay = parser.isSet("delay") ? parser.value("delay").toInt() : 3000;

    // ✅ Define preset notifications (1-5)
    QMap<int, QPair<QString, QString>> presetMap = {
        {1, {"VPN DISCONNECTED", "VPN connection was disconnected."}},
        {2, {"TORRENTS COMPLETED", "Download Completed."}},
        {3, {"LOW BATTERY", "Battery level is below 10%!"}},
        {4, {"UPDATE AVAILABLE", "A new system update is ready to install."}},
        {5, {"SECURITY WARNING", "Unusual login activity detected."}}
    };

    QMap<int, QString> presetIcons = {
        {1, ":/icons/vpn.png"},
        {2, ":/icons/alert.png"},
        {3, ":/icons/warning.png"},
        {4, ":/icons/non-ionizing-radiation.png"},
        {5, ":/icons/flammable.png"}
    };

    // ✅ Apply preset if selected
    if (presetMap.contains(preset)) {
        title = presetMap[preset].first;
        message = presetMap[preset].second;
        category = "";  // Ignore category if preset is used
    }

    // ✅ Icon mapping for categories
    QMap<QString, QString> iconMap = {
        {"system", ":/icons/flammable.png"},
        {"alert", ":/icons/alert.png"},
        {"critical", ":/icons/flammable.png"},
        {"radiation", ":/icons/non-ionizing-radiation.png"},
        {"notify", ":/icons/notify.png"},
        {"vpn", ":/icons/vpn.png"},
        {"warning", ":/icons/warning.png"},
        {"network", ":/icons/wifi.png"}
    };

    // ✅ Select icon based on preset or category
    QString strIconPath;
    if (presetIcons.contains(preset)) {
        strIconPath = presetIcons[preset];
    } else {
        strIconPath = iconMap.value(category, ":/icons/warning.png");
    }

    DEBUG_LOG() << "Title:" << title;
    DEBUG_LOG() << "Icon Path:" << strIconPath;
    DEBUG_LOG() << "Message:" << message;
    DEBUG_LOG() << "Preset:" << preset;
    DEBUG_LOG() << "Category:" << category;
    DEBUG_LOG() << "Delay:" << delay << "ms";

    QIcon *pIcon = new QIcon(strIconPath);
    if (!pIcon || pIcon->isNull()) {
        qWarning() << "Failed to load icon " << strIconPath;
    }

    // ✅ Ensure system tray is available
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        DEBUG_LOG() << "System tray is not available!";
        return 0;
    }

    const QIcon &icon = *pIcon;
    // ✅ Create and show system tray notification
    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(icon);
    trayIcon.show();
    trayIcon.showMessage(title, message, icon, delay);

    // ✅ Hide tray icon after notification
    QTimer::singleShot(delay, [&]() {
        trayIcon.hide();
        app.quit();
    });

    return app.exec();
}
