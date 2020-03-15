
#ifndef _INCLUDED_JAM_DEF_H_
#define _INCLUDED_JAM_DEF_H_

#include "CarTree.h"
#include "CarEditor.h"

#include "sys/stat.h"
#include "bmp.h"
#include "Pallette.h"

typedef struct
{
  unsigned short num_items;
  unsigned short image_total_size;
} JAM_HDR;

typedef struct
{
  unsigned char x;
  unsigned short y;
  // unsigned short    idx_02;
  unsigned char unk;
  unsigned short width;
  unsigned short height;
  short idx_08;
  unsigned short idx_0A;
  unsigned short image_ptr;  // TODO: long ptr?
  unsigned short idx_0E;
  unsigned short palette_size_div4;
  unsigned short jam_id;
  unsigned short idx_14;
  unsigned char idx_16;
  unsigned char idx_17;
  unsigned char idx_18[8];
} JAM_ENTRY;

class JAMEntry : public CObject
{
 public:
  unsigned char *jamData;
  BMP *jamBmp;
  JAM_ENTRY *info;

  JAMEntry()
  {
    info = NULL;
    imageAdjusted = FALSE;
    jamBmp = NULL;
  }

  ~JAMEntry();

  BOOL imageAdjusted;

  unsigned char *
  AdjustImage(int size, unsigned char *imgPtr, BOOL rcr);

  unsigned char *palette1;
  unsigned char *palette2;
  unsigned char *palette3;
  unsigned char *palette4;
  unsigned char *img_ptr;

  void
  LoadTree(CCarTree *tree, HTREEITEM root)
  {
    char buffer[1024];

    wsprintf(buffer, "JamId:%d  (%d %d %d %d)", info->jam_id, info->x, info->y,
             info->width, info->height);
    HTREEITEM JamItem = tree->insertInfoNode(root, buffer, TO_ID(IDB_OFFSET));
    wsprintf(buffer, "X:%d", info->x);
    tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));
    wsprintf(buffer, "Y:%d", info->y);
    tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));
    wsprintf(buffer, "Width:%d", info->width);
    tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));
    wsprintf(buffer, "Height:%d", info->height);
    tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));
    // wsprintf(buffer,"IDX_02:%d",info->idx_02);
    // tree->insertInfoNode(JamItem,buffer,TO_ID(IDB_OFFSET));
    wsprintf(buffer, "IDX_08:%d", info->idx_08);
    tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));
    wsprintf(buffer, "IDX_0A:%d", info->idx_0A);
    tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));
    wsprintf(buffer, "IDX_0E:%d", info->idx_0E);
    tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));
    wsprintf(buffer, "IDX_14:%d", info->idx_14);
    tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));
    wsprintf(buffer, "IDX_16:%d", info->idx_16);
    tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));
    wsprintf(buffer, "IDX_17:%d", info->idx_17);
    tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));
    for (int i = 0; i < 8; i++) {
      wsprintf(buffer, "IDX_18[%d]:%d", i, info->idx_18[i]);
      tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));
    }
    tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));
    wsprintf(buffer, "PaletteSize:%d", info->palette_size_div4);
    tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));
    wsprintf(buffer, "PalettePtr:%d", info->image_ptr);
    tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));
    wsprintf(buffer, "Palette1:%d", info->palette_size_div4);
    HTREEITEM pal1 = tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));

    unsigned char *ptr = palette1;
    for (int i = 0; i < info->palette_size_div4; i++) {
      wsprintf(buffer, "[%d] Val:%d (%d %d %d)", i, (int)*ptr,
               palette[(int)*ptr].r, palette[(int)*ptr].g,
               palette[(int)*ptr].b);
      ptr++;
      tree->insertInfoNode(pal1, buffer, TO_ID(IDB_OFFSET));
    }
    wsprintf(buffer, "Palette2:%d", info->palette_size_div4);
    HTREEITEM pal2 = tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));

    ptr = palette2;
    for (int i = 0; i < info->palette_size_div4; i++) {
      wsprintf(buffer, "[%d] Val:%d", i, (int)*ptr);
      ptr++;
      tree->insertInfoNode(pal2, buffer, TO_ID(IDB_OFFSET));
    }
    wsprintf(buffer, "Palette3:%d", info->palette_size_div4);
    HTREEITEM pal3 = tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));
    ptr = palette3;
    for (int i = 0; i < info->palette_size_div4; i++) {
      wsprintf(buffer, "[%d] Val:%d", i, (int)*ptr);
      ptr++;
      tree->insertInfoNode(pal3, buffer, TO_ID(IDB_OFFSET));
    }
    wsprintf(buffer, "Palette4:%d", info->palette_size_div4);
    HTREEITEM pal4 = tree->insertInfoNode(JamItem, buffer, TO_ID(IDB_OFFSET));

    ptr = palette4;

    for (int i = 0; i < info->palette_size_div4; i++) {
      wsprintf(buffer, "[%d] Val:%d", i, (int)*ptr);
      ptr++;
      tree->insertInfoNode(pal4, buffer, TO_ID(IDB_OFFSET));
    }
  }
};

class JAM : public CObject
{
 public:
  JAM_HDR *header;
  JAMEntry *entry;
  CString filename;
  FILE *fpJam;
  BOOL valid;
  BOOL rcr;
  unsigned char *jammemory;
  unsigned char *jamimagememory;

  JAM(CString filename, BOOL isRCR)
      : filename(filename), entry(NULL), header(NULL), rcr(isRCR)
  {
    valid = FALSE;
    jammemory = NULL;
  }

  ~JAM();

