#ifndef TABLEBUTTONS_BUTTON_H
#define TABLEBUTTONS_BUTTON_H
#include <QString>
class QPushButton;
class QPoint;
class QSize;
class QIcon;
class QTableView;

namespace TableButtons {

class ButtonImpl;

class Button {
public:
    Button(ButtonImpl* button = 0);
    Button& fixed(int index);
    Button& variable();
    Button& inside();
    Button& between();
    Button& insert();
    Button& remove();
    Button& append();
    Button& prepend();
    Button& horizontal();
    Button& vertical();
    Button& text(const QString& text);
    Button& icon(const QIcon& icon);
    Button& size(int w, int h);
    Button& size(const QSize& size);
    Button& iconSize(int w, int h);
    Button& iconSize(const QSize& size);
    Button& offset(int x, int y);
    Button& offset(const QPoint& offset);
    QPushButton* button() const;
    int id() const;
    bool valid() const;
    bool fixed() const;

protected:
    ButtonImpl* mButton;
};

}
#endif // TABLEBUTTONS_BUTTON_H
