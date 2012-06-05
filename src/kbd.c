#include "../include/kasm.h"
#include "../include/ctype.h"
#include "../include/string.h"
#include "../include/kbd.h"

static char kbd_status[KEYCOUNT];


static unsigned int kbd_EN[][2] = { { NPRTBL, NPRTBL },/*000*/
    { NPRTBL, NPRTBL },/*001 ESCAPE*/
    { '1', '!' }, /*002*/
    { '2', '\"' }, /*003*/
    { '3', '#' }, /*004*/
    { '4', '$' }, /*005*/
    { '5', '%' }, /*006*/
    { '6', '&' }, /*007*/
    { '7', '/' }, /*008*/
    { '8', '(' }, /*009*/
    { '9', ')' }, /*010*/
    { '0', '=' }, /*011*/
    { '\'', '?' }, /*012*/
    { '\n', '\n' }, /*013*/
    { '\b', '\b' }, /*014 BACKSPACE*/
    { '\t', '\t' }, /*015 TAB*/
    { 'q', 'Q' }, /*016*/
    { 'w', 'W' }, /*017*/
    { 'e', 'E' }, /*018*/
    { 'r', 'R' }, /*019*/
    { 't', 'T' }, /*020*/
    { 'y', 'Y' }, /*021*/
    { 'u', 'U' }, /*022*/
    { 'i', 'I' }, /*023*/
    { 'o', 'O' }, /*024*/
    { 'p', 'P' }, /*025*/
    { '\'', '\"' }, /*026*/
    { '+', '*' }, /*027*/
    { '\n', '\n' }, /*028*/
    { NPRTBL, NPRTBL },/*029 CTRL IZQ*/
    { 'a', 'A' }, /*030*/
    { 's', 'S' }, /*031*/
    { 'd', 'D' }, /*032*/
    { 'f', 'F' }, /*033*/
    { 'g', 'G' }, /*034*/
    { 'h', 'H' }, /*035*/
    { 'j', 'J' }, /*036*/
    { 'k', 'K' }, /*037*/
    { 'l', 'L' }, /*038*/
    { '.', ';' }, /*039*/
    { '{', '[' }, /*040*/
    { '}', ']' }, /*041*/
    { NPRTBL, NPRTBL },/*042 SHIFT IZQ*/
    { '<', '>' }, /*043*/
    { 'z', 'Z' }, /*044*/
    { 'x', 'X' }, /*045*/
    { 'c', 'C' }, /*046*/
    { 'v', 'V' }, /*047*/
    { 'b', 'B' }, /*048*/
    { 'n', 'N' }, /*049*/
    { 'm', 'M' }, /*050*/
    { ',', ';' }, /*051*/
    { '.', ':' }, /*052*/
    { '?', '?' }, /*053*/
    { NPRTBL, NPRTBL },/*054 SHIFT DER*/
    { '*', '*' }, /*055 KEY **/
    { NPRTBL, NPRTBL },/*056 ALT IZQ*/
    { ' ', ' ' }, /*057 SPACE*/
    { NPRTBL, NPRTBL }, /*058 CAPSLOCK*/
    { NPRTBL, NPRTBL }, /*059 F1*/
    { NPRTBL, NPRTBL }, /*060 F2*/
    { NPRTBL, NPRTBL }, /*061 F3*/
    { NPRTBL, NPRTBL }, /*062 F4*/
    { NPRTBL, NPRTBL }, /*063 F5*/
    { NPRTBL, NPRTBL }, /*064 F6*/
    { NPRTBL, NPRTBL }, /*065 F7*/
    { NPRTBL, NPRTBL }, /*066 F8*/
    { NPRTBL, NPRTBL }, /*067 F9*/
    { NPRTBL, NPRTBL }, /*068 F10*/
    { NPRTBL, NPRTBL }, /*069 NUM LOCK*/
    { NPRTBL, NPRTBL }, /*070 SCROLL LOCK*/
    { '7', '7' }, /*071 KEY 7*/
    { '8', '8' }, /*072 KEY 8*/
    { '9', '9' }, /*073 KEY 9*/
    { '-', '-' }, /*074 KEY -*/
    { '4', '4' }, /*075 KEY 4*/
    { '5', '5' }, /*076 KEY 5*/
    { '6', '6' }, /*077 KEY 6*/
    { '+', '+' }, /*078 KEY +*/
    { '1', '1' }, /*079 KEY 1*/
    { '2', '2' }, /*080 KEY 2*/
    { '3', '3' }, /*081 KEY 3*/
    { '0', '0' }, /*082 KEY 0*/
    { '.', '.' }, /*083 KEY .*/
    { NPRTBL, NPRTBL },/*084 SYS REQ (AT)*/
    { '+', '*' }, /*085*/
    { '+', '*' }, /*086*/
    { NPRTBL, NPRTBL },/*087 F11*/
    { NPRTBL, NPRTBL },/*088 F12*/
    { '+', '*' }, /*089*/
    { '+', '*' } /*090*/
};

