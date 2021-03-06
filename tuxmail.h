
#ifndef __TUXMAIL_H__
#define __TUXMAIL_H__

//#include "config.h"
/*
#if !defined(HAVE_DVB_API_VERSION) && defined(HAVE_OST_DMX_H)
#define HAVE_DVB_API_VERSION 1
#endif
#define HAVE_DVB_API_VERSION 3*/

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/fb.h>
#include <linux/input.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H
#include FT_CACHE_SMALL_BITMAPS_H

#if ((defined(FREETYPE_MAJOR)) && (((FREETYPE_MAJOR == 2) && (((FREETYPE_MINOR == 1) && (FREETYPE_PATCH >= 9)) || (FREETYPE_MINOR > 1))) || (FREETYPE_MAJOR > 2)))
#define FTC_Manager_Lookup_Face FTC_Manager_LookupFace
#define _FTC_SBit_Cache_Lookup(a,b,c,d)	FTC_SBitCache_Lookup(a,b,c,d,NULL)
#else
#define _FTC_SBit_Cache_Lookup(a,b,c,d)	FTC_SBit_Cache_Lookup(a,b,c,d)
#endif

#define SCKFILE "/tmp/tuxmaild.socket"
#define LCKFILE "/tmp/lcd.locked"
#define RUNFILE "/var/etc/.tuxmaild"
#define CFGPATH "/var/tuxbox/config/tuxmail/"
#define CFGFILE "tuxmail.conf"
#define SPMFILE "spamlist"
#define POP3FILE "/tmp/tuxmail.pop3"
#define SMTPFILE "/tmp/tuxmail.smtp"
#define NOTIFILE "/tmp/tuxmail.new"
#define TEXTFILE "/var/tuxbox/config/tuxmail/mailtext"
#define ADDRFILE "/var/tuxbox/config/tuxmail/tuxmail.addr"
#define T9FILE   "/var/tuxbox/config/tuxmail/tuxmail.t9"
#define KBLCKFILE "/tmp/keyboard.lck"										//! file to lock keyboard-conversion

#define OE_START "/etc/rc2.d/S99tuxmail"
#define OE_KILL0 "/etc/rc0.d/K00tuxmail"
#define OE_KILL6 "/etc/rc6.d/K00tuxmail"

#define MAXMAIL 100

// rc codes

#define	RC_0			'0'
#define	RC_1			'1'
#define	RC_2			'2'
#define	RC_3			'3'
#define	RC_4			'4'
#define	RC_5			'5'
#define	RC_6			'6'
#define	RC_7			'7'
#define	RC_8			'8'
#define	RC_9			'9'

#define	RC_RIGHT	0x0191
#define	RC_LEFT		0x0192
#define	RC_UP		0x0193
#define	RC_DOWN		0x0194
#define	RC_PLUS		0x0195
#define	RC_MINUS	0x0196

#define	RC_OK				0x0D
#define	RC_STANDBY	0x1C
#define RC_ESC			RC_HOME

#define	RC_HOME			0x01B1
#define	RC_MUTE			0x01B2
#define	RC_HELP			0x01B3
#define	RC_DBOX			0x01B4

#define	RC_GREEN	0x01A1
#define	RC_YELLOW	0x01A2
#define	RC_RED		0x01A3
#define	RC_BLUE		0x01A4

#define RC_PAUSE	RC_HELP
#define RC_ALTGR	0x12
#define RC_BS			0x08
#define RC_POS1		RC_HOME
#define RC_END		0x13
#define RC_INS		0x10
#define RC_ENTF		0x11
#define RC_STRG		0x00
#define RC_LSHIFT	0x0E
#define RC_RSHIFT	0x0E
#define RC_ALT		0x0F
#define RC_NUM		RC_DBOX
#define RC_ROLLEN	0x00
#define RC_F5			0x01C5
#define RC_F6			0x01C6
#define RC_F7			0x01C7
#define RC_F8			0x01C8
#define RC_F9			0x01C9
#define RC_F10		0x01CA
#define RC_RET		0x0D
#define RC_RET1		0x01CC
#define RC_CAPSLOCK	0x01CD
#define RC_ON			0x01CE

