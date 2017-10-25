#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>

struct Object {
    std::string name;
    std::vector<Object*> child;
    virtual void clear() = 0;
    virtual ~Object() {}
};

struct MyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = 0);
    ~MyButton();
    void (*Handle)(Object *);
    Object* O;
    void SetHandler(void (*handle)(Object *));
    void SetObj(Object*);
public Q_SLOTS:
    void handleButtonClicked();
};

struct Application : public Object {
    Application();
    void clear() override;
    virtual ~Application();
    QApplication* app;
};

struct Widget : public Object {
    Widget();
    void clear() override;
    virtual ~Widget();
    QWidget* widget;
};

struct Layout : public Object {
    Layout();
    void clear() override;
    virtual ~Layout();
    QVBoxLayout* layout;
};

struct Label : public Widget {
    Label();
    void clear() override;
    virtual ~Label();
};

struct PushButton : public Widget {
    PushButton();
    void clear() override;
    virtual ~PushButton();
};

Application* Application_New();
Widget* Widget_New(Widget*);
Layout* VBoxLayout_New(Widget*);
Label* Label_New(Widget*);
PushButton* PushButton_New(Widget*);

const char* Object_GetClassName(Object* obj);
void Widget_SetLayout(Widget*, Layout*);
int Application_Exec(Application* app);
void Label_SetText(Label*, const char*);
void Layout_AddWidget(Layout*, Widget*);
void PushButton_SetText(PushButton*, const char*);
void Widget_SetWindowTitle(Widget*, const char*);
void Widget_SetSize(Widget*, int, int);
void Widget_SetVisible(Widget*, bool);
void Object_Delete(Object*);
void PushButton_SetOnClicked(PushButton*, void (*handle_button_click)(Object*));
#endif // MAINWINDOW_H
