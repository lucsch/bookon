#include "treemodel.h"

TreeModel::TreeModel() {
  m_root = new TreeModelNode(NULL, "Bookmarks");
  TreeModelNode *my_node = new TreeModelNode(m_root, "TestDesc", "TestPath");
  m_root->Append(my_node);
  TreeModelNode * my_node2 = new TreeModelNode(m_root, "Frequent");
  m_root->Append(my_node2);
  TreeModelNode * my_node3 = new TreeModelNode(my_node2, "TestDesc2", "TestPath2");
  my_node2->Append(my_node3);
}

TreeModel::~TreeModel() {
  delete m_root;
}

unsigned int TreeModel::GetColumnCount() const {
  return 3;
}

wxString TreeModel::GetColumnType(unsigned int col) const {
  if (col == 1) {
    return "bitmap";
  }

  return "string";
}

void TreeModel::GetValue(wxVariant &variant, const wxDataViewItem &item, unsigned int col) const {
  wxASSERT(item.IsOk());

  TreeModelNode *node = (TreeModelNode *)item.GetID();
  switch (col) {
    case 0:
      variant = node->m_description;
      break;
    case 1:
      // variant = node->m_artist;
      break;
    case 2:
      variant = node->m_path;
      break;

    default:
      wxLogError("TreeModel::GetValue: wrong column %d", col);
  }
}

bool TreeModel::SetValue(const wxVariant &variant, const wxDataViewItem &item, unsigned int col) {
  wxASSERT(item.IsOk());

  TreeModelNode *node = (TreeModelNode *)item.GetID();
  switch (col) {
    case 0:
      node->m_description = variant.GetString();
      return true;
    case 1:
      // node->m_artist = variant.GetString();
      return true;
    case 2:
      node->m_path = variant.GetString();
      return true;

    default:
      wxLogError("TreeModel::SetValue: wrong column");
  }
  return false;
}

bool TreeModel::IsEnabled(const wxDataViewItem &item, unsigned int col) const {
  wxASSERT(item.IsOk());

  TreeModelNode *node = (TreeModelNode *)item.GetID();

  // return !(col == 3 && node->m_artist.EndsWith("Beethoven"));
  return false;
}

wxDataViewItem TreeModel::GetParent(const wxDataViewItem &item) const {
  // the invisible root node has no parent
  if (!item.IsOk()) return wxDataViewItem(0);

  TreeModelNode *node = (TreeModelNode *)item.GetID();

  // "MyMusic" also has no parent
  if (node == m_root) return wxDataViewItem(0);

  return wxDataViewItem((void *)node->GetParent());
}

bool TreeModel::IsContainer(const wxDataViewItem &item) const {
  // the invisble root node can have children
  // (in our model always "MyMusic")
  if (!item.IsOk()) return true;

  TreeModelNode *node = (TreeModelNode *)item.GetID();
  return node->IsContainer();
}

unsigned int TreeModel::GetChildren(const wxDataViewItem &parent, wxDataViewItemArray &array) const {
  TreeModelNode *node = (TreeModelNode *)parent.GetID();
  if (!node) {
    array.Add(wxDataViewItem((void *)m_root));
    return 1;
  }

  if (node->GetChildCount() == 0) {
    return 0;
  }

  unsigned int count = node->GetChildren().GetCount();
  for (unsigned int pos = 0; pos < count; pos++) {
    TreeModelNode *child = node->GetChildren().Item(pos);
    array.Add(wxDataViewItem((void *)child));
  }
  return count;
}
