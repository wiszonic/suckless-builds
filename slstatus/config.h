/* See LICENSE file for copyright and license details. */

/* interval between updates (in ms) */
const unsigned int interval = 1000;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "U";

/* maximum command output length */
#define CMDLEN 4096

/*
 * function            description                     argument (example)
 *
 * battery_perc        battery percentage              battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_remaining   battery remaining HH:MM         battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_state       battery charging state          battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * cat                 read arbitrary file             path
 * cpu_freq            cpu frequency in MHz            NULL
 * cpu_perc            cpu usage in percent            NULL
 * datetime            date and time                   format string (%F %T)
 * disk_free           free disk space in GB           mountpoint path (/)
 * disk_perc           disk usage in percent           mountpoint path (/)
 * disk_total          total disk space in GB          mountpoint path (/)
 * disk_used           used disk space in GB           mountpoint path (/)
 * entropy             available entropy               NULL
 * gid                 GID of current user             NULL
 * hostname            hostname                        NULL
 * ipv4                IPv4 address                    interface name (eth0)
 * ipv6                IPv6 address                    interface name (eth0)
 * kernel_release      `uname -r`                      NULL
 * keyboard_indicators caps/num lock indicators        format string (c?n?)
 *                                                     see keyboard_indicators.c
 * keymap              layout (variant) of current     NULL
 *                     keymap
 * load_avg            load average                    NULL
 * netspeed_rx         receive network speed           interface name (wlan0)
 * netspeed_tx         transfer network speed          interface name (wlan0)
 * num_files           number of files in a directory  path
 *                                                     (/home/foo/Inbox/cur)
 * ram_free            free memory in GB               NULL
 * ram_perc            memory usage in percent         NULL
 * ram_total           total memory size in GB         NULL
 * ram_used            used memory in GB               NULL
 * run_command         custom shell command            command (echo foo)
 * swap_free           free swap in GB                 NULL
 * swap_perc           swap usage in percent           NULL
 * swap_total          total swap size in GB           NULL
 * swap_used           used swap in GB                 NULL
 * temp                temperature in degree celsius   sensor file
 *                                                     (/sys/class/thermal/...)
 *                                                     NULL on OpenBSD
 *                                                     thermal zone on FreeBSD
 *                                                     (tz0, tz1, etc.)
 * uid                 UID of current user             NULL
 * up                  interface is running            interface name (eth0)
 * uptime              system uptime                   NULL
 * username            username of current user        NULL
 * vol_perc            OSS/ALSA volume in percent      mixer file (/dev/mixer)
 *                                                     NULL on OpenBSD/FreeBSD
 * wifi_essid          WiFi ESSID                      interface name (wlan0)
 * wifi_perc           WiFi signal in percent          interface name (wlan0)
 */

/* define commands */
static const char keycmd[] 	= "setxkbmap -print | awk -F'+' '/xkb_symbols/ {print $2}' | tr 'a-z' 'A-Z'";
static const char volcmd[] 	= "wpctl get-volume @DEFAULT_AUDIO_SINK@ | awk '{print int($2 * 100)}'";
static const char volmutcmd[] 	= "wpctl get-volume @DEFAULT_AUDIO_SINK@ | awk '{print substr($3, 2, 1)}'";
static const char miccmd[]	= "wpctl get-volume @DEFAULT_AUDIO_SOURCE@ | awk '{print int($2 * 100)}'";
static const char micmutcmd[] 	= "wpctl get-volume @DEFAULT_AUDIO_SOURCE@ | awk '{print substr($3, 2, 1)}'";

