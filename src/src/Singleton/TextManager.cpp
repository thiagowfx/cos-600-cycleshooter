#include "TextManager.hpp"

namespace Cycleshooter {

template<> TextManager* Ogre::Singleton<TextManager>::msSingleton = 0;

TextManager::TextManager() {
    overlayMgr = Ogre::OverlayManager::getSingletonPtr();

    overlay = overlayMgr->create("overlay1");
    overlay->setZOrder(10);
    panel = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "container1"));
    panel->setDimensions(1, 1);
    panel->setPosition(0, 0);
    panel->setEnabled(true);

    overlay->add2D(panel);
    overlay->show();
}

void TextManager::addTextBox(const std::string &ID, const std::string &text, Ogre::Real x, Ogre::Real y, Ogre::Real width, Ogre::Real height, const Ogre::ColourValue &color) {

    std::cout << "=======================" << std::endl;

    Ogre::OverlayElement* textBox = overlayMgr->createOverlayElement("TextArea", ID);

    textBox->setDimensions(width, height);
    textBox->setMetricsMode(Ogre::GMM_PIXELS);
    textBox->setPosition(x, y);
    textBox->setWidth(width);
    textBox->setHeight(height);
    textBox->setParameter("font_name", "SdkTrays/Value");
    textBox->setParameter("char_height", "1.0");
    textBox->setColour(color);
    textBox->setCaption(text);

    panel->addChild(textBox);
}

void TextManager::removeTextBox(const std::string &ID) {
    panel->removeChild(ID);
    overlayMgr->destroyOverlayElement(ID);
}

void TextManager::setText(const std::string &ID, const std::string &Text) {
    overlayMgr->getOverlayElement(ID)->setCaption(Text);
}

const std::string TextManager::getText(const std::string &ID) const {
    return overlayMgr->getOverlayElement(ID)->getCaption();
}

}
