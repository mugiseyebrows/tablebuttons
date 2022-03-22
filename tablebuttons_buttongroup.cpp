#include "tablebuttons/tablebuttons_buttongroup.h"

namespace TableButtons {

namespace  {

QList<ButtonImpl*> filterOrientation(const QList<ButtonImpl*>& buttons, Qt::Orientation orientation) {
    QList<ButtonImpl*> res;
    foreach(ButtonImpl* button, buttons) {
        if (button->orientation() == orientation) {
            res.append(button);
        }
    }
    return res;
}

int minLeft(const QList<ButtonImpl*>& buttons) {
    std::vector<int> lefts;
    std::transform(buttons.begin(),buttons.end(),std::back_inserter(lefts),
                   [](ButtonImpl* button){return button->left();});
    return *std::min_element(lefts.begin(),lefts.end());
}

int maxRight(const QList<ButtonImpl*>& buttons) {
    std::vector<int> rights;
    std::transform(buttons.begin(),buttons.end(),std::back_inserter(rights),
                   [](ButtonImpl* button){return button->right();});
    return *std::max_element(rights.begin(),rights.end());
}

int maxBottom(const QList<ButtonImpl*>& buttons) {
    std::vector<int> bottoms;
    std::transform(buttons.begin(),buttons.end(),std::back_inserter(bottoms),
                   [](ButtonImpl* button){return button->bottom();});
    return *std::max_element(bottoms.begin(),bottoms.end());
}

int minTop(const QList<ButtonImpl*>& buttons) {
    std::vector<int> tops;
    std::transform(buttons.begin(),buttons.end(),std::back_inserter(tops),
                   [](ButtonImpl* button){return button->top();});
    return *std::min_element(tops.begin(),tops.end());
}


}

ButtonGroup::ButtonGroup(const QList<ButtonImpl *> &buttons, Qt::Orientation orientation)
{
    QList<ButtonImpl *> filtered = filterOrientation(buttons, orientation);
    append(filtered);
}

int ButtonGroup::left() const
{
    return minLeft(*this);
}

int ButtonGroup::right() const
{
    return maxRight(*this);
}

int ButtonGroup::width() const {
    return right() - left();
}

int ButtonGroup::height() const {
    return bottom() - top();
}

int ButtonGroup::top() const
{
    return minTop(*this);
}

int ButtonGroup::bottom() const
{
    return maxBottom(*this);
}

}
