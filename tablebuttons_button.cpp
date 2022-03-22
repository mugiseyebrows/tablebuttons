#include "tablebuttons/tablebuttons_button.h"
#include "tablebuttons/tablebuttons_buttonimpl.h"
#include <QIcon>
#include <QPoint>
#include <QPushButton>
#include <QSize>
#include <QTableView>

namespace TableButtons {

Button::Button(ButtonImpl* button) : mButton(button) {
}

Button& Button::fixed(int index) {
    mButton->fixed(index);
    return *this;
}

Button& Button::variable() {
    mButton->variable();
    return *this;
}

Button& Button::inside() {
    mButton->inside();
    return *this;
}

Button& Button::between() {
    mButton->between();
    return *this;
}

Button& Button::insert() {
    mButton->insert();
    return *this;
}

Button& Button::remove() {
    mButton->remove();
    return *this;
}

Button& Button::append() {
    mButton->append();
    return *this;
}

Button& Button::prepend() {
    mButton->prepend();
    return *this;
}

Button& Button::horizontal() {
    mButton->horizontal();
    return *this;
}

Button& Button::vertical() {
    mButton->vertical();
    return *this;
}

Button& Button::text(const QString& text) {
    mButton->text(text);
    return *this;
}

Button& Button::icon(const QIcon& icon) {
    mButton->icon(icon);
    return *this;
}

Button& Button::size(int w, int h) {
    mButton->size(w, h);
    return *this;
}

Button& Button::size(const QSize& size) {
    mButton->size(size);
    return *this;
}

Button& Button::iconSize(int w, int h) {
    mButton->iconSize(w, h);
    return *this;
}

Button& Button::iconSize(const QSize& size) {
    mButton->iconSize(size);
    return *this;
}

Button& Button::offset(int x, int y) {
    mButton->offset(x, y);
    return *this;
}

Button& Button::offset(const QPoint& offset) {
    mButton->offset(offset);
    return *this;
}

QPushButton* Button::button() const {
    return mButton->button();
}

int Button::id() const {
    return mButton->id();
}

bool Button::valid() const {
    return mButton != 0;
}

bool Button::fixed() const {
    return mButton->type() == ButtonImpl::ButtonTypeFixed;
}

}