  BOOL
  LoadBMPImage(CBitmap &bitmap, int m_Width, int m_Height);
  BOOL
  LoadSingleBMPImage(CBitmap &bitmap, int m_Width, int m_Height, int index);

  BOOL
  DrawAllJams(CDC *pDC, double currentScale, BOOL drawRcr,
              BOOL drawOutlines = TRUE);
  BOOL
  DrawSingleJams(CDC *pDC, double currentScale, CBitmap &currentJam, BOOL rcr);

  int
  GetPoint(int x, int y);

  void
  OpenToRead(BOOL notifyErrors)
  {
    fpJam = fopen(filename, "rb");
    if (fpJam == NULL) {
      if (notifyErrors) {
        char msg[256];
        wsprintf(msg, "Failed to load JAM File (%s)", filename);
        AfxMessageBox(msg);
      }
      return;
    }
    valid = TRUE;
    Read();
    fclose(fpJam);
  }

  void
  Read()
  {
    if (valid) {
      if (entry != NULL) {
        // jam file has already been read
        return;
      }
      unsigned char *ptr;
      int len = GetFileLength();
      jammemory = (unsigned char *)malloc(len * sizeof(unsigned char));
      memset(jammemory, 0, len);

      fread(jammemory, 1, len, fpJam);
      UnJam(jammemory, len);

      header = (JAM_HDR *)jammemory;
      ptr = jammemory;

      entry = new JAMEntry[header->num_items];
      ptr += sizeof(JAM_HDR);

      for (int i = 0; i < header->num_items; i++) {
        (entry[i].info) = (JAM_ENTRY *)ptr;
        ptr += sizeof(JAM_ENTRY);
      }
      for (int i = 0; i < header->num_items; i++) {
        entry[i].palette1 = ptr;
        // entry[i].palette1 = jammemory+=entry[i].info->image_ptr;
        ptr += entry[i].info->palette_size_div4;
        entry[i].palette2 = ptr;
        ptr += entry[i].info->palette_size_div4;
        entry[i].palette3 = ptr;
        ptr += entry[i].info->palette_size_div4;
        entry[i].palette4 = ptr;
        ptr += entry[i].info->palette_size_div4;
      }
      jamimagememory = ptr;
    }
  }

  int
  getNumberOfImages()
  {
    if (header)
      return header->num_items;
    else
      return 0;
  }

  int
  getMinX()
  {
    int minX = 0;

    for (int i = 0; i < header->num_items; i++) {
      minX = min(minX, entry[i].info->x);
    }
    return minX;
  }

  int
  getMinY()
  {
    int minY = 0;

    for (int i = 0; i < header->num_items; i++) {
      minY = min(minY, entry[i].info->y);
    }
    return minY;
  }

  int
  getMaxX()
  {
    int maxX = 0;

    for (int i = 0; i < header->num_items; i++) {
      maxX = max(maxX, entry[i].info->x + entry[i].info->width);
    }
    return maxX;
  }

  int
  getMaxY()
  {
    int maxY = 0;

    for (int i = 0; i < header->num_items; i++) {
      maxY = max(maxY, entry[i].info->y + entry[i].info->height);
    }
    return maxY;
  }

  int
  getImageWidth(int img)
  {
    return entry[img].info->width;
  }

  int
  getImageHeight(int img)
  {
    return entry[img].info->height;
  }

  int
  getImageX(int img)
  {
    return entry[img].info->x;
  }

  int
  getImageId(int img)
  {
    return entry[img].info->jam_id;
  }

  int
  getImageIDx2(int img)
  {
    // return entry[img].info->idx_02;
    return 0;
  }

  int
  getImageIndex(int id)
  {
    int subimages = getNumberOfImages();
    for (int j = 0; j < subimages; j++) {
      int idx = getImageId(j);
      if (idx == id) return j;
    }
    return -1;
  }

  int
  getImageY(int img)
  {
    return entry[img].info->y;
  }

  int
  getPalleteSize(int img)
  {
    return entry[img].info->palette_size_div4;
  }

  unsigned char *
  getPalletePtr(int img)
  {
    return entry[img].palette1;
  }

  unsigned char *
  getImagePtr(int img);

  void
  UnJam(void *ptr, unsigned int len);

  HTREEITEM
  LoadTree(CCarTree *tree, HTREEITEM root, BOOL loaded)
  {
    char buffer[128];

    if (valid || loaded) {
      if (header != NULL) {
        wsprintf(buffer, "Num Jam Items:%d", header->num_items);
        HTREEITEM items = tree->insertInfoNode(root, buffer, TO_ID(IDB_OFFSET));

        for (int i = 0; i < header->num_items; i++) {
          entry[i].LoadTree(tree, items);
        }

        wsprintf(buffer, "Total Image Size:%d", header->image_total_size);
        tree->insertInfoNode(root, buffer, TO_ID(IDB_OFFSET));
        return items;
      }
      else {
        wsprintf(buffer, "Cannot Find Jam File");
        HTREEITEM item = tree->insertInfoNode(root, buffer, TO_ID(IDB_UNK));
        return item;
      }
    }
    else {
      wsprintf(buffer, "Currently Unloaded");
      HTREEITEM item = tree->insertInfoNode(root, buffer, TO_ID(IDB_UNK));
      return item;
    }
  }

  int
  GetFileLength()
  {
    struct _stat buf;
    int result;

    /* Get data associated with "stat.c": */
    result = _fstat(_fileno(fpJam), &buf);

    /* Check if statistics are valid: */
    return (int)(buf.st_size);
  }
};

#endif
