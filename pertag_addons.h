/* @author Alex Kozadaev <akozadaev at yahoo com> */

/* changing active tags using left/right arrows
 * pta_shiftleft, pta_shiftright */
#define SHIFT_LEFTRIGHT

/* size of monitor buffer (a record per monitor) */
#define MON  3
#define TAGS 9

/* Tile, Floating and Monocle respectively (needs to be
 * adjusted as per the order in layouts[] */
enum {
    T = 0,
    F,
    M,
};

#define ACTIVE_TAG()        getactivetag(selmon->tagset[selmon->seltags])
#define GET_LAYOUT(tag)     (void*)tagLayouts[selmon->num % MON][(tag) % TAGS].layout
#define SET_LAYOUT(tag, l)  tagLayouts[selmon->num % MON][(tag) % TAGS].layout = l
#define GET_MFACT(tag)      tagLayouts[selmon->num % MON][(tag) % TAGS].mfact
#define SET_MFACT(tag, m)   tagLayouts[selmon->num % MON][(tag) % TAGS].mfact = m
#define GET_BAR(tag)        tagLayouts[selmon->num % MON][(tag) % TAGS].showbar
#define SET_BAR(tag, v)     tagLayouts[selmon->num % MON][(tag) % TAGS].showbar = v

/* Layouts/mfact values per tag per monitor */
struct TagInfo {
    const void *layout;
    float mfact;
    Bool showbar;
};

static struct TagInfo tagLayouts[MON][TAGS] = {
    {
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[M], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
    }, {
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[M], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
    }, {
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[M], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
        { &layouts[T], 0.55, 1 },
    },
};

/* get tag number from arg->ui */
static unsigned int
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

/* Sets custom layouts for each tag.
 * Change all instances of view function to pta_view in config.h */
static void
pta_view(const Arg *arg)
{
    if((arg->ui & TAGMASK) == selmon->tagset[selmon->seltags])
        return;
    selmon->seltags ^= 1; /* toggle sel tagset */
    if(arg->ui & TAGMASK) {
        selmon->tagset[selmon->seltags] = arg->ui & TAGMASK;
    }
    selmon->lt[selmon->sellt] = GET_LAYOUT(ACTIVE_TAG());
    selmon->mfact = GET_MFACT(ACTIVE_TAG());

    if (selmon->showbar != GET_BAR(ACTIVE_TAG())) {
        togglebar(NULL);
    }
    focus(NULL);
    arrange(selmon);
}

/* Sets layout and saves the states to tagLaytouts.
 * Change all setlayout funtion to pta_setlayout in config */
static void
pta_setlayout(const Arg * arg)
{
    setlayout(arg);
    SET_LAYOUT(ACTIVE_TAG(), (Layout*)selmon->lt[selmon->sellt]);
}

/* change all setmfact functions to pta_setmfact
 * Change all instances of setmfact function to pta_setmfact in config.h */
static void
pta_setmfact(const Arg *arg)
{
    setmfact(arg);
    SET_MFACT(ACTIVE_TAG(), selmon->mfact);
}

/* Keep state of the bar per tag.
 * Change all instances of togglebar function to pta_togglebar in config.h */
static void
pta_togglebar(const Arg *arg)
{
    togglebar(arg);
    SET_BAR(ACTIVE_TAG(), selmon->showbar);
}

#ifdef SHIFT_LEFTRIGHT
/* shift to the next tag. Set in config for shift left shortcut */
static void
pta_shiftleft(const Arg * arg)
{
    unsigned int cur_ui = selmon->tagset[selmon->seltags];
    selmon->seltags ^= 1; /* toggle sel tagset */
    selmon->tagset[selmon->seltags] = (cur_ui <= 1) ? 1 << 8 : cur_ui >> 1;
    selmon->lt[selmon->sellt] = GET_LAYOUT(ACTIVE_TAG());
    selmon->mfact = GET_MFACT(ACTIVE_TAG());
    focus(NULL);
    arrange (selmon);
}

/* shift to the previous tag. Set in config for shift left shortcut */
static void
pta_shiftright(const Arg * arg)
{
    unsigned int cur_ui = selmon->tagset[selmon->seltags];
    selmon->seltags ^= 1; /* toggle sel tagset */
    selmon->tagset[selmon->seltags] = (cur_ui >= 1 << 8) ? 1 : cur_ui << 1;
    selmon->lt[selmon->sellt] = GET_LAYOUT(ACTIVE_TAG());
    selmon->mfact = GET_MFACT(ACTIVE_TAG());
    focus(NULL);
    arrange (selmon);
}
#endif  /*  -----  not SHIFT_LEFTRIGHT ----- */

/* exit with custom error code function for dwmguard */
static inline void exit_code(const Arg *args)
{
    exit(args->i);
}
