/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const unsigned int statuspad = 20;        /* horizontal padding for right status text */
static const char *fonts[]          = { "monospace:size=10", "Symbols Nerd Font:pixelsize=30" /* icon fallback; adjust pixelsize for proportion */ };
static const char dmenufont[]       = "monospace:size=10";
static unsigned int baralpha        = 0x80;
static unsigned int borderalpha     = OPAQUE;
static char normbgcolor[]           = "#000000";
static char normbordercolor[]       = "#2D084C";
static char normfgcolor[]           = "#CBB7E6";
static char selfgcolor[]            = "#DC4ADF";
static char selbgcolor[]            = "#000000";
static char selbordercolor[]        = "#E040F3";
static char termcol0[]              = "#030009";
static char termcol1[]              = "#FF3B5C";
static char termcol2[]              = "#2DFF8F";
static char termcol3[]              = "#FFB020";
static char termcol4[]              = "#631086";
static char termcol5[]              = "#DC4ADF";
static char termcol6[]              = "#2DE2FF";
static char termcol7[]              = "#CBB7E6";
static char termcol8[]              = "#2D084C";
static char termcol9[]              = "#FF3B5C";
static char termcol10[]             = "#2DFF8F";
static char termcol11[]             = "#FFB020";
static char termcol12[]             = "#8B6BB2";
static char termcol13[]             = "#E040F3";
static char termcol14[]             = "#2DE2FF";
static char termcol15[]             = "#F3E9FF";
static char *termcolor[]            = {
	termcol0, termcol1, termcol2, termcol3,
	termcol4, termcol5, termcol6, termcol7,
	termcol8, termcol9, termcol10, termcol11,
	termcol12, termcol13, termcol14, termcol15,
};
static char *colors[][3]            = {
	/*               fg           bg           border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	{ NULL,       "spterm",   NULL,       SPTAG(0),     1,           -1 },
	{ NULL,       "spyazi",   NULL,       SPTAG(1),     1,           -1 },
	{ NULL,       "keepassxc", NULL,      SPTAG(2),     1,           -1 },
	{ NULL,       "nextcloud", NULL,      SPTAG(3),     1,           -1 },
};

/* window following */
#define WFACTIVE '>'
#define WFINACTIVE 'v'
#define WFDEFAULT WFACTIVE

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static int attachbelow = 1;          /* 1 means attach after the currently active window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#define PERTAG_PATCH 1

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },                    /* [0] default: tile */
	{ "[M]",      monocle },                 /* [1] monocle */
	{ "[@]",      spiral },                  /* [2] spiral */
	{ "[\\]",     dwindle },                 /* [3] dwindle */
	{ "H[]",      deck },                    /* [4] deck */
	{ "TTT",      bstack },                  /* [5] bstack */
	{ "===",      bstackhoriz },             /* [6] bstackhoriz */
	{ "HHH",      grid },                    /* [7] grid */
	{ "###",      nrowgrid },                /* [8] nrowgrid */
	{ "---",      horizgrid },               /* [9] horizgrid */
	{ ":::",      gaplessgrid },             /* [10] gaplessgrid */
	{ "|M|",      centeredmaster },          /* [11] centeredmaster */
	{ ">M>",      centeredfloatingmaster },  /* [12] centeredfloatingmaster */
	{ "><>",      NULL },                    /* [13] floating */
	{ NULL,       NULL },                    /* [14] sentinel */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	&((Keychord){1, {{MODKEY, KEY}},                       view,           {.ui = 1 << TAG} }), \
	&((Keychord){1, {{MODKEY|ControlMask, KEY}},           toggleview,     {.ui = 1 << TAG} }), \
	&((Keychord){1, {{MODKEY|ShiftMask, KEY}},             tag,            {.ui = 1 << TAG} }), \
	&((Keychord){1, {{MODKEY|ControlMask|ShiftMask, KEY}}, toggletag,      {.ui = 1 << TAG} }),

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "rofi", "-show", "drun", NULL  };
static const char *termcmd[]  = { "kitty", NULL };
typedef struct {
	const char *name;
	const void *cmd;
	int width_pct;
	int height_pct;
} Sp;
static const char *spcmd1[] = { "kitty", "--name", "spterm", NULL };
static const char *spcmd2[] = { "kitty", "--name", "spyazi", "yazi", NULL };
static const char *spcmd3[] = { "keepassxc", NULL };
static const char *spcmd4[] = { "nextcloud", NULL };
static Sp scratchpads[] = {
	/* name        cmd      width height (% of monitor, 0 keeps app default) */
	{ "spterm",    spcmd1,  80,   0 },
	{ "spyazi",    spcmd2,  80,   0 },
	{ "keepassxc", spcmd3,  0,    0 },
	{ "nextcloud", spcmd4,  70,   70 },
};

