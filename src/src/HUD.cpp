#include "HUD.hpp"

namespace Cycleshooter {

HUD::HUD(const Ogre::String &name, Ogre::RenderWindow *window, OgreBites::InputContext inputContext, OgreBites::SdkTrayListener *listener) :
    mTrayManager(0),
    mDebugPanel(0)
{
    mTrayManager = new OgreBites::SdkTrayManager(name, window, inputContext, listener);

    mTrayManager->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayManager->showLogo(OgreBites::TL_TOPRIGHT);
    mTrayManager->hideCursor();

    // Create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mDebugPanel = mTrayManager->createParamsPanel(OgreBites::TL_BOTTOMRIGHT, "DetailsPanel", 200, items);
    mDebugPanel->setParamValue(9, "Bilinear");
    mDebugPanel->setParamValue(10, "Solid");
}

HUD::~HUD() {
    if(mDebugPanel)
        delete mDebugPanel;
    if(mTrayManager)
        delete mTrayManager;
}

void HUD::toggleDebugPanel() {
    if (mDebugPanel->getTrayLocation() == OgreBites::TL_NONE) {
        mTrayManager->moveWidgetToTray(mDebugPanel, OgreBites::TL_BOTTOMRIGHT, 0);
        mDebugPanel->show();
    }
    else {
        mTrayManager->removeWidgetFromTray(mDebugPanel);
        mDebugPanel->hide();
    }
}

void HUD::toggleFPSPanel() {
    mTrayManager->toggleAdvancedFrameStats();
}

void HUD::update(const Ogre::FrameEvent& evt) {
    mTrayManager->frameRenderingQueued(evt);
}

void HUD::updateDebugPanel_CameraElements(Ogre::Camera* camera) {
    mDebugPanel->setParamValue(0, Ogre::StringConverter::toString(camera->getDerivedPosition().x));
    mDebugPanel->setParamValue(1, Ogre::StringConverter::toString(camera->getDerivedPosition().y));
    mDebugPanel->setParamValue(2, Ogre::StringConverter::toString(camera->getDerivedPosition().z));
    mDebugPanel->setParamValue(4, Ogre::StringConverter::toString(camera->getDerivedOrientation().w));
    mDebugPanel->setParamValue(5, Ogre::StringConverter::toString(camera->getDerivedOrientation().x));
    mDebugPanel->setParamValue(6, Ogre::StringConverter::toString(camera->getDerivedOrientation().y));
    mDebugPanel->setParamValue(7, Ogre::StringConverter::toString(camera->getDerivedOrientation().z));
}

bool HUD::isDebugPanelVisible() const {
    return mDebugPanel->isVisible();
}

std::string HUD::getDebugPanel_PolygonFilteringElement() const {
    mDebugPanel->getParamValue(9).asUTF8();
}

void HUD::setDebugPanel_PolygonFilteringElement(const std::string& newVal) {
    mDebugPanel->setParamValue(9, newVal);
}

std::string HUD::getDebugPanel_PolygonRenderingElement() const {
    mDebugPanel->getParamValue(10).asUTF8();
}

void HUD::setDebugPanel_PolygonRenderingElement(const std::string& newVal) {
    mDebugPanel->setParamValue(10, newVal);
}

OgreBites::SdkTrayManager* HUD::getTrayManager() const {
    return mTrayManager;
}

}
