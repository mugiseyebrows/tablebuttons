#include "tablebuttons/tablebuttons_buttonimpl.h"

namespace TableButtons {

ButtonImpl::ButtonImpl(int id)
    : mId(id), mButton(nullptr), mOrientation(Qt::Vertical), mType(ButtonTypeVariable),
      mPosition(ButtonPositionInside), mIndex(-1) {
}

ButtonImpl::~ButtonImpl()
{
    if (mButton) {
        mButton->setParent(0);
        mButton->deleteLater();
        mButton = 0;
    }
}

void ButtonImpl::fixed(int index) {
    mType = ButtonTypeFixed;
    mIndex = index;
}

void ButtonImpl::variable() {
    mType = ButtonTypeVariable;
}

void ButtonImpl::inside() {
    mPosition = ButtonPositionInside;
}

void ButtonImpl::between() {
    mPosition = ButtonPositionBetween;
}

void ButtonImpl::insert() {
    mType = ButtonTypeVariable;
    mPosition = ButtonPositionBetween;
}

void ButtonImpl::remove() {
    mType = ButtonTypeVariable;
    mPosition = ButtonPositionInside;
}

void ButtonImpl::append() {
    mType = ButtonTypeAppend;
    mPosition = ButtonPositionBetween;
}

void ButtonImpl::prepend() {
    mType = ButtonTypePrepend;
    mPosition = ButtonPositionBetween;
}

void ButtonImpl::horizontal() {
    mOrientation = Qt::Horizontal;
}

void ButtonImpl::vertical() {
    mOrientation = Qt::Vertical;
}

void ButtonImpl::text(const QString& text) {
    mText = text;
    update(Text);
}

void ButtonImpl::icon(const QIcon& icon) {
    mIcon = icon;update(Icon);
}

void ButtonImpl::size(int w, int h) {
    mSize = QSize(w, h);update(Size);
}

void ButtonImpl::size(const QSize& size) {
    mSize = size;update(Size);
}

void ButtonImpl::iconSize(int w, int h) {
    mIconSize = QSize(w, h);update(IconSize);
}

void ButtonImpl::iconSize(const QSize& size) {
    mIconSize = size;
update(IconSize);
}

void ButtonImpl::offset(int x, int y) {
    mOffset = QPoint(x, y);
}

void ButtonImpl::offset(const QPoint& offset) {
    mOffset = offset;
}

QPushButton* ButtonImpl::button() const {
    return mButton;
}

int ButtonImpl::id() const {
    return mId;
}

QPoint ButtonImpl::offset() const {
    return mOffset;
}

QSize ButtonImpl::size() const {
    return mSize;
}

QSize ButtonImpl::iconSize() const {
    return mIconSize;
}

Qt::Orientation ButtonImpl::orientation() const {
    return mOrientation;
}

QIcon ButtonImpl::icon() const {
    return mIcon;
}

int ButtonImpl::index() const {
    return mIndex;
}

ButtonImpl::ButtonType ButtonImpl::type() const {
    return mType;
}

ButtonImpl::ButtonPosition ButtonImpl::position() const {
    return mPosition;
}

QString ButtonImpl::text() const {
    return mText;
}

int ButtonImpl::top() const {
    return mOffset.y();
}

int ButtonImpl::left() const {
    return mOffset.x();
}

int ButtonImpl::bottom() const {
    QSize size = mSize.isEmpty() ? mButton->size() : mSize;
    return size.height() + top();
}

int ButtonImpl::right() const {
    QSize size = mSize.isEmpty() ? mButton->size() : mSize;
    return size.width() + left();
}

int ButtonImpl::currentIndex() const {
    return mCurrentIndex;
}

void ButtonImpl::setCurrentIndex(int currentIndex) {
    mCurrentIndex = currentIndex;
}

QPushButton* ButtonImpl::create(QTableView* view) {
    if (mButton) {
        return mButton;
    }
    mButton = new QPushButton(mText, view);
    update(All);
    return mButton;
}

void ButtonImpl::update(int updateProp)
{
    if (!mButton) {
        return;
    }
    if (!mSize.isEmpty() && updateProp & Size) {
        mButton->setFixedSize(mSize);
    }
    if (!mIcon.isNull()) {
        if (updateProp && updateProp & Icon) {
            mButton->setIcon(mIcon);
        }
        if (!mIconSize.isNull() && updateProp & IconSize) {
            mButton->setIconSize(mIconSize);
        }
    }
    if (!mText.isEmpty() && updateProp & Text) {
        mButton->setText(mText);
    }
}

}
