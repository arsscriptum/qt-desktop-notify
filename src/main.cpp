
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
#include "resources.h"

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

    ResourcesUtils::Initialize();

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

    // ✅ Define preset notifications (1-15)
    QMap<int, QPair<QString, QString>> presetMap = {
        {1, {"VPN DISCONNECTED", "VPN connection was disconnected."}},
        {2, {"TORRENTS COMPLETED", "Download Completed."}},
        {3, {"LOW BATTERY", "Battery level is below 10%!"}},
        {4, {"UPDATE AVAILABLE", "A new system update is ready to install."}},
        {5, {"SECURITY WARNING", "Unusual login activity detected."}},
        {6, {"EMAIL RECEIVED", "You have a new unread email."}},
        {7, {"NEW MESSAGE", "You received a new instant message."}},
        {8, {"PRINTER ERROR", "Printer is out of paper or jammed."}},
        {9, {"WEATHER ALERT", "Severe weather warning issued."}},
        {10, {"USB DEVICE CONNECTED", "A new USB device has been detected."}},
        {11, {"DISK ERROR", "Disk read/write failure detected!"}},
        {12, {"MEETING REMINDER", "Your scheduled meeting is starting soon."}},
        {13, {"FILE DOWNLOAD STARTED", "A file download has been initiated."}},
        {14, {"FILE DOWNLOAD COMPLETE", "Your file download has finished."}},
        {15, {"LOW MEMORY", "System memory usage is critically high."}}
    };

    // ✅ Define preset icons
    QMap<int, QString> presetIcons = {
        {1, "vpn.png"},
        {2, "alert.png"},
        {3, "warning.png"},
        {4, "non-ionizing-radiation.png"},
        {5, "flammable.png"},
        {6, "package-delivered.png"},
        {7, "youtube.png"},
        {8, "urgent.png"},
        {9, "alarm64.png"},
        {10, "warning1.png"},
        {11, "vault.png"},
        {12, "warning.png"},
        {13, "sign.png"},
        {14, "package-delivered.png"},
        {15, "warning2.png"}
    };

    // ✅ Apply preset if selected
    if (presetMap.contains(preset)) {
        title = presetMap[preset].first;
        message = presetMap[preset].second;
        category = "";  // Ignore category if preset is used
    }

    // ✅ Icon mapping for categories
    QMap<QString, QString> iconMap = {
        {"system", "flammable.png"},
        {"alert", "alert.png"},
        {"critical", "flammable.png"},
        {"radiation", "non-ionizing-radiation.png"},
        {"notify", "notify.png"},
        {"vpn", "vpn.png"},
        {"warning", "warning.png"},
        {"network", "wifi.png"}
    };

    // ✅ Apply preset if selected
    if (presetMap.contains(preset)) {
        title = presetMap[preset].first;
        message = presetMap[preset].second;
        category = "";  // Ignore category if preset is used
    }

    // ✅ Select icon based on preset or category
    QString resourcePath;
    if (presetIcons.contains(preset)) {
        resourcePath = ResourcesUtils::get_iconPath(presetIcons[preset]);
    } else {
        resourcePath = ResourcesUtils::get_iconPath(iconMap.value(category, "warning.png"));
    }

    DEBUG_LOG() << "Title:" << title;
    DEBUG_LOG() << "Icon Path:" << resourcePath;
    DEBUG_LOG() << "Message:" << message;
    DEBUG_LOG() << "Preset:" << preset;
    DEBUG_LOG() << "Category:" << category;
    DEBUG_LOG() << "Delay:" << delay << "ms";

    QIcon *pIcon = new QIcon(resourcePath);
    if (!pIcon || pIcon->isNull()) {
        qWarning() << "Failed to load icon " << resourcePath;
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
    
    ResourcesUtils::Destroy();

    return app.exec();
}
