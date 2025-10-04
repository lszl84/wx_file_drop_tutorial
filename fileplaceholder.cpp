#include "fileplaceholder.h"

#include <wx/dcbuffer.h>

FilePlaceholder::FilePlaceholder(wxWindow *parent, wxWindowID id,
                                 const wxPoint& position,
                                 const wxSize& size,
                                 long style)
    : wxWindow(parent, id, position, size, wxFULL_REPAINT_ON_RESIZE | style)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    Bind(wxEVT_PAINT, &FilePlaceholder::OnPaint, this);
}

void FilePlaceholder::DrawDropFileBackground(wxGraphicsContext *gc) const
{
    const auto size = GetClientSize();
    const auto margin = FromDIP(5);
    const auto color = wxColour(128, 128, 128);

    const auto penInfo =
        wxGraphicsPenInfo(color)
        .Width(1.0)
        .Style(wxPENSTYLE_SHORT_DASH);

    const auto dashedPen = gc->CreatePen(penInfo);

    gc->SetPen(dashedPen);
    gc->SetBrush(*wxTRANSPARENT_BRUSH);

    gc->DrawRoundedRectangle(margin, margin,
                             size.GetWidth() - 2 * margin,
                             size.GetHeight() - 2 * margin,
                             FromDIP(10));

    const wxFont font(wxFontInfo(24).Family(wxFONTFAMILY_SWISS).Bold());
    gc->SetFont(font, color);

    const wxString text = "Drop your image here";

    double textWidth, textHeight, descent, leading;
    gc->GetTextExtent(text, &textWidth, &textHeight, &descent, &leading);

    gc->DrawText(text,
                 (size.GetWidth() - textWidth) / 2.0,
                 (size.GetHeight() - textHeight) / 2.0);
}

void FilePlaceholder::OnPaint(wxPaintEvent &evt)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

    if(gc) {
        DrawDropFileBackground(gc);
        delete gc;
    }
}