static unsigned int kbd_ES[][2] = { { NPRTBL, NPRTBL },/*000*/
    { NPRTBL, NPRTBL },/*001 ESCAPE*/
    { '1', '!' }, /*002*/
    { '2', '"' }, /*003*/
    { '3', '#' }, /*004*/
    { '4', '$' }, /*005*/
    { '5', '%' }, /*006*/
    { '6', '&' }, /*007*/
    { '7', '/' }, /*008*/
    { '8', '(' }, /*009*/
    { '9', ')' }, /*010*/
    { '0', '=' }, /*011*/
    { '\'', '?' }, /*012*/
    { '\n', '\n' }, /*013*/
    { '\b', '\b' }, /*014 BACKSPACE*/
    { '\t', '\t' }, /*015 TAB*/
    { 'q', 'Q' }, /*016*/
    { 'w', 'W' }, /*017*/
    { 'e', 'E' }, /*018*/
    { 'r', 'R' }, /*019*/
    { 't', 'T' }, /*020*/
    { 'y', 'Y' }, /*021*/
    { 'u', 'U' }, /*022*/
    { 'i', 'I' }, /*023*/
    { 'o', 'O' }, /*024*/
    { 'p', 'P' }, /*025*/
    { '\'', '\"' }, /*026*/
    { '+', '*' }, /*027*/
    { '\n', '\n' }, /*028*/
    { NPRTBL, NPRTBL },/*029 CTRL IZQ*/
    { 'a', 'A' }, /*030*/
    { 's', 'S' }, /*031*/
    { 'd', 'D' }, /*032*/
    { 'f', 'F' }, /*033*/
    { 'g', 'G' }, /*034*/
    { 'h', 'H' }, /*035*/
    { 'j', 'J' }, /*036*/
    { 'k', 'K' }, /*037*/
    { 'l', 'L' }, /*038*/
    { 164, 165 }, /*039*/
    { '{', '[' }, /*040*/
    { '}', ']' }, /*041*/
    { NPRTBL, NPRTBL },/*042 SHIFT IZQ*/
    { '<', '>' }, /*043*/
    { 'z', 'Z' }, /*044*/
    { 'x', 'X' }, /*045*/
    { 'c', 'C' }, /*046*/
    { 'v', 'V' }, /*047*/
    { 'b', 'B' }, /*048*/
    { 'n', 'N' }, /*049*/
    { 'm', 'M' }, /*050*/
    { ',', ';' }, /*051*/
    { '.', ':' }, /*052*/
    { '-', '_' }, /*053*/
    { NPRTBL, NPRTBL },/*054 SHIFT DER*/
    { '*', '*' }, /*055 KEY **/
    { NPRTBL, NPRTBL },/*056 ALT IZQ*/
    { ' ', ' ' }, /*057 SPACE*/
    { NPRTBL, NPRTBL },/*058 CAPSLOCK*/
    { NPRTBL, NPRTBL },/*059 F1*/
    { NPRTBL, NPRTBL },/*060 F2*/
    { NPRTBL, NPRTBL },/*061 F3*/
    { NPRTBL, NPRTBL },/*062 F4*/
    { NPRTBL, NPRTBL },/*063 F5*/
    { NPRTBL, NPRTBL },/*064 F6*/
    { NPRTBL, NPRTBL },/*065 F7*/
    { NPRTBL, NPRTBL },/*066 F8*/
    { NPRTBL, NPRTBL },/*067 F9*/
    { NPRTBL, NPRTBL },/*068 F10*/
    { NPRTBL, NPRTBL },/*069 NUM LOCK*/
    { NPRTBL, NPRTBL },/*070 SCROLL LOCK*/
    { '7', '7' }, /*071 KEY 7*/
    { '8', '8' }, /*072 KEY 8*/
    { '9', '9' }, /*073 KEY 9*/
    { '-', '-' }, /*074 KEY -*/
    { '4', '4' }, /*075 KEY 4*/
    { '5', '5' }, /*076 KEY 5*/
    { '6', '6' }, /*077 KEY 6*/
    { '+', '+' }, /*078 KEY +*/
    { '1', '1' }, /*079 KEY 1*/
    { '2', '2' }, /*080 KEY 2*/
    { '3', '3' }, /*081 KEY 3*/
    { '0', '0' }, /*082 KEY 0*/
    { '.', '.' }, /*083 KEY .*/
    { NPRTBL, NPRTBL },/*084 SYS REQ (AT)*/
    { '+', '*' }, /*085*/
    { '+', '*' }, /*086*/
    { NPRTBL, NPRTBL },/*087 F11*/
    { NPRTBL, NPRTBL },/*088 F12*/
    { '+', '*' }, /*089*/
    { '+', '*' } /*090*/
};

