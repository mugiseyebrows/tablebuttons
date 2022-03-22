#ifndef TABLEBUTTONS_BUTTONGROUP_H
#define TABLEBUTTONS_BUTTONGROUP_H

#include "tablebuttons/tablebuttons_buttonimpl.h"

namespace TableButtons {

class ButtonGroup : public QList<ButtonImpl*>
{
public:
    ButtonGroup(const QList<ButtonImpl*>& buttons, Qt::Orientation orientation);

    int left() const;
    int right() const;
    int top() const;
    int bottom() const;

    int width() const;
    int height() const;
};

}

#endif // TABLEBUTTONS_BUTTONGROUP_H
