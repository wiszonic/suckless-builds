/* See LICENSE file for copyright and license details. */

/* interval between updates (in ms) */
const unsigned int interval = 1000;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "U";

/* maximum command output length */
#define CMDLEN 2048

/* define commands */
static const char keycmd[] 	= "setxkbmap -print | awk -F'+' '/xkb_symbols/ {print $2}' | tr 'a-z' 'A-Z'";
static const char volcmd[] 	= "wpctl get-volume @DEFAULT_AUDIO_SINK@ | awk '{print int($2 * 100)}'";
static const char volmutcmd[] 	= "wpctl get-volume @DEFAULT_AUDIO_SINK@ | awk '{print substr($3, 2, 1)}'";
static const char miccmd[]	= "wpctl get-volume @DEFAULT_AUDIO_SOURCE@ | awk '{print int($2 * 100)}'";
static const char micmutcmd[] 	= "wpctl get-volume @DEFAULT_AUDIO_SOURCE@ | awk '{print substr($3, 2, 1)}'";
static const char moncmd[]	= "xrandr --verbose | grep -i brightness | awk '{print $2}'";

/* define output */
static const struct arg args[] 	= {
	/* function 	format          		argument 	turn	signal */
	{ run_command, 	"^b1^^f6^%s^f6^^s^",           	"echo 'Key'", 	0,   	-1 },
	{ run_command, 	"^b1^^f6^%s^f6^^i1^^o1^",       keycmd, 	1,   	-1 },
	{ run_command, 	"^b1^^f6^%s^f6^^s^",           	"echo 'Vol'", 	0,   	-1 },
	{ run_command, 	"^b1^^f6^%s%%^f6^^s^",         	volcmd, 	1,   	-1 },
	{ run_command, 	"^b1^^f6^%s^f6^^i1^^o1^",       volmutcmd, 	1,   	-1 },
	{ run_command, 	"^b1^^f6^%s^f6^^s^",           	"echo 'Mic'", 	0,   	-1 },
	{ run_command, 	"^b1^^f6^%s%%^f6^^s^",         	miccmd, 	1,   	-1 },
	{ run_command, 	"^b1^^f6^%s^f6^^i1^^o1^",       micmutcmd, 	1,   	-1 },
	{ run_command, 	"^b1^^f6^%s^f6^^s^",        	"echo 'Bri'", 	0,   	-1 },
	{ run_command, 	"^b1^^f6^%s^f6^^i1^^o1^",       moncmd, 	1,   	-1 },
	{ datetime,	"^b2^^f6^%s^f6^^i2^",		"%H:%M", 	1, 	-1 },
	{ datetime,	"^b2^^f6^%s^f6^^i3^^o2^",	"%a %d. %b", 	1, 	-1 },
};

/* maximum output string length */
#define MAXLEN CMDLEN * LEN(args)

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

