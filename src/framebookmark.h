#ifndef BOOKON_FRAMEBOOKMARK_H
#define BOOKON_FRAMEBOOKMARK_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "bookmark.h"

///////////////////////////////////////////////////////////////////////////////
/// Class FrameBookmark
///////////////////////////////////////////////////////////////////////////////
class FrameBookmark : public wxDialog {
 public:
  FrameBookmark(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Bookmark"),
                const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

  virtual bool TransferDataToWindow();
  virtual bool TransferDataFromWindow();

 private:
  void _create_controls();

  wxTextCtrl* m_description_ctrl;
  wxTextCtrl* m_path_ctrl;
  wxButton* m_btn_paste_path;
  wxButton* m_btn_paste_text;
  wxRadioButton* m_opt_open_ctrl;
  wxRadioButton* m_opt_copy_ctrl;
  wxRadioButton* m_opt_web_ctrl;

  BookMark m_bookmark;

 public:
  const BookMark& GetBookmark() const;
  void SetBookmark(const BookMark& mBookmark);
};

#endif  // BOOKON_FRAMEBOOKMARK_H