static const struct arg args[] = {

        /* key */
	{ run_command,
		"^c#3a3150^^r0,4,45,1^^r0,5,1,34^^f1^^r0,5,44,1^^r0,6,44,1^^r0,7,1,31^^r1,7,1,30 \
		^^c#8c5dd9^^r2,36,42,1^^r1,37,43,1^^r0,38,44,1 \
		^^f2^^c#4b3f66^^r0,7,42,29 \
		^^f4^^c#8c5dd9^^r0,11,38,1^^r0,12,38,1^^r0,13,1,19^^r1,13,1,18 \
		^^c#3a3150^^r2,30,36,1^^r1,31,37,1 \
		^^f8^^c#eeeeee^^b#4b3f66^%s^f6 \
		^^c#3a3150^^r0,13,1,17 \
		^^c#8c5dd9^^r1,13,1,17^",
		"echo 'Key'", 0, -1 },
        { run_command,  
		"^f2^^c#3a3150^^r0,4,37,1^^r0,5,36,1^^r0,6,35,1 \
		^^c#8c5dd9^^r0,36,36,1^^r0,37,36,1^^r0,38,37,1 \
		^^c#4b3f66^^r0,7,34,29 \
		^^c#8c5dd9^^r0,11,30,1^^r0,12,29,1 \
		^^c#3a3150^^r0,30,30,1^^r0,31,30,1 \
		^^f6^^c#eeeeee^^b#4b3f66^%s^f6 \
		^^c#3a3150^^r0,13,1,17^^r1,12,1,18^^f6 \
		^^c#8c5dd9^^r0,7,1,29^^r1,6,1,30^^r2,5,1,33^^f7^",                                 
		keycmd, 5, -1 },

        /* vol */
        { run_command,
		"^c#3a3150^^r0,4,44,1^^r0,5,1,34^^f1^^r0,5,43,1^^r0,6,43,1^^r0,7,1,31^^r1,7,1,30 \
		^^c#8c5dd9^^r2,36,41,1^^r1,37,42,1^^r0,38,43,1 \
		^^f2^^c#4b3f66^^r0,7,41,29 \
		^^f4^^c#8c5dd9^^r0,11,37,1^^r0,12,37,1^^r0,13,1,19^^r1,13,1,18 \
		^^c#3a3150^^r2,30,35,1^^r1,31,36,1 \
		^^f8^^c#eeeeee^^b#4b3f66^%s^f6 \
		^^c#3a3150^^r0,13,1,17 \
		^^c#8c5dd9^^r1,13,1,17^",
		"echo 'Vol'", 0, -1 },
        { run_command,
		"^f2^^c#3a3150^^r0,4,41,1^^r0,5,41,1^^r0,6,41,1 \
		^^c#8c5dd9^^r0,36,41,1^^r0,37,41,1^^r0,38,41,1 \
		^^c#4b3f66^^r0,7,41,29 \
		^^c#8c5dd9^^r0,11,41,1^^r0,12,41,1 \
		^^c#3a3150^^r0,30,41,1^^r0,31,41,1 \
		^^f6^^c#eeeeee^^b#4b3f66^%s%%^f6 \
		^^c#3a3150^^r0,13,1,17 \
		^^c#8c5dd9^^r1,13,1,17^",
		volcmd, 5, -1 },
	{ run_command,  
		"^f2^^c#3a3150^^r0,4,30,1^^r0,5,29,1^^r0,6,28,1 \
		^^c#8c5dd9^^r0,36,29,1^^r0,37,29,1^^r0,38,30,1 \
		^^c#4b3f66^^r0,7,27,29 \
		^^c#8c5dd9^^r0,11,23,1^^r0,12,22,1 \
		^^c#3a3150^^r0,30,23,1^^r0,31,23,1 \
		^^f6^^c#eeeeee^^b#4b3f66^%s^f6 \
		^^c#3a3150^^r0,13,1,17^^r1,12,1,18^^f6 \
		^^c#8c5dd9^^r0,7,1,29^^r1,6,1,30^^r2,5,1,33^^f7^",
		volmutcmd, 5, -1 },

        /* mic */
        { run_command,
		"^c#3a3150^^r0,4,44,1^^r0,5,1,34^^f1^^r0,5,43,1^^r0,6,43,1^^r0,7,1,31^^r1,7,1,30 \
		^^c#8c5dd9^^r2,36,41,1^^r1,37,42,1^^r0,38,43,1 \
		^^f2^^c#4b3f66^^r0,7,41,29 \
		^^f4^^c#8c5dd9^^r0,11,37,1^^r0,12,37,1^^r0,13,1,19^^r1,13,1,18 \
		^^c#3a3150^^r2,30,35,1^^r1,31,36,1 \
		^^f8^^c#eeeeee^^b#4b3f66^%s^f6 \
		^^c#3a3150^^r0,13,1,17 \
		^^c#8c5dd9^^r1,13,1,17^",
		"echo 'Mic'", 0, -1 },
        { run_command,
		"^f2^^c#3a3150^^r0,4,41,1^^r0,5,41,1^^r0,6,41,1 \
		^^c#8c5dd9^^r0,36,41,1^^r0,37,41,1^^r0,38,41,1 \
		^^c#4b3f66^^r0,7,41,29 \
		^^c#8c5dd9^^r0,11,41,1^^r0,12,41,1 \
		^^c#3a3150^^r0,30,41,1^^r0,31,41,1 \
		^^f6^^c#eeeeee^^b#4b3f66^%s%%^f6 \
		^^c#3a3150^^r0,13,1,17 \
		^^c#8c5dd9^^r1,13,1,17^",
		miccmd, 5, -1 },
	{ run_command,
		"^f2^^c#3a3150^^r0,4,30,1^^r0,5,29,1^^r0,6,28,1 \
		^^c#8c5dd9^^r0,36,29,1^^r0,37,29,1^^r0,38,30,1 \
		^^c#4b3f66^^r0,7,27,29 \
		^^c#8c5dd9^^r0,11,23,1^^r0,12,22,1 \
		^^c#3a3150^^r0,30,23,1^^r0,31,23,1 \
		^^f6^^c#eeeeee^^b#4b3f66^%s^f6 \
		^^c#3a3150^^r0,13,1,17^^r1,12,1,18^^f6 \
		^^c#8c5dd9^^r0,7,1,29^^r1,6,1,30^^r2,5,1,33^^f7^",
		micmutcmd, 5, -1 },

        /* time and date */
        { datetime,
		"^c#3a3150^^r0,4,58,1^^r0,5,1,34^^f1 \
		^^c#8c5dd9^^r0,5,57,1^^r0,6,57,1^^r0,7,1,31^^r1,7,1,30 \
		^^c#3a3150^^r2,36,55,1^^r1,37,56,1 \
		^^c#8c5dd9^^r0,38,57,1 \
		^^f2^^c#4b3f66^^r0,7,55,29 \
		^^c#a86f46^^r6,13,6,17 \
		^^f4^^c#875534^^r0,11,51,1^^r0,12,50,1^^r0,13,1,19^^r1,13,1,18 \
		^^c#f6c175^^r2,30,49,1^^r1,31,50,1 \
		^^f8^^c#eeeeee^^b#a86f46^%s^f6 \
		^^c#a86f46^^r-6,13,6,17 \
		^^c#f6c175^^r0,13,1,17^^r1,12,1,18^",
		"%H:%M", 1, -1 },
	{ datetime,
		"^f2^^c#3a3150^^r0,4,95,1 \
		^^c#8c5dd9^^r0,5,94,1^^r0,6,93,1 \
		^^c#3a3150^^r0,36,94,1^^r0,37,94,1 \
		^^c#8c5dd9^^r0,38,95,1 \
		^^c#4b3f66^^r0,7,92,29 \
		^^f2^^c#a86f46^^r2,13,6,17 \
		^^c#f6c175^^r0,11,86,1^^r0,12,85,1^^r0,13,1,19^^r1,13,1,18 \
		^^c#875534^^r2,30,84,1^^r1,31,85,1 \
		^^f8^^c#eeeeee^^b#a86f46^%s^f6 \
		^^c#a86f46^^r-6,13,6,17 \
		^^c#875534^^r0,13,1,17^^r1,12,1,18^^f6 \
		^^c#3a3150^^r0,7,1,29^^r1,6,1,30 \
		^^c#8c5dd9^^r2,5,1,33^^f3^",
		"%a %d. %b", 1, -1 },
};

/* maximum output string length */
#define MAXLEN CMDLEN * LEN(args)
