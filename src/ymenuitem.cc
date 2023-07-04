/*
 * IceWM
 *
 * Copyright (C) 1997-2001 Marko Macek
 */
#include "config.h"
#include "ykey.h"
#include "yimage.h"
#include "ymenuitem.h"
#include "ymenu.h"
#include "yaction.h"

#include "yapp.h"
#include "yprefs.h"
#include "yicon.h"

#include <string.h>

extern YFont menuFont;

YMenuItem::YMenuItem(const mstring &name, int aHotCharPos, const mstring &param,
                     YAction action, YMenu *submenu) :
    fName(name), fParam(param), fAction(action),
    fHotCharPos(aHotCharPos), fSubmenu(submenu), fIcon(null),
    fChecked(false), fEnabled(true) {

    if (fName != null && (fHotCharPos == -2 || fHotCharPos == -3)) {
        int i = fName.indexOf('_');
        if (i >= 0) {
            fHotCharPos = i;
            fName = fName.remove(i, 1);
        } else {
            if (fHotCharPos == -3)
                fHotCharPos = 0;
            else
                fHotCharPos = -1;
        }
    }

    if (inrange(fHotCharPos + 1, 0, int(fName.length())) == false)
        fHotCharPos = -1;
}

YMenuItem::YMenuItem(const mstring &name) :
    fName(name), fParam(null), fAction(actionNull), fHotCharPos(-1),
    fSubmenu(nullptr), fIcon(null), fChecked(false), fEnabled(true) {
}

YMenuItem::YMenuItem():
    fName(null), fParam(null), fAction(actionNull), fHotCharPos(-1),
    fSubmenu(nullptr), fIcon(null), fChecked(false), fEnabled(false) {
}

YMenuItem::~YMenuItem() {
    if (fSubmenu && !fSubmenu->isShared())
        delete fSubmenu;
    fSubmenu = nullptr;
}

void YMenuItem::setChecked(bool c) {
    fChecked = c;
}

void YMenuItem::setIcon(ref<YIcon> icon) {
    fIcon = icon;
}

int YMenuItem::queryHeight(int &top, int &bottom, int &pad) {
    int height;
    if (isSeparator()) {
        top = 0;
        bottom = 0;
        pad = 1;
        height = LOOK(lookMetal | lookFlat) ? 3 : 4;
    } else {
        int fontHeight = max(16, int(menuFont ? menuFont->height() : 0) + 1);
        int ih = max(fontHeight, int(YIcon::menuSize()));

        if (LOOK(lookWarp4 | lookWin95)) {
            top = bottom = 0;
            pad = 1;
        } else if (LOOK(lookMetal | lookFlat)) {
            top = bottom = 1;
            pad = 1;
        } else if (LOOK(lookMotif)) {
            top = bottom = 2;
            pad = 0; //1
        } else if (LOOK(lookGtk)) {
            top = bottom = 2;
            pad = 0; //1
        } else {
            top = 1;
            bottom = 2;
            pad = 0; //1;
        }
        height = top + pad + ih + pad + bottom;
    }
    return height;
}

int YMenuItem::getIconWidth() const {
    ref<YIcon> icon = getIcon();
    return icon != null ? YIcon::menuSize(): 0;
}

int YMenuItem::getNameWidth() {
    return fName.nonempty() && menuFont ? menuFont->textWidth(fName) : 0;
}

int YMenuItem::getParamWidth() {
    return fParam.nonempty() && menuFont ? menuFont->textWidth(fParam) : 0;
}

// vim: set sw=4 ts=4 et:
