#ifndef _TEXTMANAGER_HPP_
#define _TEXTMANAGER_HPP_

#include <cstdio>
#include <cstdarg>
#include <OgreSingleton.h>
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayManager.h>

namespace Cycleshooter {

// upstream: http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Simple+Text+Output
class TextManager: public Ogre::Singleton<TextManager> {

    Ogre::OverlayManager* overlayMgr;
    Ogre::Overlay* overlay;
    Ogre::OverlayContainer* panel;

public:

    TextManager();

    void addTextBox(const std::string& ID,
                    const std::string& text,
                    Ogre::Real x, Ogre::Real y,
                    Ogre::Real width, Ogre::Real height,
                    const Ogre::ColourValue& color = Ogre::ColourValue(1.0, 1.0, 1.0, 1.0));

    void removeTextBox(const std::string& ID);

    void setText(const std::string& ID, const std::string& Text);

    const std::string getText(const std::string& ID) const;

    void printf(const std::string& ID,  const char *fmt, /* args */ ...) {
        char text[256];
        va_list ap;

        if (fmt == NULL) {
            *text = 0;
        }
        else {
            va_start(ap, fmt);
            vsprintf(text, fmt, ap);
            va_end(ap);
        }

        overlayMgr->getOverlayElement(ID)->setCaption(text);
    }

};

}

#endif
