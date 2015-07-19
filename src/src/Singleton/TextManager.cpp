#include "TextManager.hpp"

namespace Cycleshooter {

TextManager::TextManager() {
    LOG("Initializing TextManager");

    panel = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "TextManagerPanel"));
    panel->setDimensions(0.0, 0.0);
    panel->setPosition(1.0, 1.0);
    panel->setEnabled(true);

    overlay = Ogre::OverlayManager::getSingleton().create("TextManagerOverlay");
    overlay->add2D(panel);
    overlay->setZOrder(1000);
    overlay->show();
}

void TextManager::addTextBox(const std::string &ID, const std::string &text, Ogre::Real x, Ogre::Real y, Ogre::Real width, Ogre::Real height, const Ogre::ColourValue &color) { 
    Ogre::TextAreaOverlayElement* textBox = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", ID));

    textBox->setMetricsMode(Ogre::GMM_PIXELS);
    textBox->setPosition(x, y);
    textBox->setDimensions(width, height);
    textBox->setCharHeight(1.0);
    textBox->setColour(color);
    textBox->setCaption(text);
    textBox->setFontName("OpenSansRegular");

    panel->addChild(textBox);
}

void TextManager::removeTextBox(const std::string &ID) {
    panel->removeChild(ID);
    Ogre::OverlayManager::getSingleton().destroyOverlayElement(ID);
}

void TextManager::setText(const std::string &ID, const std::string &Text) {
    Ogre::OverlayManager::getSingleton().getOverlayElement(ID)->setCaption(Text);
}

const std::string TextManager::getText(const std::string &ID) const {
    return Ogre::OverlayManager::getSingleton().getOverlayElement(ID)->getCaption();
}

}
