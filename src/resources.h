//==============================================================================
//
//  resources.h
//
//==============================================================================


#ifndef RESOURCESUTILS_H
#define RESOURCESUTILS_H

#include <QResource>
#include <QDirIterator>
#include <QString>
#include <map>
#include <string>

class ResourcesUtils {
public:
    // Initializes the resource map
    static void Initialize();

    // Clears the resource map
    static void Destroy();

    // Gets icon path as QString
    static QString get_iconPath(const QString& filename);

    // Gets icon path as QString
    static QString get_iconPath(const std::string& filename);

private:
    // Loads all Qt resources into a map
    static void loadResourceMap();

    // Stores the resource mappings (Filename -> Full Resource Path)
    static std::map<std::string, std::string> resourceMap;
};

#endif // RESOURCESUTILS_H
