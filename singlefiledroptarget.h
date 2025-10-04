#pragma once

#include <wx/dnd.h>
#include <functional>

class SingleFileDropTarget : public wxFileDropTarget
{
public:
    using FileDropCallback = std::function<void(const wxString&)>;

    FileDropCallback callback;

    SingleFileDropTarget(const FileDropCallback& c) : callback(c) {}

private:
    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) override
    {
        if (!filenames.IsEmpty() && callback) {
            callback(filenames[0]);
            return true;
        }
        return false;
    }
};
