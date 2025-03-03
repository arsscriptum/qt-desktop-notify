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
#include <QMessageBox>
#include <QTimer>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Ensure an argument is provided
    if (argc < 2) {
        QMessageBox::critical(nullptr, "Error", "Usage: notify '<message>'");
        return 1;
    }

    QString message = argv[1];

    // Check if system supports tray icon
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, "Error", "System tray not available.");
        return 1;
    }

    QSystemTrayIcon trayIcon;
    trayIcon.show();  // Required for notifications on some desktops

    // Show notification
    trayIcon.showMessage("Notification", message, QSystemTrayIcon::Information, 3000); // 3 seconds

    // Exit after a short delay to allow notification to be displayed
    QTimer::singleShot(3500, &app, &QApplication::quit);

    return app.exec();
}