static unsigned int (*kbd)[2] = kbd_EN;

static bool acute = false;
static bool lctrl = false;
static bool rctrl = false;
static bool lshift = false;
static bool rshift = false;
static bool capslock = false;

kbd_callback_t _event_callback;

int kbd_set_keymap(char *code)
{
    if (strcmp(code, "ES") == 0)
    {
        kbd = kbd_ES;
        return 1;
    }

    if (strcmp(code, "EN") == 0)
    {
        kbd = kbd_EN;
        return 1;
    }

    return 0;
}

void kbd_init(kbd_callback_t event_callback)
{
    _event_callback = event_callback;
}


bool kbd_shifted(void)
{
    return lshift || rshift;
}

bool kbd_capitalized(void)
{
    return ((lshift || rshift) && !capslock) || (!(lshift || rshift) && capslock);
}

char kbd_keymap_get(unsigned int scancode)
{
    char key = kbd[scancode & 0x7F][kbd_shifted()];
    if (isalpha(key))
    {
        key = (kbd_capitalized()) ? toupper(key) : tolower(key);
    }
    if (acute)
    {
        /* TODO: May need to disable for english keyboard */
        if (key == 'a')
            key = 160; /* á */
        else if (key == 'e')
            key = 130; /* é */
        else if (key == 'i')
            key = 161; /* í */
        else if (key == 'o')
            key = 162; /* ó */
        else if (key == 'u')
            key = 163; /* ú */

        acute = false;
    }
    return key;
}

void keyboard_handler(void)
{
    unsigned int scancode = _inb(0x60);
    char key = 0;

	kbd_status[scancode & 0x7F] = !(scancode & 0x80);

    if ((scancode & ~SCANCODE_EXT) == SCANCODE_CHAR_LSHIFT)
    {
        lshift = !(scancode & SCANCODE_EXT);
        return;
    }
    if ((scancode & ~SCANCODE_EXT) == SCANCODE_CHAR_RSHIFT)
    {
        rshift = !(scancode & SCANCODE_EXT);
        return;
    }
    if ((scancode & ~SCANCODE_EXT) == SCANCODE_CHAR_CAPSLOCK)
    {
        if (!(scancode & SCANCODE_EXT))
        {
            capslock = !capslock;
        }
        return;
    }
    if ((scancode & ~SCANCODE_EXT) == SCANCODE_CHAR_LCTRL)
    {
        lctrl = !(scancode & SCANCODE_EXT);
        return;
    }
    if ((scancode & ~SCANCODE_EXT) == SCANCODE_CHAR_RCTRL)
    {
        rctrl = !(scancode & SCANCODE_EXT);
        return;
    }
    if ((scancode & ~SCANCODE_EXT) == SCANCODE_CHAR_APOSTROPHE && !acute)
    {
        if (!(scancode & SCANCODE_EXT))
        {
            acute = true;
        }
        return;
    }

    if (!(scancode & 0x80))
    {
        key = kbd_keymap_get(scancode);
    }
    _event_callback(key, kbd_status);
}

