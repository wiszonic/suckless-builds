/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

/* fonts */
static const char *fonts[]          = { "UbuntuNerdFont:size=10" };
static const char dmenufont[]       = "UbuntuNerdFont:size=10";

/* colors */
static const char col_bg1[]       = "#222226";
static const char col_bg2[]       = "#444444";
static const char col_bg3[]       = "#4b3f66";
static const char col_bg4[]       = "#a86f46";

static const char col_tl1[]       = "#8c5dd9";
static const char col_tl2[]       = "#f6c175";
static const char col_br1[]       = "#3a3150";
static const char col_br2[]       = "#875534";

static const char col_fg1[]       = "#eeeeee";

/* schemes */
static const char *colors[][3]      = {
	/* name        fg	  bg       border  */
	[SchemeNorm] = { col_fg1, col_bg1, col_bg3 },
	[SchemeItem] = { col_fg1, col_bg3, col_bg1 },
	[SchemeSel]  = { col_fg1, col_bg4, col_bg4 },

	[SchemeTL1]  = { col_bg1, col_tl1, col_bg1 },
	[SchemeTL2]  = { col_bg1, col_tl2, col_bg1 },
	[SchemeBR1]  = { col_bg1, col_br1, col_bg1 },
	[SchemeBR2]  = { col_bg1, col_br2, col_bg1 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

/* window rules */
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layouts */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "T",      tile },    /* first entry is default */
	{ "F",      NULL },    /* no layout function means floating behavior */
	{ "M",      monocle },
};
static const char *indicator = { "Mode" };

/* custom symbols for nr. of clients in monocle layout */
/* when clients >= LENGTH(monocles), uses the last element */
static const char *monocles[] = { "M" };

static const char *notitle = { "Desktop" };

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, NULL };
static const char *termcmd[]  = { "st", NULL };

static const char *incvolcmd[] = { "/usr/bin/wpctl", "set-volume", "@DEFAULT_AUDIO_SINK@", "2%+", NULL };
static const char *decvolcmd[] = { "/usr/bin/wpctl", "set-volume", "@DEFAULT_AUDIO_SINK@", "2%-", NULL };
static const char *mutvolcmd[] 	= { "wpctl", "set-mute", "@DEFAULT_AUDIO_SINK@", "toggle", NULL };

static const char *incmiccmd[] = { "/usr/bin/wpctl", "set-volume", "@DEFAULT_AUDIO_SOURCE@", "2%+", NULL };
static const char *decmiccmd[] = { "/usr/bin/wpctl", "set-volume", "@DEFAULT_AUDIO_SOURCE@", "2%-", NULL };
static const char *mutmiccmd[] 	= { "wpctl", "set-mute", "@DEFAULT_AUDIO_SOURCE@", "toggle", NULL };

/* keybinds */
static const Key keys[] = {
	/* modifier                     key        			function        	argument */

	/* general */
	{ MODKEY,                       XK_p,      			spawn,          	{.v = dmenucmd } },
	{ MODKEY,             		XK_Return, 			spawn,          	{.v = termcmd } },
	{ MODKEY,                       XK_b,      			togglebar,      	{0} },
	{ MODKEY,                       XK_j,      			focusstack,     	{.i = +1 } },
	{ MODKEY,                       XK_k,      			focusstack,     	{.i = -1 } },
	{ MODKEY,                       XK_i,      			incnmaster,     	{.i = +1 } },
	{ MODKEY,                       XK_d,      			incnmaster,     	{.i = -1 } },
	{ MODKEY,                       XK_h,      			setmfact,       	{.f = -0.05} },
	{ MODKEY,                       XK_l,      			setmfact,       	{.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, 			zoom,           	{0} },
	{ MODKEY,                       XK_Tab,    			view,           	{0} },
	{ MODKEY,             		XK_q,      			killclient,     	{0} },
	{ MODKEY,                       XK_t,      			setlayout,      	{.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      			setlayout,      	{.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      			setlayout,      	{.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  			setlayout,      	{0} },
	{ MODKEY|ShiftMask,             XK_space,  			togglefloating, 	{0} },
	{ MODKEY,                       XK_0,      			view,           	{.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      			tag,            	{.ui = ~0 } },
	{ MODKEY,                       XK_comma,  			focusmon,       	{.i = -1 } },
	{ MODKEY,                       XK_period, 			focusmon,       	{.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  			tagmon,         	{.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, 			tagmon,         	{.i = +1 } },
	{ MODKEY|ShiftMask,             XK_q,      			quit,           	{0} },
	TAGKEYS(                        XK_1,      			                	0)
	TAGKEYS(                        XK_2,      			                	1)
	TAGKEYS(                        XK_3,      			                	2)
	TAGKEYS(                        XK_4,      			                	3)
	TAGKEYS(                        XK_5,      			                	4)
	TAGKEYS(                        XK_6,      			                	5)
	TAGKEYS(                        XK_7,      			                	6)
	TAGKEYS(                        XK_8,      			                	7)
	TAGKEYS(                        XK_9,      			                	8)

	/* commands */
	{ 0,				XF86XK_AudioLowerVolume,	spawn,			{.v = decvolcmd} },	
	{ 0,				XF86XK_AudioRaiseVolume,	spawn,			{.v = incvolcmd} },
	{ 0,				XF86XK_AudioMute,		spawn,			{.v = mutmiccmd} },

	{ MODKEY,			XF86XK_AudioLowerVolume,	spawn,			{.v = decmiccmd} },	
	{ MODKEY,			XF86XK_AudioRaiseVolume,	spawn,			{.v = incmiccmd} },
	{ MODKEY,			XF86XK_AudioMute,		spawn,			{.v = mutvolcmd} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
//	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
//	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
//	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
//	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
//	{ ClkTagBar,            0,              Button1,        view,           {0} },
//	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
//	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
//	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

