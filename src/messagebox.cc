#include "messagebox.hh"

MessageBox::MessageBox(CL_GUIComponent* parent): window(parent, CL_GUITopLevelDescription()), okButton(&window), text(&window)
{
    window.set_visible(false, false);
    okButton.func_clicked().set(this, &MessageBox::okButtonClicked);
    okButton.set_text("Ok");
}

void MessageBox::show(CL_Rect messageBoxRect, const std::string& title, const std::string& message, const std::string textId)
{
    window.set_geometry(messageBoxRect);
    window.set_title(title);

    const int windowWidth = window.get_width();
    const int windowHeight = window.get_height();

    const int buttonWidth = windowWidth / 4;
    const int buttonHeight = 25;
    const int buttonLeft = (windowWidth / 2) - (buttonWidth / 2);
    const int margin = 10;

    CL_Rect buttonRect(buttonLeft, windowHeight - buttonHeight - margin, buttonLeft + buttonWidth, windowHeight - margin);
    okButton.set_geometry(buttonRect);

    CL_Rect clientArea = window.get_client_area();
    text.set_id_name(textId);
    text.set_geometry(CL_Rect(clientArea.left, clientArea.top, clientArea.right, clientArea.bottom - buttonHeight - margin));
    text.set_text(message);

    window.set_visible(true, false);
}

bool MessageBox::isVisible() const
{
    return window.is_visible();
}

void MessageBox::okButtonClicked()
{
    window.set_visible(false, false);
    windowClosedSignal.invoke();
}
