#include "bufferedbitmap.h"

BufferedBitmap::BufferedBitmap(wxWindow *parent, wxWindowID id,
                               const wxPoint &pos,
                               const wxSize &size,
                               long style)
    : wxScrolled<wxWindow>(parent, id, pos, size,
                           wxFULL_REPAINT_ON_RESIZE |
                           wxVSCROLL | wxHSCROLL | style)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT); // needed for windows

    this->Bind(wxEVT_PAINT, &BufferedBitmap::OnPaint, this);
    
    SetScrollRate(FromDIP(10), FromDIP(10));
    SetupVirtualSize();
}

void BufferedBitmap::OnPaint(wxPaintEvent &evt)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    DoPrepareDC(dc);

    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

    if (gc) {
        if (bitmap) {
            // scaling consistent with wxStaticBitmap
            const wxSize drawSize = ToDIP(GetVirtualSize());

            const wxSize bmpSize = GetScaledBitmapSize();

            double w = bmpSize.GetWidth();
            double h = bmpSize.GetHeight();

            double x = (drawSize.GetWidth() - w) / 2;
            double y = (drawSize.GetHeight() - h) / 2;

            gc->DrawBitmap(bitmap.value(),
                           gc->FromDIP(x), gc->FromDIP(y),
                           gc->FromDIP(w), gc->FromDIP(h));
        }
        
        delete gc;
    }
}

void BufferedBitmap::SetBitmap(const wxBitmap &bitmap)
{
    this->bitmap = bitmap;


    SetupVirtualSize();

    this->Refresh();
}

std::optional<wxBitmap> BufferedBitmap::GetBitmap() const
{
    return bitmap;
}

double BufferedBitmap::GetZoomMultiplier() const
{
    return pow(ZOOM_FACTOR, zoomLevel);
}

double BufferedBitmap::GetZoomPercentage() const
{
    return GetZoomMultiplier() * 100;
}

void BufferedBitmap::ZoomIn()
{
    auto centerPos = CalcUnscrolledPosition(wxPoint(GetClientSize().GetWidth() / 2,
                                                    GetClientSize().GetHeight() / 2));

    zoomLevel++;

    CenterAfterZoom(centerPos, centerPos * ZOOM_FACTOR);
    SetupVirtualSize();

    this->Refresh();
}

void BufferedBitmap::ZoomOut()
{
    auto centerPos = CalcUnscrolledPosition(wxPoint(GetClientSize().GetWidth() / 2,
                                                    GetClientSize().GetHeight() / 2));

    zoomLevel--;

    CenterAfterZoom(centerPos, centerPos * (1.0 / ZOOM_FACTOR));
    SetupVirtualSize();

    this->Refresh();
}

void BufferedBitmap::SetupVirtualSize() {
    SetVirtualSize(bitmap
                   ? FromDIP(GetScaledBitmapSize())
                   : GetSize());
}                            
                   
wxSize BufferedBitmap::GetScaledBitmapSize() const
{
    const wxSize bmpSize = bitmap->GetSize();
    const double zoom = GetZoomMultiplier();
    return wxSize(bmpSize.GetWidth() * zoom, bmpSize.GetHeight() * zoom);
}

void BufferedBitmap::CenterAfterZoom(wxPoint previousCenter, wxPoint currentCenter)
{
    wxPoint pixelsPerUnit;
    GetScrollPixelsPerUnit(&pixelsPerUnit.x, &pixelsPerUnit.y);

    auto delta = currentCenter - previousCenter;

    auto destX = GetViewStart().x + delta.x / pixelsPerUnit.x;
    auto destY = GetViewStart().y + delta.y / pixelsPerUnit.y;

    Scroll(destX, destY);
}
