#include "tablebuttons/tablebuttons_buttons.h"
#include <QDebug>
#include "tablebuttons/tablebuttons_buttongroup.h"
#include <QSet>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QDebug>
#include <QMouseEvent>
#include <QSignalMapper>
#include <QScrollBar>

namespace TableButtons {

Buttons::Buttons(QObject *parent) : QObject(parent), mView(nullptr), mVisible(true)
{

}

Buttons::~Buttons()
{
    clear();
}

int Buttons::nextId()
{
    QSet<int> keys = mButtons.keys().toSet();
    int i = 0;
    while (keys.contains(i)) {
        i++;
    }
    return i;
}

void Buttons::create() {
    foreach(ButtonImpl* button, mButtons) {
        if (!button->button()) {
            QPushButton* widget = button->create(mView);
            connect(widget,&QPushButton::clicked,[=](){
                emit clicked(button->id(), button->currentIndex());
            });
        }
    }
}

int verticalButtonsX(int viewportWidth, int shift, QHeaderView* horizontalHeader, int buttonsWidth) {
    int lastColumn = horizontalHeader->count() - 1;
    int x0 = horizontalHeader->sectionViewportPosition(lastColumn) + horizontalHeader->sectionSize(lastColumn) + shift;
    if (x0 + buttonsWidth > viewportWidth + shift) {
        x0 = viewportWidth + shift - buttonsWidth;
    }
    return x0;
}

int horizontalButtonsY(int viewportHeight, int shift, QHeaderView* verticalHeader, int buttonsHeight) {
    int lastColumn = verticalHeader->count() - 1;
    int y0 = verticalHeader->sectionViewportPosition(lastColumn) + verticalHeader->sectionSize(lastColumn) + shift;
    if (y0 + buttonsHeight > viewportHeight + shift) {
        y0 = viewportHeight + shift - buttonsHeight;
    }
    return y0;
}

int Buttons::fixedButtonIndex(QHeaderView* header, ButtonImpl* button) {
    int index = button->index();
    if (button->type() == ButtonImpl::ButtonTypePrepend) {
        index = 0;
    } else if (button->type() == ButtonImpl::ButtonTypeAppend) {
        index = header->count();
    }
    return index;
}

void Buttons::clear()
{
    QList<ButtonImpl*> values = mButtons.values();
    qDeleteAll(values);
    mButtons.clear();
}

Button Buttons::button(int id)
{
    if (id < 0) {
        id = nextId();
    }

    if (!mButtons.contains(id)) {
        mButtons[id] = new ButtonImpl(id);
    }

    return Button(mButtons[id]);
}

void Buttons::setVisible(bool visible)
{
    mVisible = visible;
    update();
}

bool Buttons::visible() const
{
    return mVisible;
}

void Buttons::updateVertical() {

    ButtonGroup buttons(mButtons.values(), Qt::Vertical);
    if (buttons.isEmpty()) {
        return;
    }
    QPoint shift_ = mView->viewport()->mapToParent(QPoint(0,0));
    QHeaderView* header = mView->verticalHeader();
    int x0 = verticalButtonsX(mView->viewport()->width(), shift_.x(), mView->horizontalHeader(), buttons.width());
    foreach(ButtonImpl* button, buttons) {
        QPushButton* button_ = button->button();
        if (!mVisible) {
            button_->hide();
            continue;
        }
        int buttonSize = button_->height();
        int shift = shift_.y();
        int x = x0 + button->offset().x();
        int y = 0;
        if (button->type() == ButtonImpl::ButtonTypeVariable) {
            int index;
            y = posBorderOrCenterShifted(button->position(), header, mPoint.y(), shift, buttonSize, &index);
            button->setCurrentIndex(index);
        } else if (button->type() == ButtonImpl::ButtonTypeFixed ||
                   button->type() == ButtonImpl::ButtonTypePrepend ||
                   button->type() == ButtonImpl::ButtonTypeAppend) {
            int index = fixedButtonIndex(header, button);
            y = posBorderOrCenterFixedShifted(button->position(), header, index, shift, buttonSize);
            button->setCurrentIndex(index);
        }
        y += button->offset().y();
        button_->move(x,y);
        button_->show();
    }
}

int Buttons::posBorderOrCenterShifted(ButtonImpl::ButtonPosition position, QHeaderView* header,
                                           int pos, int shift, int buttonSize, int* index) {
    return position == ButtonImpl::ButtonPositionInside ?
                posCenterShifted(header, pos, shift, buttonSize, index) :
                posBorderShifted(header, pos, shift, buttonSize, index);
}

int Buttons::posCenterShifted(QHeaderView* header, int pos, int shift, int buttonSize, int* index) {
    return posCenter(header,pos,index) + shift - buttonSize / 2 - 1;
}

int Buttons::posBorderShifted(QHeaderView* header, int pos, int shift, int buttonSize, int* index) {
    return posBorder(header,pos,index) + shift - buttonSize / 2 - 1;
}

int Buttons::indexAt(QHeaderView* header, int pos) {
    int index = header->visualIndexAt(pos);
    if (index < 0) {
        index = header->count() - 1;
    }
    return index;
}

int Buttons::posBorderOrCenterFixedShifted(ButtonImpl::ButtonPosition position, QHeaderView* header, int index, int shift, int buttonSize) {
    return position == ButtonImpl::ButtonPositionInside ? posCenterFixedShifted(header, index, shift, buttonSize) : posBorderFixedShifted(header, index, shift, buttonSize);
}

int Buttons::posCenterFixedShifted(QHeaderView* header, int index, int shift, int buttonSize) {
    return posCenterFixed(header,index) + shift - buttonSize / 2 - 1;
}

int Buttons::posBorderFixedShifted(QHeaderView* header, int index, int shift, int buttonSize) {
    return posBorderFixed(header,index) + shift - buttonSize / 2 - 1;
}

int Buttons::posCenterFixed(QHeaderView* header, int index) {
    int sectionPos = header->sectionViewportPosition(index) + header->sectionSize(index) / 2;
    return sectionPos;
}

int Buttons::posBorderFixed(QHeaderView* header, int index) {
    if (index >= header->count()) {
        return header->sectionViewportPosition(index-1) + header->sectionSize(index-1);
    }
    return header->sectionViewportPosition(index);
}

int Buttons::posCenter(QHeaderView* header, int pos, int* index_) {
    int index = indexAt(header, pos);
    *index_ = index;
    int sectionPos = header->sectionViewportPosition(index) + header->sectionSize(index) / 2;
    return sectionPos;
}

int Buttons::posBorder(QHeaderView* header, int pos, int* index_) {
    int index = indexAt(header, pos);
    int topPos = header->sectionViewportPosition(index);
    int bottomPos = header->sectionViewportPosition(index) + header->sectionSize(index);
    if (pos - topPos < bottomPos - pos) {
        *index_ = index;
        return topPos;
    } else {
        *index_ = index + 1;
        return bottomPos;
    }
}

void Buttons::updateHorizontal() {

    ButtonGroup buttons(mButtons.values(), Qt::Horizontal);
    if (buttons.isEmpty()) {
        return;
    }

    QPoint shift_ = mView->viewport()->mapToParent(QPoint(0,0));
    QHeaderView* header = mView->horizontalHeader();
    int y0 = horizontalButtonsY(mView->viewport()->height(), shift_.y(), mView->verticalHeader(), buttons.height());
    foreach(ButtonImpl* button, buttons) {
        QPushButton* button_ = button->button();
        if (!mVisible) {
            button_->hide();
            continue;
        }
        int buttonSize = button_->width();
        int shift = shift_.x();
        int y = y0 + button->offset().y();
        int x = 0;
        if (button->type() == ButtonImpl::ButtonTypeVariable) {
            int index;
            x = posBorderOrCenterShifted(button->position(), header, mPoint.x(), shift, buttonSize, &index);
            button->setCurrentIndex(index);
        } else if (button->type() == ButtonImpl::ButtonTypeFixed ||
                   button->type() == ButtonImpl::ButtonTypePrepend ||
                   button->type() == ButtonImpl::ButtonTypeAppend) {
            int index = fixedButtonIndex(header, button);
            x = posBorderOrCenterFixedShifted(button->position(), header, index, shift, buttonSize);
            button->setCurrentIndex(index);
        }
        x += button->offset().x();
        button_->move(x,y);
        button_->show();
    }

}

void Buttons::update()
{
    create();
    updateVertical();
    updateHorizontal();
}

void Buttons::setView(QTableView *table)
{

    //mButton->setParent(table);
    //mButton->show();


    mView = table;
    /*foreach(b,mButtons) {
        b.button()->setParent(table);
    }*/

    //qDebug() << "scrollbars" << mView->horizontalScrollBar() << mView->verticalScrollBar();

    connect(mView->horizontalScrollBar(),SIGNAL(sliderMoved(int)),this,SLOT(onSliderMoved(int)));
    connect(mView->verticalScrollBar(),SIGNAL(sliderMoved(int)),this,SLOT(onSliderMoved(int)));

    table->setMouseTracking(true);
    table->viewport()->installEventFilter(this);
    connect(table->horizontalHeader(),SIGNAL(sectionResized(int,int,int)),this,SLOT(update()));
    connect(table->verticalHeader(),SIGNAL(sectionResized(int,int,int)),this,SLOT(update()));
    update();
}

bool Buttons::eventFilter(QObject* object, QEvent* event)
{

    if (event->type() == QEvent::MouseMove && object == mView->viewport())
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (!mouseEvent)
            return false;
        mPoint = mouseEvent->pos();

        update();
    } else if (event->type() == QEvent::Resize && object == mView->viewport()) {
        update();
    }
    return false;
}


void Buttons::onSliderMoved(int) {
    //qDebug() << "onSliderMoved";

    QPoint pos = mView->viewport()->mapFromGlobal(QCursor::pos());

    //qDebug() << "point (scrollbar)" << pos;

    mPoint = pos;

    update();
}

}
