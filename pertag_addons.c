/* pertag_addons.c */
/* Copyright 2011-2013 Alex Kozadaev <akozadaev@yahoo.com> */

/* changing active tags using left/right arrows
 * cst_shiftleft, cst_shiftright */
#define SHIFT_LEFTRIGHT

/* remember layout for each tag  (tile, float, monocle)
 * view->cst_view, setlayout->cst_setlayout */
#define LAYOUT_MEM

/* remember mfact values for each tag (MODKEY - h,l) LAYOUT_MEM,
 * setmfact->cst_setmfact */
#define MFACT_MEM

/* size of monitor buffer (a record per monitor) */
#define MON  2
#define TAGS 9

/* Tile, Floating and Monocle respectively (needs to be
 * adjusted as per the order in layouts[] */
#define T 0
#define F 1
#define M 2

#define ACTIVE_TAG()        getactivetag(selmon->tagset[selmon->seltags])
#define GET_LAYOUT(tag)     (void*)tagLayouts[selmon->num % MON][(tag) % TAGS].layout
#define SET_LAYOUT(tag, l)  tagLayouts[selmon->num % MON][(tag) % TAGS].layout = l
#define GET_MFACT(tag)      tagLayouts[selmon->num % MON][(tag) % TAGS].mfact
#define SET_MFACT(tag, m)   tagLayouts[selmon->num % MON][(tag) % TAGS].mfact = m

#if defined(LAYOUT_MEM) || defined(MFACT_MEM)
/* Layouts/mfact values per tag per monitor */
struct TagInfo {
    const void *layout;
    float mfact;
};

static struct TagInfo tagLayouts[MON][TAGS] = {
    {
        { &layouts[T], 0.55 },
        { &layouts[T], 0.55 },
        { &layouts[T], 0.55 },
        { &layouts[M], 0.55 },
        { &layouts[M], 0.55 },
        { &layouts[T], 0.55 },
        { &layouts[T], 0.55 },
        { &layouts[T], 0.55 },
        { &layouts[T], 0.55 },
    }, {
        { &layouts[T], 0.55 },
        { &layouts[T], 0.55 },
        { &layouts[T], 0.55 },
        { &layouts[M], 0.55 },
        { &layouts[T], 0.55 },
        { &layouts[T], 0.55 },
        { &layouts[T], 0.55 },
        { &layouts[T], 0.55 },
        { &layouts[T], 0.55 },
    },
};

/* get tag number from arg->ui */
unsigned int
getactivetag(const unsigned int ui)
{
    int i = 0;
    unsigned int active = i;
    /* if there are more that 1 tag selected - we take the very left */
    for (i = 0; i < TAGS; i++) {
        if (((ui>>i) & 1) == 1) {
            active = i;
        }
    }
    return active;
}

#endif /* if defined(LAYOUT_MEM) || defined(MFACT_MEM) */

#ifdef  LAYOUT_MEM
/* sets custom layouts for each tag. Change all instances of view functions
 * to cst_view */
void
cst_view (const Arg *arg)
{
    if((arg->ui & TAGMASK) == selmon->tagset[selmon->seltags])
        return;
    selmon->seltags ^= 1; /* toggle sel tagset */
    if(arg->ui & TAGMASK)
        selmon->tagset[selmon->seltags] = arg->ui & TAGMASK;
    selmon->lt[selmon->sellt] = GET_LAYOUT(ACTIVE_TAG());
#ifdef  MFACT_MEM
    selmon->mfact = GET_MFACT(ACTIVE_TAG());
#endif     /* -----  not MFACT_MEM  ----- */
    focus(NULL);
    arrange(selmon);
}

/* sets layout and saves the states to tagLaytouts. Change all setlayout
 * funtions to cst_setlayout in config */
void
cst_setlayout(const Arg * arg)
{
    setlayout(arg);
    SET_LAYOUT(ACTIVE_TAG(),
               (Layout*)selmon->lt[selmon->sellt]);
}
#endif     /* -----  not LAYOUT_MEM  ----- */

#ifdef  MFACT_MEM
/* change all setmfact functions to cst_setmfact */
void
cst_setmfact(const Arg *arg)
{
    setmfact(arg);
    SET_MFACT(ACTIVE_TAG(), selmon->mfact);
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
    selmon->lt[selmon->sellt] = GET_LAYOUT(ACTIVE_TAG());
#endif     /* -----  not LAYOUT_MEM  ----- */
#ifdef  MFACT_MEM
    selmon->mfact = GET_MFACT(ACTIVE_TAG());
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
    selmon->lt[selmon->sellt] = GET_LAYOUT(ACTIVE_TAG());
#endif     /* -----  not LAYOUT_MEM  ----- */
#ifdef  MFACT_MEM
    selmon->mfact = GET_MFACT(ACTIVE_TAG());
#endif     /* -----  not MFACT_MEM  ----- */
    focus(NULL);
    arrange (selmon);
}
#endif  /*  -----  not SHIFT_LEFTRIGHT ----- */
