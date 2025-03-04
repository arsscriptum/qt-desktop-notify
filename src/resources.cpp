//==============================================================================
//
//  resources.cpp
//
//==============================================================================
//  automatically generated on 2025-03-04 00:54:33
//==============================================================================


#include "resources.h"

// Static member initialization
std::map<std::string, std::string> ResourcesUtils::resourceMap;

void ResourcesUtils::Initialize() {
    loadResourceMap();
}

void ResourcesUtils::Destroy() {
    resourceMap.clear();
}

void ResourcesUtils::loadResourceMap() {
    resourceMap.clear(); // Ensure it's empty before loading
    QDirIterator it(":/", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString fullPath = it.next();
        QString fileName = QFileInfo(fullPath).fileName();

        // Add to map with filename as key and full Qt resource path as value
        resourceMap[fileName.toStdString()] = fullPath.toStdString();
    }
}

QString ResourcesUtils::get_iconPath(const QString& filename) {
    std::string key = filename.toStdString();
    auto it = resourceMap.find(key);
    return (it != resourceMap.end()) ? QString::fromStdString(it->second) : QString();
}

QString ResourcesUtils::get_iconPath(const std::string& filename) {
    auto it = resourceMap.find(filename);
    return (it != resourceMap.end()) ? QString::fromStdString(it->second) : QString();
}
