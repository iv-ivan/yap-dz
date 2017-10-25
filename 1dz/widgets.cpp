#include "widgets.h"

MyButton::MyButton(QWidget *parent) :
    QPushButton(parent)
{
}

MyButton::~MyButton()
{
}

void MyButton::SetHandler(void (*handle)(Object *)) {
    Handle = handle;
}
void MyButton::SetObj(Object* o) {
    O = o;
}
void MyButton::handleButtonClicked()
{
    Handle(O);
}

Application::Application() {name = "Application";}
void Application::clear() {delete app;}
Application::~Application() {}
Widget::Widget() {name = "Widget";}
void Widget::clear() {delete widget;}
Widget::~Widget() {}
Layout::Layout() {name = "VBoxLayout";}
void Layout::clear() {delete layout;}
Layout::~Layout() {}
Label::Label() {name = "Label";}
void Label::clear() {}
Label::~Label() {}
PushButton::PushButton() {name = "PushButton";}
void PushButton::clear() {}
PushButton::~PushButton() {}

Application* Application_New() {
    Application* app = new Application;
    int args = 0;
    char ** empty_args;
    app->app = new QApplication(args, empty_args);
    return app;
}

Widget* Widget_New(Widget*) {
    Widget* w = new Widget;
    w->widget = new QWidget;
    return w;
}

Layout* VBoxLayout_New(Widget*) {
    Layout* l = new Layout;
    l->layout = new QVBoxLayout;
    return l;
}

Label* Label_New(Widget*) {
    Label* l = new Label;
    l->widget = new QLabel;
    return l;
}

struct PushButton* PushButton_New(Widget*) {
    PushButton* b = new PushButton;
    b->widget= new MyButton;
    return b;
}

const char* Object_GetClassName(Object* obj) {
    return obj->name.c_str();
}
void Widget_SetLayout(Widget* widget, Layout* layout) {
    widget->widget->setLayout(layout->layout);
    widget->child.push_back((Object*)layout->layout);
}
int Application_Exec(Application* app) {
    return app->app->exec();
}
void Label_SetText(Label* label, const char* text) {
    dynamic_cast<QLabel*>(label->widget)->setText(text);
}
void PushButton_SetText(PushButton* button, const char* text) {
    dynamic_cast<QPushButton*>(button->widget)->setText(text);
}
void Layout_AddWidget(Layout* layout, Widget* widget) {
    layout->layout->addWidget(widget->widget);
    layout->child.push_back((Object*)widget->widget);
}
void Widget_SetWindowTitle(Widget* widget, const char* text) {
    widget->widget->setWindowTitle(text);
}
void Widget_SetSize(Widget* widget, int w, int h) {
    widget->widget->resize(w, h);
}
void Widget_SetVisible(Widget* widget, bool flag) {
    widget->widget->setVisible(flag);
}
void Object_Delete(Object* object) {
    for (int i = 0; i < object->child.size(); ++i) {
        object->child[i]->clear();
        delete object->child[i];
    }
    object->clear();
    delete object;
}


void PushButton_SetOnClicked(PushButton* button, void (*handle_button_click)(Object*)) {
    dynamic_cast<MyButton*>(button->widget)->SetHandler(handle_button_click);
    dynamic_cast<MyButton*>(button->widget)->SetObj((Object*)button);
    MyButton::connect(button->widget,
        SIGNAL(clicked(bool)),
        button->widget,
        SLOT(handleButtonClicked())
        );
}
