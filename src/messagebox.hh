#ifndef MESSAGEBOX_HH
#define MESSAGEBOX_HH

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gui.h>
#include <string>

class MessageBox
{
public:
    MessageBox(CL_GUIComponent* parent);

    void show(CL_Rect messageBoxRect, const std::string& title, const std::string& message, const std::string textId = "");
    bool isVisible() const;

    CL_Signal_v0 windowClosedSignal;

private:
    void okButtonClicked();

    CL_Window window;
    CL_PushButton okButton;
    CL_Label text;
};

#endif
