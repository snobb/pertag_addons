//      pertag_addons.c
//
//      Copyright 2011 Alex Kozadaev <akozadaev@yahoo.com>
//

/* changing active tags using left/right arrows */
#define SHIFT_LEFTRIGHT

/* remember layout for each tag  (tile, float, monocle). view->cst_view, setlayout->cst_setlayout */
#define LAYOUT_MEM

/* remember mfact values for each tag (MODKEY - h,l) LAYOUT_MEM, setmfact->cst_setmfact */
#define MFACT_MEM

/* Tile, Floating and Monocle respectively (needs to be adjusted as per the order in layouts[] */
#define T 0
#define F 1
#define M 2

/* Layouts sequence numbers from layouts[] for each tag starting from first */
#ifdef  LAYOUT_MEM
static const void *tagLayouts[] = {
          &layouts[T],
          &layouts[T],
          &layouts[T],
          &layouts[M],
          &layouts[M],
          &layouts[T],
          &layouts[T],
          &layouts[T],
          &layouts[T]};
#endif     /* -----  not LAYOUT_MEM  ----- */

#ifdef  MFACT_MEM
static float tagMFact [] = {
          0.55,
          0.55,
          0.55,
          0.55,
          0.55,
          0.55,
          0.55,
          0.55,
          0.55,
          };
#endif     /* -----  not MFACT_MEM  ----- */

/* get tag number from arg->ui */
unsigned int
getactivetag(const unsigned int ui)
{
  int i = 0;
  unsigned int active = i;
  for (i = 0; i < 9; i++) {  /* if there are more that 1 tag selected - we take the very left */
    if (((ui>>i) & 1) == 1) {
      active = i;
    }
  }
  return active;
}

#ifdef  LAYOUT_MEM
/* sets custom layouts for each tag. Change all instances of view functions to cst_view*/
void
cst_view (const Arg *arg)
{
  if((arg->ui & TAGMASK) == selmon->tagset[selmon->seltags])
    return;
  selmon->seltags ^= 1; /* toggle sel tagset */
  if(arg->ui & TAGMASK)
    selmon->tagset[selmon->seltags] = arg->ui & TAGMASK;
  selmon->lt[selmon->sellt] = (void*)(tagLayouts[getactivetag(selmon->tagset[selmon->seltags])]);
#ifdef  MFACT_MEM
  selmon->mfact = tagMFact[getactivetag(selmon->tagset[selmon->seltags])];
#endif     /* -----  not MFACT_MEM  ----- */
  focus(NULL);
  arrange(selmon);
}

/* sets layout and saves the states to tagLaytouts. Change all setlayout funtions to cst_setlayout in config */
void
cst_setlayout(const Arg * arg)
{
  setlayout(arg);
  tagLayouts[getactivetag(selmon->tagset[selmon->seltags])] = (Layout*)selmon->lt[selmon->sellt];
}
#endif     /* -----  not LAYOUT_MEM  ----- */

#ifdef  MFACT_MEM
/* change all setmfact functions to cst_setmfact */
void
cst_setmfact(const Arg *arg)
{
  setmfact(arg);
  tagMFact[getactivetag(selmon->tagset[selmon->seltags])] = selmon->mfact;
}
#endif     /* -----  not MFACT_MEM  ----- */


#ifdef SHIFT_LEFTRIGHT
/* shift to the next tag. Set in config for shift left shortcut */
void
cst_shiftleft (const Arg * arg)
{
  unsigned int curui = selmon->tagset[selmon->seltags];
  selmon->seltags ^= 1; /* toggle sel tagset */
  selmon->tagset[selmon->seltags] = (curui <= 1) ? 1 << 8 : curui >> 1;
#ifdef  LAYOUT_MEM
  selmon->lt[selmon->sellt] = (void*)(tagLayouts[getactivetag(selmon->tagset[selmon->seltags])]); /* recall the tags layout */
#endif     /* -----  not LAYOUT_MEM  ----- */
#ifdef  MFACT_MEM
  selmon->mfact = tagMFact[getactivetag(selmon->tagset[selmon->seltags])];
#endif     /* -----  not MFACT_MEM  ----- */
  focus(NULL);
  arrange (selmon);
}

/* shift to the previous tag. Set in config for shift left shortcut */
void
cst_shiftright (const Arg * arg)
{
  unsigned int curui = selmon->tagset[selmon->seltags];
  selmon->seltags ^= 1; /* toggle sel tagset */
  selmon->tagset[selmon->seltags] = (curui >= 1 << 8) ? 1 : curui << 1;
#ifdef  LAYOUT_MEM
  selmon->lt[selmon->sellt] = (void*)(tagLayouts[getactivetag(selmon->tagset[selmon->seltags])]); /* recall the tags layout */
#endif     /* -----  not LAYOUT_MEM  ----- */
#ifdef  MFACT_MEM
  selmon->mfact = tagMFact[getactivetag(selmon->tagset[selmon->seltags])];
#endif     /* -----  not MFACT_MEM  ----- */
  focus(NULL);
  arrange (selmon);
}
#endif		/*  -----  not SHIFT_LEFTRIGHT ----- */
