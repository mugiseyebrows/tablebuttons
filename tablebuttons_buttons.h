#ifndef TABLEBUTTONS_BUTTONS_H
#define TABLEBUTTONS_BUTTONS_H

#include <QObject>

#include "tablebuttons/tablebuttons_button.h"
#include "tablebuttons/tablebuttons_buttonimpl.h"
#include <QMap>
#include <QPoint>
class QTableView;
class QHeaderView;
class QSignalMapper;

namespace TableButtons {

class Buttons : public QObject
{
    Q_OBJECT
public:
    explicit Buttons(QObject *parent = nullptr);
    ~Buttons();

    void setView(QTableView* view);
    void clear();
    Button button(int id);
    void setVisible(bool visible);
    bool visible() const;

protected:

    int nextId();
    virtual void updateVertical();
    virtual void updateHorizontal();
    void create();
    bool eventFilter(QObject *object, QEvent *event);
    int posCenter(QHeaderView *header, int pos, int *index_);
    int posBorder(QHeaderView *header, int pos, int *index);
    int indexAt(QHeaderView *header, int pos);
    int posBorderShifted(QHeaderView *header, int pos, int shift, int buttonSize, int* index);
    int posCenterShifted(QHeaderView *header, int pos, int shift, int buttonSize, int *index);
    int posCenterFixed(QHeaderView *header, int index);
    int posBorderFixed(QHeaderView *header, int index);
    int posCenterFixedShifted(QHeaderView *header, int index, int shift, int buttonSize);
    int posBorderFixedShifted(QHeaderView *header, int index, int shift, int buttonSize);
    int posBorderOrCenterShifted(ButtonImpl::ButtonPosition position, QHeaderView *header, int pos, int shift, int buttonSize, int* index);
    int posBorderOrCenterFixedShifted(ButtonImpl::ButtonPosition position, QHeaderView *header, int index, int shift, int buttonSize);
    int fixedButtonIndex(QHeaderView *header, ButtonImpl *button);

    QMap<int,ButtonImpl*> mButtons;
    QTableView* mView;
    QPushButton* mButton;
    QPoint mPoint;
    bool mVisible;

signals:
    void clicked(int,int);
protected slots:
    void onSliderMoved(int);

public slots:
    virtual void update();
};

}

#endif // TABLEBUTTONS_BUTTONS_H
