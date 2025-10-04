#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>

class FilePlaceholder : public wxWindow
{
public:
    FilePlaceholder(wxWindow *parent, wxWindowID id,
                    const wxPoint& position = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = 0);
private:
    void DrawDropFileBackground(wxGraphicsContext *gc) const;
    void OnPaint(wxPaintEvent &evt);
};
