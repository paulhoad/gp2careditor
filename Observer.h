
#ifndef _INCLUDED_OBSERVER_H
#define _INCLUDED_OBSERVER_H
class Object3D;
class CCarTree;

enum t_Observer
{
  t_Point,
  t_Vertex,
  t_Polygon,
  t_Camera,
  t_DataVal,
  t_TextureCmd,
  t_Scale,
  t_JamFile
};
enum t_ParentType
{
  t_Car,
  t_Helmet
};

class Observer : public CObject
{
 public:
  t_Observer type;
  t_ParentType parent;
  Observer(t_Observer type) : type(type)
  {
    changed = FALSE;
    object = NULL;
    type = type;
  }

  virtual ~Observer() {}

  virtual void
  Open(CCarTree *tree);

  virtual void
  Select(CCarTree *tree);

  Object3D *
  getObject()
  {
    return object;
  }

  void
  setChanged()
  {
    changed = TRUE;
  }

  BOOL
  isChanged()
  {
    return changed;
  }

  Object3D *object;
  CDocument *pDoc;

  CDocument *
  getDocument()
  {
    return pDoc;
  }

  BOOL changed;
};

#endif