#define RC_F1		RC_RED
#define RC_F2		RC_GREEN
#define RC_F3		RC_YELLOW
#define RC_F4		RC_BLUE
#define RC_PAGEUP	RC_PLUS
#define RC_PAGEDOWN	RC_MINUS

int rctable[] = 
{
   0x00, RC_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '�', '�', RC_BS, 0x09,
   'q',  'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', '�', '+', RC_RET, RC_STRG, 'a', 's',
   'd',  'f', 'g', 'h', 'j', 'k', 'l', '�', '�', '^', RC_LSHIFT, '#', 'y', 'x', 'c', 'v',
   'b',  'n', 'm', ',', '.', '-', RC_RSHIFT, 0x00, RC_ALT, 0x20, RC_CAPSLOCK,RC_F1,RC_F2,RC_F3,RC_F4,RC_F5,
   RC_F6,RC_F7,RC_F8,RC_F9,RC_F10,RC_NUM,RC_ROLLEN,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, RC_STANDBY, 0x00, 0x00, 0x00, 0x00, '<', RC_OK, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, RC_ALTGR, 0x00, RC_POS1, RC_UP, RC_PAGEUP, RC_LEFT, RC_RIGHT, RC_END, RC_DOWN,RC_PAGEDOWN,RC_INS,RC_ENTF,
   0x00, RC_MUTE, RC_MINUS, RC_PLUS, RC_STANDBY, 0x00, 0x00, RC_PAUSE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
int rcshifttable[] = 
{
   0x00, RC_ESC, '!', '"', '�', '$', '%', '&', '/', '(', ')', '=', '?', '`', 0x08, 0x09,
   'Q',  'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', '�', '*', RC_RET1, RC_STRG, 'A', 'S',
   'D',  'F', 'G', 'H', 'J', 'K', 'L', '�', '�', '�', RC_LSHIFT, 0x27, 'Y', 'X', 'C', 'V',
   'B',  'N', 'M', ';', ':', '_', RC_RSHIFT, 0x00, RC_ALT, 0x20, RC_CAPSLOCK,RC_F1,RC_F2,RC_F3,RC_F4,RC_F5,
   RC_F6,RC_F7,RC_F8,RC_F9,RC_F10,RC_NUM,RC_ROLLEN,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, RC_STANDBY, 0x00, 0x00, 0x00, 0x00, '>'
};
int rcaltgrtable[] = 
{
   0x00, RC_ESC, 0x00, '�', '�', 0x00, 0x00, 0x00, '{', '[', ']', '}', '\\', 0x00, 0x00, 0x00,
   '@',  0x00, '�', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '~', RC_RET1, RC_STRG, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, RC_LSHIFT, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00,  0x00, '�', 0x00, 0x00, 0x00, RC_RSHIFT, 0x00, RC_ALT, 0x20, RC_CAPSLOCK,RC_F1,RC_F2,RC_F3,RC_F4,RC_F5,
   RC_F6,RC_F7,RC_F8,RC_F9,RC_F10,RC_NUM,RC_ROLLEN,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, RC_STANDBY, 0x00, 0x00, 0x00, 0x00, '|'
};
	
// kb codes

#define KBC_UP		0x01
#define KBC_DOWN	0x02
#define KBC_RIGHT	0x03
#define KBC_LEFT	0x04
#define KBC_INS		0x05
#define KBC_DEL		0x06
#define KBC_POS1	0x07
#define KBC_BACKSPACE	0x08
#define KBC_END		0x0A
#define KBC_PAGEUP	0x0B
#define KBC_PAGEDOWN	0x0C
#define KBC_RETURN	0x0D

// defines for mail-reading/writing
 
#define BORDERSIZE 4
#define FONTHEIGHT_BIG 30
#define FONTHEIGHT_NORMAL 24
#define FONTHEIGHT_SMALL 20
#define FONT_OFFSET_BIG 6
#define FONT_OFFSET 5
#define VIEWY  504
#define INFOBOXY   125
#define MBOX_WIDTH_NORMAL 360
#define MBOX_WIDTH_BIG 420
#define MBOX_HEIGHT_NORMAL 160
#define KEYBOX_SPACE  5
#define KEYBOX_HEIGHT 25
#define KEYBOX_WIDTH  120


#define MAXINFOLINES 16
#define MAXLINELEN	 80

#define KEYBOX_KEYS 12

char *szKeyBoxInfo[KEYBOX_KEYS] = {
  " @!""#$%&'()*+-./[]\\1"   , "ABC2" , "DEF3" ,
  "GHI4" , "JKL5" , "MNO6" ,
  "PQRS7", "TUV8" , "WXYZ9",
  "0"    ,     "" ,  "" } ;

char *szKeyBoxKey[KEYBOX_KEYS] = {
  "1" , "2" , "3" ,
  "4" , "5" , "6" ,
  "7" , "8" , "9",
  "0" , "+" , "-" } ;

char *szKeyBBoxInfoEn[KEYBOX_KEYS] = {
  "red"   , "OK"   , "clr ln" ,
  "green" , "HOME" , "clr all" ,
  "yellow", "pos1" , "plus",
  "blue"  , "end"  ,  "minus" } ;

#if HAVE_DVB_API_VERSION == 1
char *szKeyBBoxInfoDe[KEYBOX_KEYS] = {
  "leeren"   , ""   , "" ,
  "senden" , "" , "" ,
  "entf.", "" , "plus",
  "einf."  , ""  ,  "minus" } ;

char *szKeyBBoxKey[KEYBOX_KEYS] = {
  "F1" , "" , "" ,
  "F2" , "" , "" ,
  "M1" , "" , "P+",
  "M2" , "" , "P-" } ;
#else
char *szKeyBBoxInfoDe[KEYBOX_KEYS] = {
  "ROT"   , "OK"   , "entf." ,
  "GR�N" , "HOME" , "leeren" ,
  "GELB", "Anf." , "plus",
  "BLAU"  , "Ende"  ,  "minus" } ;

char *szKeyBBoxKey[KEYBOX_KEYS] = {
  "F1" , "F5" , "F9" ,
  "F2" , "F6" , "F10" ,
  "F3" , "F7" , "Pg+",
  "F4" , "F8" , "Pg-" } ;
#endif

char *szDirectStyle[4] = {
"ABC", "Abc", "abc", "Keyboard" };

// functions

void ShowMessage (int message);
int CheckPIN (int Account);
void SaveAndReloadDB (int iSave);
void PaintMessageBox (int width, int height, int button);

// freetype stuff
#define FONT "/share/fonts/micron_bold.ttf"
// if font is not in usual place, we look here:
#define FONT2 "share/fonts/pakenham.ttf"

enum {BUT_EXIT = 1, BUT_OK, BUT_OKEXIT};
enum {LEFT, CENTER, RIGHT};
enum {SMALL, NORMAL, BIG};

FT_Library		library;
FTC_Manager		manager;
static FTC_SBitCache		cache;
FTC_SBit		sbit;
FTC_ImageTypeRec	desc;
FT_Face			face;
FT_UInt			prev_glyphindex;
FT_Bool			use_kerning;

char versioninfo_p[12], versioninfo_d[12] = "?.??";

// config

char admin = 'Y';
char osd = 'G';
int skin = 1;
int pal = 0;
int startdelay = 30;
int intervall = 15;
char logging = 'Y';
char logmode = 'S';
char audio = 'Y';
char savedb = 'Y';
char lcdc = 'Y';
int typeflag = 1;
int video=1;
int webport=80;
char webuser[32] = "";
char webpass[32] = "";
char security[80] = "";
char configcode[5] = "";
int  configpincount = 0;

// mail database

struct mi
{
	char type[2];	/* N=new, O=old, D=del */
	char save[2];	/* save type for restore */
	char uid[80];
	char date[8];	/* 01.Jan */
	char time[6];	/* 00:00 */
	char from[256];
	char subj[256];
};

struct
{
	int inactive;
	int mails;
	char nr[2];	/* 0...9 */
	char time[6];	/* 00:00 */
	char name[32];
	char namebox[32];
	char status[8];	/* 000/000 */
	int pincount;
	char pop3[64];
	char imap[64];
	char user[64];
	char pass[64];
	char smtp[64];
	char from[64];
	char code[5];
	int  auth;
	char suser[64];
	char spass[64];
	char inbox[64];
	int  ssl;
	struct mi mailinfo[MAXMAIL];

}maildb[10];

// devs

int fb, kb, lcd;
int rc;

// daemon commands

enum {GET_STATUS, SET_STATUS, RELOAD_SPAMLIST, GET_VERSION, GET_MAIL, SEND_MAIL, RELOAD_CONFIG};

// framebuffer stuff

enum {UP, DOWN};
enum {FILL, GRID};
enum {TRANSP, WHITE, SKIN0, SKIN1, SKIN2, ORANGE, GREEN, YELLOW, RED};
enum {NODAEMON, STARTDONE, STARTFAIL, STOPDONE, STOPFAIL, BOOTON, BOOTOFF, ADD2SPAM, DELSPAM, SPAMFAIL, INFO, GETMAIL, GETMAILFAIL, SENDMAILDONE, SENDMAILFAIL};

unsigned char *lfb = 0, *lbb = 0;

struct fb_fix_screeninfo fix_screeninfo;
struct fb_var_screeninfo var_screeninfo;

unsigned char bgra[][9][4] =
{		// Transp			White				 skin0				skin1				skin2				Orange				Green				Yellow				Red
	{"\x00\x00\x00\x00", "\xFF\xFF\xFF\xFF", "\x80\x00\x00\xD7", "\x80\x40\x00\xD7", "\xFF\x80\x00\xD7", "\x00\xC0\xFF\xFF", "\x00\xD0\x00\xFF", "\x00\xE8\xE8\xFF", "\x04\x04\xC0\xFF" },
	{"\x00\x00\x00\x00", "\xFF\xFF\xFF\xFF", "\x4D\x3A\x25\xD7", "\x77\x63\x4A\xD7", "\xC1\xAC\x97\xD7", "\x00\xC0\xFF\xFF", "\x00\xD0\x00\xFF", "\x00\xE8\xE8\xFF", "\x04\x04\xC0\xFF" },
	{"\x00\x00\x00\x00", "\xFF\xFF\xFF\xFF", "\x00\x00\x00\xFF", "\x80\x00\x00\xFF", "\xFF\x80\x00\xFF", "\x00\xC0\xFF\xFF", "\x00\xD0\x00\xFF", "\x00\xE8\xE8\xFF", "\x04\x04\xC0\xFF" }
};

int startx, starty, sx, ex, sy, ey, mx, my, bw, bh;
int preset;
int viewx, viewy;
char online;
char mailfile;
char mailsend;
char maildir[256];
int mailcache = 0;
char szInfo[MAXINFOLINES][MAXLINELEN];

struct input_event ev;

unsigned short rccode;
unsigned char kbcode;
//int sim_key = 0;
char tch[100];


char scroll_up[] =
{
	0,0,0,0,0,0,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,0,0,0,0,0,
	0,0,0,0,1,1,1,1,1,1,0,0,0,0,
	0,0,0,0,1,1,1,1,1,1,0,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

char scroll_dn[] =
{
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,1,1,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,0,0,
	0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	0,0,0,0,1,1,1,1,1,1,0,0,0,0,
	0,0,0,0,1,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,0,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,0,0
};

char circle[] =
{
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,0
};

// lcd stuff

unsigned char lcd_buffer[] =
{
	0xE0, 0xF8, 0xFC, 0xFE, 0xFE, 0xFF, 0x7F, 0x7F, 0x7F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0xFF, 0xFF, 0xFF, 0xC7, 0xBB, 0x3B, 0xFB, 0xFB, 0x3B, 0xBB, 0xC7, 0xFF, 0x07, 0xFB, 0xFB, 0x07, 0xFB, 0xFB, 0xFB, 0x07, 0xFF, 0x87, 0x7B, 0xFB, 0xC7, 0xFB, 0x7B, 0x7B, 0x87, 0xFF, 0x07, 0xFB, 0xFB, 0x3B, 0xFB, 0xFB, 0xFB, 0x3B, 0xFB, 0xFB, 0x07, 0xFF, 0x07, 0xFB, 0xFB, 0xBB, 0xFB, 0xFB, 0xFB, 0x07, 0xFF, 0x27, 0xDB, 0xDB, 0x27, 0xFF, 0x07, 0xFB, 0xFB, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x7F, 0x7F, 0x7F, 0xFF, 0xFE, 0xFE, 0xFC, 0xF8, 0xE0,
	0xFF, 0x7F, 0x7F, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x70, 0x6F, 0x6F, 0x70, 0x7F, 0x7F, 0x7F, 0x70, 0x6F, 0x6F, 0x6C, 0x6F, 0x6F, 0x6F, 0x70, 0x7F, 0x70, 0x6F, 0x6F, 0x71, 0x6F, 0x6F, 0x6F, 0x70, 0x7F, 0x70, 0x6F, 0x6F, 0x70, 0x6F, 0x6F, 0x6F, 0x70, 0x6F, 0x6F, 0x70, 0x7F, 0x70, 0x6F, 0x6F, 0x71, 0x6F, 0x6F, 0x6F, 0x70, 0x7F, 0x70, 0x6F, 0x6F, 0x70, 0x7F, 0x70, 0x6F, 0x6F, 0x6C, 0x6D, 0x6D, 0x6D, 0x73, 0x7F, 0x7F, 0x7F, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7F, 0x7F, 0xFF,
	0xFF, 0x00, 0x00, 0x00, 0xF8, 0xFC, 0x04, 0xFC, 0xF8, 0x00, 0xFC, 0xFC, 0x30, 0x60, 0xFC, 0xFC, 0x00, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x40, 0xC0, 0x00, 0xC0, 0x40, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0xE0, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xE0, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0xE0, 0x00, 0x00, 0x00, 0xC0, 0x20, 0x20, 0x20, 0x20, 0xC0, 0x00, 0x00, 0x00, 0xE0, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0xE0, 0x00, 0x00, 0x00, 0xFF,
	0xFF, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x80, 0xC0, 0xE1, 0x31, 0x18, 0x09, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x77, 0x55, 0xDD, 0x00, 0xDD, 0x55, 0xDD, 0x00, 0xDD, 0x55, 0x77, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xE3, 0x14, 0x14, 0x14, 0x14, 0xE3, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
	0xFF, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x81, 0x83, 0x9F, 0x9E, 0x8F, 0x87, 0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0x81, 0x81, 0x80, 0x81, 0x81, 0x81, 0x80, 0x80, 0x80, 0x80, 0x83, 0x86, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x86, 0x83, 0x80, 0x80, 0x80, 0xFF, 0x80, 0x80, 0x80, 0x83, 0x86, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x86, 0x83, 0x80, 0x80, 0x80, 0x83, 0x86, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x86, 0x83, 0x80, 0x80, 0x80, 0x81, 0x82, 0x82, 0x82, 0x82, 0x81, 0x80, 0x80, 0x80, 0x83, 0x86, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x86, 0x83, 0x80, 0x80, 0x80, 0x83, 0x86, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x86, 0x83, 0x80, 0x00, 0x00, 0xFF,
	0xFF, 0x00, 0x00, 0x00, 0xE0, 0x60, 0x60, 0xA0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xA0, 0x60, 0x60, 0xE0, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x10, 0x10, 0x10, 0x10, 0xE0, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0xE0, 0x00, 0x00, 0x00, 0xFF,
	0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x80, 0x40, 0x21, 0x11, 0x0A, 0x06, 0x04, 0x08, 0x08, 0x04, 0x06, 0x0A, 0x11, 0x21, 0x40, 0x80, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x3E, 0x01, 0x00, 0x00, 0xC0, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
	0x07, 0x18, 0x20, 0x40, 0x43, 0x83, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x83, 0x83, 0x80, 0x80, 0x80, 0x80, 0x83, 0x86, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x86, 0x83, 0x80, 0x80, 0x80, 0x83, 0x86, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x86, 0x83, 0x80, 0x80, 0x80, 0x83, 0x86, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x86, 0x83, 0x80, 0x80, 0x80, 0x80, 0x83, 0x84, 0x84, 0x84, 0x84, 0x83, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x83, 0x86, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x86, 0x83, 0x80, 0x80, 0x80, 0x83, 0x86, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x86, 0x83, 0x80, 0x80, 0x80, 0x83, 0x86, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x86, 0x43, 0x40, 0x20, 0x18, 0x07
};

char lcd_status[] =
{
	1,1,1,1,0,0,1,1,1,1,1,0,1,1,0,1,1,	/* PAU */
	1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,
	1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,
	1,1,1,1,0,0,1,1,1,1,1,0,1,1,0,1,1,
	1,1,0,0,0,0,1,1,0,1,1,0,1,1,0,1,1,
	1,1,0,0,0,0,1,1,0,1,1,0,1,1,0,1,1,
	1,1,0,0,0,0,1,1,0,1,1,0,1,1,1,1,1,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,
	0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,1,1,1,0,1,1,0,0,0,0,0,0,0,
	0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,

	0,1,1,1,0,0,1,1,0,0,1,1,0,1,1,0,0,	/* ONL */
	1,1,0,1,1,0,1,1,0,0,1,1,0,1,1,0,0,
	1,1,0,1,1,0,1,1,1,0,1,1,0,1,1,0,0,
	1,1,0,1,1,0,1,1,1,1,1,1,0,1,1,0,0,
	1,1,0,1,1,0,1,1,0,1,1,1,0,1,1,0,0,
	1,1,0,1,1,0,1,1,0,0,1,1,0,1,1,0,0,
	0,1,1,1,0,0,1,1,0,0,1,1,0,1,1,1,1,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
	0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,

	0,1,1,1,0,0,1,1,1,1,1,1,0,1,1,0,0,	/* OFL */
	1,1,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,
	1,1,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,
	1,1,0,1,1,0,0,1,1,1,1,0,0,1,1,0,0,
	1,1,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,
	1,1,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,
	0,1,1,1,0,0,0,1,1,0,0,0,0,1,1,1,1,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,1,0,1,1,1,1,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
	0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

char lcd_digits[] =
{
	0,1,1,1,1,1,1,1,1,0,
	1,1,0,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,1,1,0,0,0,1,
	1,0,0,0,1,1,0,0,0,1,
	1,0,0,0,1,1,0,0,0,1,
	1,0,0,0,1,1,0,0,0,1,
	1,0,0,0,1,1,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,0,1,1,
	0,1,1,1,1,1,1,1,1,0,

	0,0,0,1,1,1,1,0,0,0,
	0,0,1,1,0,0,1,1,0,0,
	0,0,1,0,0,0,0,1,0,0,
	0,0,1,0,0,0,0,1,0,0,
	0,0,1,0,0,0,0,1,0,0,
	0,0,1,0,0,0,0,1,0,0,
	0,0,1,0,0,0,0,1,0,0,
	0,0,1,0,0,0,0,1,0,0,
	0,0,1,0,0,0,0,1,0,0,
	0,0,1,0,0,0,0,1,0,0,
	0,0,1,0,0,0,0,1,0,0,
	0,0,1,0,0,0,0,1,0,0,
	0,0,1,0,0,0,0,1,0,0,
	0,0,1,1,0,0,1,1,0,0,
	0,0,0,1,1,1,1,0,0,0,

	0,1,1,1,1,1,1,1,1,0,
	1,1,0,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,0,0,1,
	0,1,1,1,1,1,0,0,0,1,
	1,1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,1,
	1,0,0,0,0,1,1,1,1,0,
	1,0,0,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,0,1,1,
	0,1,1,1,1,1,1,1,1,0,

	0,1,1,1,1,1,1,1,1,0,
	1,1,0,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,0,0,1,
	0,1,1,1,1,1,0,0,0,1,
	1,1,0,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,1,1,0,
	1,1,0,0,0,0,0,0,1,1,
	0,1,1,1,1,1,0,0,0,1,
	1,1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,0,1,1,
	0,1,1,1,1,1,1,1,1,0,

	0,1,1,1,0,1,1,1,1,0,
	1,1,0,1,1,1,0,0,1,1,
	1,0,0,0,1,0,0,0,0,1,
	1,0,0,0,1,0,0,0,0,1,
	1,0,0,0,1,0,0,0,0,1,
	1,0,0,0,1,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,0,0,1,
	0,1,1,1,1,0,0,0,0,1,
	0,0,0,0,1,0,0,0,0,1,
	0,0,0,0,1,0,0,0,0,1,
	0,0,0,0,1,0,0,0,0,1,
	0,0,0,0,1,1,0,0,1,1,
	0,0,0,0,0,1,1,1,1,0,

	0,1,1,1,1,1,1,1,1,0,
	1,1,0,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,1,
	1,0,0,0,0,1,1,1,1,0,
	1,0,0,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,0,0,1,
	0,1,1,1,1,1,0,0,0,1,
	1,1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,0,1,1,
	0,1,1,1,1,1,1,1,1,0,

	0,1,1,1,1,1,1,1,1,0,
	1,1,0,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,1,
	1,0,0,0,0,1,1,1,1,0,
	1,0,0,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,1,1,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,0,1,1,
	0,1,1,1,1,1,1,1,1,0,

	0,1,1,1,1,1,1,1,1,0,
	1,1,0,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,0,0,1,
	0,1,1,1,1,0,0,0,0,1,
	0,0,0,0,1,0,0,0,0,1,
	0,0,0,0,1,0,0,0,0,1,
	0,0,0,0,1,0,0,0,0,1,
	0,0,0,0,1,0,0,0,0,1,
	0,0,0,0,1,0,0,0,0,1,
	0,0,0,0,1,0,0,0,0,1,
	0,0,0,0,1,0,0,0,0,1,
	0,0,0,0,1,1,0,0,1,1,
	0,0,0,0,0,1,1,1,1,0,

	0,1,1,1,1,1,1,1,1,0,
	1,1,0,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,1,1,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,0,1,1,
	0,1,1,0,0,0,0,1,1,0,
	1,1,0,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,1,1,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,0,1,1,
	0,1,1,1,1,1,1,1,1,0,

	0,1,1,1,1,1,1,1,1,0,
	1,1,0,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,1,1,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,0,0,1,
	0,1,1,1,1,0,0,0,0,1,
	1,1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,0,1,1,
	0,1,1,1,1,1,1,1,1,0,
};
#endif