static Keychord *keychords[] = {
	/* keys                           function        argument */
	&((Keychord){1, {{MODKEY, XK_p}},                            spawn,          {.v = dmenucmd } }),
	&((Keychord){1, {{MODKEY, XK_Return}},                       spawn,          {.v = termcmd } }),
	&((Keychord){1, {{MODKEY, XK_b}},                            togglebar,      {0} }),
	&((Keychord){1, {{MODKEY, XK_n}},                            togglefollow,   {0} }),
	&((Keychord){1, {{MODKEY, XK_Down}},                         focusstack,     {.i = +1 } }),
	&((Keychord){1, {{MODKEY, XK_Up}},                           focusstack,     {.i = -1 } }),
	&((Keychord){1, {{MODKEY, XK_h}},                            focusdir,       {.i = 0 } }),  // left
	&((Keychord){1, {{MODKEY, XK_l}},                            focusdir,       {.i = 1 } }),  // right
	&((Keychord){1, {{MODKEY, XK_k}},                            focusdir,       {.i = 2 } }),  // up
	&((Keychord){1, {{MODKEY, XK_j}},                            focusdir,       {.i = 3 } }),  // down
	&((Keychord){1, {{MODKEY, XK_i}},                            incnmaster,     {.i = +1 } }),
	&((Keychord){1, {{MODKEY, XK_d}},                            incnmaster,     {.i = -1 } }),
	&((Keychord){1, {{MODKEY|Mod1Mask, XK_h}},                   setmfact,       {.f = -0.05} }),
	&((Keychord){1, {{MODKEY|Mod1Mask, XK_l}},                   setmfact,       {.f = +0.05} }),
	&((Keychord){1, {{MODKEY|ShiftMask, XK_h}},                  setcfact,       {.f = +0.25} }),
	&((Keychord){1, {{MODKEY|ShiftMask, XK_l}},                  setcfact,       {.f = -0.25} }),
	&((Keychord){1, {{MODKEY|ShiftMask, XK_o}},                  setcfact,       {.f =  0.00} }),
	&((Keychord){1, {{MODKEY|ShiftMask, XK_Return}},             zoom,           {0} }),
	&((Keychord){1, {{MODKEY|Mod1Mask, XK_u}},                   incrgaps,       {.i = +1 } }),
	&((Keychord){1, {{MODKEY|Mod1Mask|ShiftMask, XK_u}},         incrgaps,       {.i = -1 } }),
	&((Keychord){1, {{MODKEY|Mod1Mask, XK_i}},                   incrigaps,      {.i = +1 } }),
	&((Keychord){1, {{MODKEY|Mod1Mask|ShiftMask, XK_i}},         incrigaps,      {.i = -1 } }),
	&((Keychord){1, {{MODKEY|Mod1Mask, XK_o}},                   incrogaps,      {.i = +1 } }),
	&((Keychord){1, {{MODKEY|Mod1Mask|ShiftMask, XK_o}},         incrogaps,      {.i = -1 } }),
	&((Keychord){1, {{MODKEY|Mod1Mask, XK_6}},                   incrihgaps,     {.i = +1 } }),
	&((Keychord){1, {{MODKEY|Mod1Mask|ShiftMask, XK_6}},         incrihgaps,     {.i = -1 } }),
	&((Keychord){1, {{MODKEY|Mod1Mask, XK_7}},                   incrivgaps,     {.i = +1 } }),
	&((Keychord){1, {{MODKEY|Mod1Mask|ShiftMask, XK_7}},         incrivgaps,     {.i = -1 } }),
	&((Keychord){1, {{MODKEY|Mod1Mask, XK_8}},                   incrohgaps,     {.i = +1 } }),
	&((Keychord){1, {{MODKEY|Mod1Mask|ShiftMask, XK_8}},         incrohgaps,     {.i = -1 } }),
	&((Keychord){1, {{MODKEY|Mod1Mask, XK_9}},                   incrovgaps,     {.i = +1 } }),
	&((Keychord){1, {{MODKEY|Mod1Mask|ShiftMask, XK_9}},         incrovgaps,     {.i = -1 } }),
	&((Keychord){1, {{MODKEY|Mod1Mask, XK_0}},                   togglegaps,     {0} }),
	&((Keychord){1, {{MODKEY|Mod1Mask|ShiftMask, XK_0}},         defaultgaps,    {0} }),
	&((Keychord){1, {{MODKEY, XK_Tab}},                          view,           {0} }),
	&((Keychord){1, {{MODKEY|ShiftMask, XK_c}},                  killclient,     {0} }),
	&((Keychord){1, {{MODKEY, XK_t}},                            setlayout,      {.v = &layouts[0]} }),
	&((Keychord){1, {{MODKEY, XK_f}},                            setlayout,      {.v = &layouts[13]} }),
	&((Keychord){1, {{MODKEY, XK_m}},                            setlayout,      {.v = &layouts[1]} }),
	&((Keychord){1, {{MODKEY, XK_c}},                            setlayout,      {.v = &layouts[11]} }),
	&((Keychord){1, {{MODKEY, XK_space}},                        setlayout,      {0} }),
	&((Keychord){1, {{MODKEY|ShiftMask, XK_space}},              togglefloating, {0} }),
	&((Keychord){1, {{MODKEY|ShiftMask, XK_a}},                  toggleAttachBelow, {0} }),
	&((Keychord){1, {{MODKEY, XK_equal}},                        view,           {.ui = ~0 } }),
	&((Keychord){1, {{MODKEY|ShiftMask, XK_equal}},              tag,            {.ui = ~0 } }),
	&((Keychord){1, {{MODKEY, XK_comma}},                        focusmon,       {.i = -1 } }),
	&((Keychord){1, {{MODKEY, XK_period}},                       focusmon,       {.i = +1 } }),
	&((Keychord){1, {{MODKEY|ShiftMask, XK_comma}},              tagmon,         {.i = -1 } }),
	&((Keychord){1, {{MODKEY|ShiftMask, XK_period}},             tagmon,         {.i = +1 } }),
	&((Keychord){1, {{MODKEY, XK_y}},                            togglescratch,  {.ui = 0 } }),
	&((Keychord){1, {{MODKEY, XK_u}},                            togglescratch,  {.ui = 1 } }),
	&((Keychord){1, {{MODKEY, XK_x}},                            togglescratch,  {.ui = 2 } }),
	&((Keychord){1, {{MODKEY, XK_s}},                            togglescratch,  {.ui = 3 } }),
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	TAGKEYS(                        XK_0,                      9)
	&((Keychord){1, {{MODKEY, XK_F5}},                           xrdb,           {.v = NULL} }),
	&((Keychord){1, {{MODKEY|ShiftMask, XK_q}},                 quit,           {0} }),
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[1]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
