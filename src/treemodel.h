#ifndef BOOKON_TREEMODEL_H
#define BOOKON_TREEMODEL_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/dataview.h>

/*
Implement this data model
            Description               Action               Path
--------------------------------------------------------------------------
1: My folder
    2:  frequent
        3:  Google              Web                  http://wwww.google.ch
        3:  my folder           Open                 /home/data/my_folder
    5:  Less frequent
        6:  list directory      Clipboard            ls -al
        7:  my image path       Clipboard            /home/data/bin_dir/my_img.jpg
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief A node inside the model
////////////////////////////////////////////////////////////////////////////////////////////////////
class TreeModelNode;
WX_DEFINE_ARRAY_PTR(TreeModelNode*, TreeModelNodePtrArray);

class TreeModelNode {
 public:
  TreeModelNode(TreeModelNode* parent, const wxString& description, const wxString& path) {
    m_parent = parent;
    m_description = description;
    m_path = path;
    m_container = false;
  }

  TreeModelNode(TreeModelNode* parent, const wxString& branch) {
    m_parent = parent;
    m_description = branch;
    m_container = true;
  }

  ~TreeModelNode() {
    // free all our children nodes
    size_t count = m_children.GetCount();
    for (size_t i = 0; i < count; i++) {
      TreeModelNode* child = m_children[i];
      delete child;
    }
  }

  bool IsContainer() const {
    return m_container;
  }

  TreeModelNode* GetParent() {
    return m_parent;
  }
  TreeModelNodePtrArray& GetChildren() {
    return m_children;
  }
  TreeModelNode* GetNthChild(unsigned int n) {
    return m_children.Item(n);
  }
  void Insert(TreeModelNode* child, unsigned int n) {
    m_children.Insert(child, n);
  }
  void Append(TreeModelNode* child) {
    m_children.Add(child);
  }
  unsigned int GetChildCount() const {
    return m_children.GetCount();
  }

 public:  // public to avoid getters/setters
  wxString m_description;
  wxString m_path;

  // TODO/FIXME:
  // the GTK version of wxDVC (in particular wxDataViewCtrlInternal::ItemAdded)
  // needs to know in advance if a node is or _will be_ a container.
  // Thus implementing:
  //   bool IsContainer() const
  //    { return m_children.GetCount()>0; }
  // doesn't work with wxGTK when MyMusicTreeModel::AddToClassical is called
  // AND the classical node was removed (a new node temporary without children
  // would be added to the control)
  bool m_container;

 private:
  TreeModelNode* m_parent;
  TreeModelNodePtrArray m_children;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief The model
////////////////////////////////////////////////////////////////////////////////////////////////////
class TreeModel : public wxDataViewModel {
 public:
  TreeModel();
  ~TreeModel();

  //
  //  // override sorting to always sort branches ascendingly
  //
  //  int Compare( const wxDataViewItem &item1, const wxDataViewItem &item2,
  //              unsigned int column, bool ascending ) const wxOVERRIDE;


  virtual unsigned int GetColumnCount() const wxOVERRIDE;

  virtual wxString GetColumnType(unsigned int col) const wxOVERRIDE;

  virtual void GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;
  virtual bool SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col) wxOVERRIDE;

  virtual bool IsEnabled(const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;

  virtual wxDataViewItem GetParent(const wxDataViewItem& item) const wxOVERRIDE;
  virtual bool IsContainer(const wxDataViewItem& item) const wxOVERRIDE;
  virtual unsigned int GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const wxOVERRIDE;

 private:
  TreeModelNode* m_root;

  // pointers to some "special" nodes of the tree:
  TreeModelNode* m_pop;
  TreeModelNode* m_classical;
  TreeModelNode* m_ninth;
};

#endif  // BOOKON_TREEMODEL_H
