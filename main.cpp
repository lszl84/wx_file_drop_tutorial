#include <wx/simplebook.h>
#include <wx/wx.h>

#include "bufferedbitmap.h"
#include "fileplaceholder.h"
#include "singlefiledroptarget.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

    void LoadImageFile(const wxString& file);
private:
    FilePlaceholder *placeholder;
    BufferedBitmap *bitmap;

    wxSimplebook *viewSwitcher;
    wxImage image;

    wxButton *zoomInButton;
    wxButton *zoomOutButton;
    
    void OnOpenImage(wxCommandEvent &event);
    void OnZoomIn(wxCommandEvent &event);
    void OnZoomOut(wxCommandEvent &event);

    void UpdateBitmapImage(const wxImage &image);
};

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    MyFrame *frame = new MyFrame("Hello World", wxDefaultPosition, wxDefaultSize);
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    auto sizer = new wxBoxSizer(wxVERTICAL);

    viewSwitcher = new wxSimplebook(this, wxID_ANY,
                                    wxDefaultPosition, FromDIP(wxSize(500, 200)));
    bitmap = new BufferedBitmap(viewSwitcher, wxID_ANY);
    placeholder = new FilePlaceholder(viewSwitcher, wxID_ANY);

    viewSwitcher->ShowNewPage(placeholder);

    auto dropTarget = new SingleFileDropTarget([this](const wxString& file) {
        this->LoadImageFile(file);
    });

    viewSwitcher->SetDropTarget(dropTarget);
    
    auto imageButton = new wxButton(this, wxID_ANY, "Load Image...");
    zoomInButton = new wxButton(this, wxID_ANY, "Zoom In");
    zoomOutButton = new wxButton(this, wxID_ANY, "Zoom Out");

    imageButton->Bind(wxEVT_BUTTON, &MyFrame::OnOpenImage, this);
    zoomInButton->Bind(wxEVT_BUTTON, &MyFrame::OnZoomIn, this);
    zoomOutButton->Bind(wxEVT_BUTTON, &MyFrame::OnZoomOut, this);

    zoomInButton->Disable();
    zoomOutButton->Disable();
   
    auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(imageButton, 0, wxLEFT, FromDIP(5));
    buttonSizer->Add(zoomInButton, 0, wxLEFT, FromDIP(5));
    buttonSizer->Add(zoomOutButton, 0, wxLEFT, FromDIP(5));

    sizer->Add(viewSwitcher, 1, wxEXPAND | wxALL, FromDIP(10));
    sizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, FromDIP(10));

    this->SetSizerAndFit(sizer);
}

void MyFrame::OnOpenImage(wxCommandEvent &event)
{
    wxFileDialog openFileDialog(this, _("Open Image"),
                                "", "",
                                "Image files (*.png;*.jpg;*.jpeg;*.bmp)|*.png;*.jpg;*.jpeg;*.bmp",
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    LoadImageFile(openFileDialog.GetPath());
}

void MyFrame::LoadImageFile(const wxString& file)
{
    if (!image.LoadFile(file)) {
        wxMessageBox("Failed to load image", "Error", wxOK | wxICON_ERROR);
        return;
    }

    UpdateBitmapImage(image);

    viewSwitcher->ShowNewPage(bitmap);
    
    zoomInButton->Enable();
    zoomOutButton->Enable();
}

void MyFrame::UpdateBitmapImage(const wxImage &image)
{
    bitmap->SetBitmap(wxBitmap(image));
    this->Layout();
}

void MyFrame::OnZoomIn(wxCommandEvent &event)
{
    bitmap->ZoomIn();
}

void MyFrame::OnZoomOut(wxCommandEvent &event)
{
    bitmap->ZoomOut();
}
