#!/usr/bin/env python3
"""
xml2c — compile a UIXML page description into UIKit C code.

Build-time tool (route B): the XML is a *declarative source*; the generated
.c/.h call the UIKit C API directly, so there is zero runtime parsing cost
and the compiler type-checks every call. This matches the MCU roadmap
(no filesystem / no XML parser in the firmware).

Usage:
    python xml2c.py <page.xml> <out.c> <out.h>

Attribute reference (see res/pages/*.xml for examples):
    Geometry : left top right bottom width height
               width/height may be derived from right/bottom + parent width
    Colors   : #RRGGBB  or  @color/<token>   (tokens below)
    Fonts    : font="h1|h2|body|cap"         -> montserrat 28/20/16/14
    Symbols  : {left} {call} {settings} ...  -> LV_SYMBOL_* (in any text)
    Events   : intentionally NOT in XML — bind in the controller by id
"""

import sys
import re
import xml.etree.ElementTree as ET

# --------------------------------------------------------------------------
# Component factory table: tag -> UIKit create function
# --------------------------------------------------------------------------
FACTORIES = {
    'view':      'UIView_create',
    'card':      'UIView_create',
    'lv_obj':    'UIView_create',
    'label':     'UILabel_create',
    'lv_label':  'UILabel_create',
    'button':    'UIButton_create',
    'switch':    'UISwitch_create',
    'slider':    'UISlider_create',
    'progress':  'UIProgressView_create',
    'textfield': 'UITextField_create',
    'stack':     'UIStackView_create',
}

# C types of created objects (for variable declarations + ids struct)
CTYPES = {
    'view': 'UIView', 'card': 'UIView', 'lv_obj': 'UIView',
    'label': 'UILabel', 'lv_label': 'UILabel',
    'button': 'UIButton', 'switch': 'UISwitch',
    'slider': 'UISlider', 'progress': 'UIProgressView',
    'textfield': 'UITextField', 'stack': 'UIStackView',
}

# --------------------------------------------------------------------------
# Color tokens (mirror apps/launcher/launcher_theme.h)
# --------------------------------------------------------------------------
COLOR_TOKENS = {
    'bg':          0x0B1220,
    'wallpaper':   0x0F172A,
    'surface':     0x1E293B,
    'surfaceAlt':  0x273549,
    'border':      0x334155,
    'textHi':      0xF8FAFC,
    'textMid':     0x94A3B8,
    'textLow':     0x64748B,
    'accent':      0x22D3EE,
    'accentAlt':   0x34D399,
    'success':     0x4ADE80,
}

FONT_TOKENS = {
    'h1':   '&lv_font_montserrat_28',
    'h2':   '&lv_font_montserrat_20',
    'font_h2': '&lv_font_montserrat_20',
    'body': '&lv_font_montserrat_16',
    'font_body': '&lv_font_montserrat_16',
    'cap':  '&lv_font_montserrat_14',
    'font_caption': '&lv_font_montserrat_14',
    'font_caption_bold': '&lv_font_montserrat_14',   # no bold face; 14px fallback
    'font_terminal': '&lv_font_montserrat_14',       # no monospace face; 14px fallback
}

# action_card icon glyphs (not in the Montserrat charset) -> LV_SYMBOL
ICON_MAP = {
    '\u2301': 'LV_SYMBOL_USB',       
    '\u2315': 'LV_SYMBOL_LOOP',      
    '\u21BB': 'LV_SYMBOL_REFRESH',   
    '\u2192': 'LV_SYMBOL_RIGHT',     
    '\u2605': 'LV_SYMBOL_SETTINGS',  
}

SYMBOLS = {
    'left':     'LV_SYMBOL_LEFT',
    'right':    'LV_SYMBOL_RIGHT',
    'settings': 'LV_SYMBOL_SETTINGS',
    'call':     'LV_SYMBOL_CALL',
    'wifi':     'LV_SYMBOL_WIFI',
    'bluetooth':'LV_SYMBOL_BLUETOOTH',
    'bell':     'LV_SYMBOL_BELL',
    'tint':     'LV_SYMBOL_TINT',
    'eyeclose': 'LV_SYMBOL_EYE_CLOSE',
    'eyeopen':  'LV_SYMBOL_EYE_OPEN',
    'drive':    'LV_SYMBOL_DRIVE',
    'refresh':  'LV_SYMBOL_REFRESH',
    'minus':    'LV_SYMBOL_MINUS',
    'plus':     'LV_SYMBOL_PLUS',
}

GEOM = ('left', 'top', 'right', 'bottom', 'width', 'height')


def parse_color(v):
    """#RRGGBB or @color/<token> -> '0xRRGGBB'."""
    if v.startswith('@color/'):
        tok = v[len('@color/'):]
        if tok not in COLOR_TOKENS:
            raise SystemExit('xml2c: unknown color token @color/%s' % tok)
        return '0x%06X' % COLOR_TOKENS[tok]
    m = re.match(r'^#([0-9A-Fa-f]{6})$', v)
    if not m:
        raise SystemExit('xml2c: bad color %r (use #RRGGBB or @color/token)' % v)
    return '0x%s' % m.group(1).upper()


def c_str(s):
    """Python str -> C string literal with symbol substitutions."""
    parts = []
    buf = []
    for m in re.finditer(r'\{(\w+)\}', s):
        tok = m.group(1)
        if tok in SYMBOLS:
            buf.append(s[0:m.start()])
            literal = ''.join(buf)
            if literal:
                parts.append('"%s"' % literal.replace('\\', '\\\\').replace('"', '\\"'))
            parts.append(SYMBOLS[tok])
            s = s[m.end():]
            buf = []
        else:
            buf.append(m.group(0))
    if s or buf:
        parts.append('"%s"' % s.replace('\\', '\\\\').replace('"', '\\"'))
    return ' '.join(parts) if parts else '""'


def slug(title):
    """'Confirm Destructive Commands' -> 'confirm_destructive_commands';
    'Factory Glass #24' -> 'factory_glass__24'. Always a valid C id."""
    words = title.lower().split()
    return '_'.join(''.join(c if c.isalnum() else '_' for c in w) for w in words)


def geom_val(v, parent_w):
    """Resolve '100%' -> '(parent_w) * 100 / 100'; plain ints pass through."""
    if isinstance(v, str) and v.endswith('%'):
        pct = v[:-1]
        return '(%s) * %s / 100' % (parent_w, pct)
    return v


def emit_special(out, el, parent_var, parent_w, ids_fields, ids_assigns):
    """High-level page widgets (light-theme settings schema):
       <app_bar> / <section_header> / <setting_item>."""
    tag = el.tag
    a = dict(el.attrib)
    n = _counter[0]
    _counter[0] += 1

    if tag == 'app_bar':
        title = a.get('title', 'Settings')
        subtitle = a.get('subtitle', '')
        out.append('UIButton *back = UIButton_create((UIView *)%s);' % parent_var)
        out.append('UIButton_set_text(back, LV_SYMBOL_LEFT " Back");')
        out.append('UIButton_set_title_color((UIButton *)back, 0x1F2328);')
        out.append('UIView_set_background_color((UIView *)back, 0xE9EBEF);')
        out.append('lv_obj_set_style_bg_color(UIView_native((UIView *)back), lv_color_hex(0xD3D7DD),'
                   ' LV_PART_MAIN | LV_STATE_PRESSED);')
        out.append('UIView_set_corner_radius((UIView *)back, 16);')
        out.append('UIView_set_frame((UIView *)back, 20, 56, 130, 44);')
        ids_fields.append('    UIButton *back;')
        ids_assigns.append('    ids->back = back;')

        out.append('UILabel *appTitle_%d = UILabel_create((UIView *)%s);' % (n, parent_var))
        out.append('UILabel_set_text(appTitle_%d, "%s");' % (n, title))
        out.append('UILabel_set_color((UILabel *)appTitle_%d, 0x1F2328);' % n)
        out.append('UILabel_set_font((UILabel *)appTitle_%d, &lv_font_montserrat_28);' % n)
        out.append('UILabel_set_alignment((UILabel *)appTitle_%d, 1);' % n)
        out.append('UIView_set_frame((UIView *)appTitle_%d, 0, 30, %s, 40);' % (n, parent_w))

        if subtitle:
            out.append('UILabel *appSub_%d = UILabel_create((UIView *)%s);' % (n, parent_var))
            out.append('UILabel_set_text(appSub_%d, "%s");' % (n, subtitle))
            out.append('UILabel_set_color((UILabel *)appSub_%d, 0x6E7781);' % n)
            out.append('UILabel_set_font((UILabel *)appSub_%d, &lv_font_montserrat_14);' % n)
            out.append('UILabel_set_alignment((UILabel *)appSub_%d, 1);' % n)
            out.append('UIView_set_frame((UIView *)appSub_%d, 0, 66, %s, 22);' % (n, parent_w))

    elif tag == 'section_header':
        y = geom_val(a.get('y', '0'), parent_w)
        text = a.get('text', '')
        out.append('UILabel *hdr_%d = UILabel_create((UIView *)%s);' % (n, parent_var))
        out.append('UILabel_set_text(hdr_%d, "%s");' % (n, text))
        out.append('UILabel_set_color((UILabel *)hdr_%d, 0x57606A);' % n)
        out.append('UILabel_set_font((UILabel *)hdr_%d, &lv_font_montserrat_14);' % n)
        out.append('UIView_set_frame((UIView *)hdr_%d, 20, %s, %s - 40, 22);' % (n, y, parent_w))

    elif tag == 'setting_item':
        y = geom_val(a.get('y', '0'), parent_w)
        title = a.get('title', '')
        value = a.get('value', '')
        typ = a.get('type', '')

        if typ == 'switch':
            sw_id = a.get('id') or slug(title)
            out.append('UISwitch *%s = UISwitch_create((UIView *)%s);' % (sw_id, parent_var))
            out.append('UISwitch_set_on_color((UISwitch *)%s, 0x2F81F7);' % sw_id)
            out.append('UIView_set_frame((UIView *)%s, %s - 104, %s + 4, 80, 32);'
                       % (sw_id, parent_w, y))
            ids_fields.append('    UISwitch *%s;' % sw_id)
            ids_assigns.append('    ids->%s = %s;' % (sw_id, sw_id))
        elif value:
            out.append('UILabel *val_%d = UILabel_create((UIView *)%s);' % (n, parent_var))
            out.append('UILabel_set_text(val_%d, "%s");' % (n, value))
            out.append('UILabel_set_color((UILabel *)val_%d, 0x6E7781);' % n)
            out.append('UILabel_set_font((UILabel *)val_%d, &lv_font_montserrat_16);' % n)
            out.append('UILabel_set_alignment((UILabel *)val_%d, 2);' % n)
            out.append('UIView_set_frame((UIView *)val_%d, %s - 168, %s + 4, 144, 36);'
                       % (n, parent_w, y))

        out.append('UILabel *itTitle_%d = UILabel_create((UIView *)%s);' % (n, parent_var))
        out.append('UILabel_set_text(itTitle_%d, "%s");' % (n, title))
        out.append('UILabel_set_color((UILabel *)itTitle_%d, 0x24292F);' % n)
        out.append('UILabel_set_font((UILabel *)itTitle_%d, &lv_font_montserrat_16);' % n)
        out.append('UIView_set_frame((UIView *)itTitle_%d, 24, %s, %s - 200, 40);' % (n, y, parent_w))

    elif tag == 'action_card':
        x = geom_val(a.get('x', '0'), parent_w)
        y = a.get('y', '0')
        title = a.get('title', '')
        icon = a.get('icon', '')
        sym = ICON_MAP.get(icon, 'LV_SYMBOL_DUMMY')
        cid = a.pop('id', None) or slug(title)
        out.append('UIView *%s = UIView_create((UIView *)%s);' % (cid, parent_var))
        out.append('UIView_set_background_color((UIView *)%s, 0xFFFFFF);' % cid)
        out.append('UIView_set_corner_radius((UIView *)%s, 12);' % cid)
        out.append('UIView_set_user_interaction((UIView *)%s, true);' % cid)
        out.append('UIView_set_frame((UIView *)%s, %s, %s, 120, 92);' % (cid, x, y))
        ids_fields.append('    UIView *%s;' % cid)
        ids_assigns.append('    ids->%s = %s;' % (cid, cid))
        out.append('UILabel *aicon_%d = UILabel_create((UIView *)%s);' % (n, cid))
        out.append('UILabel_set_text(aicon_%d, %s);' % (n, sym))
        out.append('UILabel_set_color((UILabel *)aicon_%d, 0x2F81F7);' % n)
        out.append('UILabel_set_font((UILabel *)aicon_%d, &lv_font_montserrat_28);' % n)
        out.append('UILabel_set_alignment((UILabel *)aicon_%d, 1);' % n)
        out.append('UIView_set_frame((UIView *)aicon_%d, 0, 12, 120, 36);' % n)
        out.append('UILabel *atitle_%d = UILabel_create((UIView *)%s);' % (n, cid))
        out.append('UILabel_set_text(atitle_%d, "%s");' % (n, title))
        out.append('UILabel_set_color((UILabel *)atitle_%d, 0x141418);' % n)
        out.append('UILabel_set_font((UILabel *)atitle_%d, &lv_font_montserrat_16);' % n)
        out.append('UILabel_set_alignment((UILabel *)atitle_%d, 1);' % n)
        out.append('UIView_set_frame((UIView *)atitle_%d, 0, 54, 120, 24);' % n)

    elif tag == 'device_card':
        x = geom_val(a.get('x', '20'), parent_w)
        y = a.get('y', '0')
        name = a.get('device_name', '')
        conn = a.get('device_connection', '')
        cid = a.pop('id', None) or slug(name)
        conn_color = '0x2EAA59' if conn.startswith('Connected') else '0x8B8E97'
        out.append('UIView *%s = UIView_create((UIView *)%s);' % (cid, parent_var))
        out.append('UIView_set_background_color((UIView *)%s, 0xFFFFFF);' % cid)
        out.append('UIView_set_corner_radius((UIView *)%s, 16);' % cid)
        out.append('UIView_set_user_interaction((UIView *)%s, true);' % cid)
        out.append('UIView_set_frame((UIView *)%s, %s, %s, 353, 70);' % (cid, x, y))
        ids_fields.append('    UIView *%s;' % cid)
        ids_assigns.append('    ids->%s = %s;' % (cid, cid))
        out.append('UILabel *dname_%d = UILabel_create((UIView *)%s);' % (n, cid))
        out.append('UILabel_set_text(dname_%d, "%s");' % (n, name))
        out.append('UILabel_set_color((UILabel *)dname_%d, 0x141418);' % n)
        out.append('UILabel_set_font((UILabel *)dname_%d, &lv_font_montserrat_16);' % n)
        out.append('UIView_set_frame((UIView *)dname_%d, 20, 14, 313, 24);' % n)
        out.append('UILabel *dconn_%d = UILabel_create((UIView *)%s);' % (n, cid))
        out.append('UILabel_set_text(dconn_%d, "%s");' % (n, conn))
        out.append('UILabel_set_color((UILabel *)dconn_%d, %s);' % (n, conn_color))
        out.append('UILabel_set_font((UILabel *)dconn_%d, &lv_font_montserrat_14);' % n)
        out.append('UIView_set_frame((UIView *)dconn_%d, 20, 42, 313, 20);' % n)

    elif tag == 'tab_bar':
        active = a.get('active', '')
        out.append('UIView *tabbar = UIView_create((UIView *)%s);' % parent_var)
        out.append('UIView_set_background_color((UIView *)tabbar, 0xFFFFFF);')
        out.append('UIView_set_frame((UIView *)tabbar, 0, screen_h - 60, %s, 60);' % parent_w)
        for i, (key, label) in enumerate((('terminal', 'Terminal'),
                                          ('devices', 'Devices'),
                                          ('settings', 'Settings'))):
            tid = 'tab_' + key
            is_active = '1' if key == active else '0'
            out.append('UIButton *%s = UIButton_create((UIView *)tabbar);' % tid)
            out.append('UIButton_set_text(%s, "%s");' % (tid, label))
            out.append('UIButton_set_title_color((UIButton *)%s, %s);'
                       % (tid, '0x2F81F7' if is_active == '1' else '0x8B8E97'))
            out.append('lv_obj_set_style_bg_opa(UIView_native((UIView *)%s), LV_OPA_TRANSP, 0);' % tid)
            out.append('UIView_set_frame((UIView *)%s, %d, 0, %d, 60);'
                       % (tid, i * 160, 160))
            ids_fields.append('    UIButton *%s;' % tid)
            ids_assigns.append('    ids->%s = %s;' % (tid, tid))


def emit_attrs(out, var, ctype, attrs, parent_w):
    """Emit component + base-view attribute setters (geometry last)."""
    a = dict(attrs)
    a.pop('tag', None)
    a.pop('id', None)
    a.pop('name', None)
    if 'bg_color' in a:      # alias: bg_color == bg
        a.setdefault('bg', a.pop('bg_color'))
    if 'scrollable' in a:    # accepted, no-op (page-level scrolling)
        a.pop('scrollable')
    if 'text_font' in a:     # alias: text_font == font
        a.setdefault('font', a.pop('text_font'))
    if 'text_color' in a:    # alias: text_color == color
        a.setdefault('color', a.pop('text_color'))

    # --- text / content --------------------------------------------------
    if 'text' in a:
        out.append('%s_set_text(%s, %s);' % (ctype, var, c_str(a.pop('text'))))
    if 'placeholder' in a:
        out.append('UITextField_set_placeholder((UITextField *)%s, %s);'
                   % (var, c_str(a.pop('placeholder'))))

    # --- colors (component-specific) -------------------------------------
    if 'color' in a:
        c = parse_color(a.pop('color'))
        if ctype == 'UILabel':
            out.append('UILabel_set_color((UILabel *)%s, %s);' % (var, c))
        elif ctype == 'UITextField':
            out.append('UITextField_set_text_color((UITextField *)%s, %s);' % (var, c))
        elif ctype == 'UIButton':
            out.append('UIButton_set_title_color((UIButton *)%s, %s);' % (var, c))
        else:
            raise SystemExit('xml2c: "color" not supported on <%s>' % attrs.get('tag', '?'))
    if 'onColor' in a:
        out.append('UISwitch_set_on_color((UISwitch *)%s, %s);'
                   % (var, parse_color(a.pop('onColor'))))
    if 'indicator' in a:
        out.append('UISlider_set_indicator_color((UISlider *)%s, %s);'
                   % (var, parse_color(a.pop('indicator'))))
    if 'track' in a:
        out.append('UIProgressView_set_track_color((UIProgressView *)%s, %s);'
                   % (var, parse_color(a.pop('track'))))
    if 'progress' in a:
        out.append('UIProgressView_set_progress_color((UIProgressView *)%s, %s);'
                   % (var, parse_color(a.pop('progress'))))

    # --- range / value ---------------------------------------------------
    if 'min' in a or 'max' in a:
        mn = a.pop('min', '0')
        mx = a.pop('max', '100')
        if ctype == 'UISlider':
            out.append('UISlider_set_range((UISlider *)%s, %s, %s);' % (var, mn, mx))
        elif ctype == 'UIProgressView':
            out.append('UIProgressView_set_range((UIProgressView *)%s, %s, %s);' % (var, mn, mx))
    if 'value' in a:
        v = a.pop('value')
        if ctype == 'UISlider':
            out.append('UISlider_set_value((UISlider *)%s, %s);' % (var, v))
        elif ctype == 'UIProgressView':
            out.append('UIProgressView_set_value((UIProgressView *)%s, %s);' % (var, v))

    # --- stack -----------------------------------------------------------
    if ctype == 'UIStackView':
        if 'axis' in a:
            ax = a.pop('axis')
            val = 'UIStackAxis_HORIZONTAL' if ax in ('h', 'horizontal') else 'UIStackAxis_VERTICAL'
            out.append('UIStackView_set_axis((UIStackView *)%s, %s);' % (var, val))
        if 'spacing' in a:
            out.append('UIStackView_set_spacing((UIStackView *)%s, %s);' % (var, a.pop('spacing')))

    # --- font ------------------------------------------------------------
    if 'font' in a:
        f = a.pop('font')
        if f not in FONT_TOKENS:
            raise SystemExit('xml2c: unknown font %r' % f)
        if ctype == 'UIButton':
            out.append('lv_obj_set_style_text_font(UIView_native((UIView *)%s), %s, 0);'
                       % (var, FONT_TOKENS[f]))
        else:
            out.append('UILabel_set_font((UILabel *)%s, %s);' % (var, FONT_TOKENS[f]))

    # --- alignment -------------------------------------------------------
    if 'halign' in a:
        h = a.pop('halign')
        val = {'center': '1', 'left': '0', 'right': '2'}.get(h)
        if val is None:
            raise SystemExit('xml2c: bad halign %r' % h)
        out.append('UILabel_set_alignment((UILabel *)%s, %s);' % (var, val))

    # --- base-view appearance ---------------------------------------------
    if 'bg' in a:
        out.append('UIView_set_background_color((UIView *)%s, %s);'
                   % (var, parse_color(a.pop('bg'))))
    if 'pad_all' in a:
        out.append('lv_obj_set_style_pad_all(UIView_native((UIView *)%s), %s, 0);'
                   % (var, a.pop('pad_all')))
    if 'pressedBg' in a:
        out.append('lv_obj_set_style_bg_color(UIView_native((UIView *)%s), lv_color_hex(%s),'
                   ' LV_PART_MAIN | LV_STATE_PRESSED);'
                   % (var, parse_color(a.pop('pressedBg'))))
    if 'radius' in a:
        out.append('UIView_set_corner_radius((UIView *)%s, %s);' % (var, a.pop('radius')))
    if 'opacity' in a:
        out.append('UIView_set_opacity((UIView *)%s, %s);' % (var, a.pop('opacity')))
    if 'shadow' in a:
        w = a.pop('shadow')
        out.append('lv_obj_set_style_shadow_width(UIView_native((UIView *)%s), %s, 0);' % (var, w))
        out.append('lv_obj_set_style_shadow_color(UIView_native((UIView *)%s), lv_color_hex(0x000000), 0);' % var)
        out.append('lv_obj_set_style_shadow_opa(UIView_native((UIView *)%s), 60, 0);' % var)

    # --- geometry ----------------------------------------------------------
    left = a.pop('left', None)
    if left is None and 'x' in a:
        left = a.pop('x')
    top = a.pop('top', None)
    if top is None and 'y' in a:
        top = a.pop('y')
    right = a.pop('right', None)
    bottom = a.pop('bottom', None)
    width = a.pop('width', None)
    height = a.pop('height', None)

    if width is None:
        if right is not None:
            if left is None:
                raise SystemExit('xml2c: <%s> needs left when right is given' % attrs.get('tag', '?'))
            width = '%s - %s - %s' % (parent_w, left, right)
        else:
            width = '%s - %s' % (parent_w, left or '0')   # full width
    if height is None:
        if bottom is not None:
            height = 'screen_h - %s - %s' % (top or '0', bottom)
        else:
            height = '0'
    width = geom_val(width, parent_w)
    height = geom_val(height, 'screen_h')
    x = left if left is not None else ('%s - %s - %s' % (parent_w, right, width) if right else '0')
    y = top if top is not None else '0'
    x = geom_val(x, parent_w)

    out.append('UIView_set_frame((UIView *)%s, %s, %s, %s, %s);' % (var, x, y, width, height))

    unknown = [k for k in a if k != 'id']
    if unknown:
        raise SystemExit('xml2c: <%s> unknown attribute(s): %s' % (attrs.get('tag', '?'), ', '.join(unknown)))


_counter = [0]


def emit_element(out, el, parent_var, parent_w, ids_fields, ids_assigns):
    tag = el.tag
    if tag in ('app_bar', 'section_header', 'setting_item',
               'action_card', 'device_card', 'tab_bar'):
        emit_special(out, el, parent_var, parent_w, ids_fields, ids_assigns)
        return '_special'
    attrs = dict(el.attrib)
    attrs['tag'] = tag
    ctype = CTYPES[tag]
    vid = attrs.get('id') or attrs.get('name')
    var = vid if vid else '_v%d' % _counter[0]
    _counter[0] += 1

    out.append('%s *%s = %s((UIView *)%s);' % (ctype, var, FACTORIES[tag], parent_var))
    if tag in ('label', 'button', 'switch', 'slider', 'progress', 'textfield'):
        out.append('UIView_set_user_interaction((UIView *)%s, true);' % var)

    emit_attrs(out, var, ctype, attrs, parent_w)

    if vid:
        ids_fields.append('    %s *%s;' % (ctype, vid))
        ids_assigns.append('    ids->%s = %s;' % (vid, var))

    # container width for children (derived from this element's geometry)
    child_parent_w = parent_w
    w_attr = el.attrib.get('width')
    l_attr = el.attrib.get('left')
    r_attr = el.attrib.get('right')
    if w_attr and l_attr:
        child_parent_w = w_attr
    elif r_attr and l_attr:
        child_parent_w = '%s - %s - %s' % (parent_w, l_attr, r_attr)
    elif r_attr:
        child_parent_w = '%s - %s - %s' % (parent_w, r_attr, w_attr or '0')

    for child in list(el):
        child_var = emit_element(out, child, var, child_parent_w, ids_fields, ids_assigns)
        if tag == 'stack':
            out.append('UIStackView_add_arranged_subview((UIStackView *)%s, (UIView *)%s);'
                       % (var, child_var))
    return var


def main():
    if len(sys.argv) != 4:
        raise SystemExit('usage: xml2c.py <page.xml> <out.c> <out.h>')
    xml_path, c_path, h_path = sys.argv[1], sys.argv[2], sys.argv[3]

    tree = ET.parse(xml_path)
    root = tree.getroot()
    if root.tag not in ('page', 'screen'):
        raise SystemExit('xml2c: root element must be <page> or <screen>')
    page_name = root.attrib.get('name', 'page')
    page_var = page_name.replace('-', '_') + '_xml_build'
    ids_type = ''.join(w.capitalize() for w in page_name.replace('-', '_').split('_')) + 'XmlIds'
    guard = 'UIKIT_XML_%s_H' % page_name.replace('-', '_').upper()

    ids_fields, ids_assigns = [], []
    out = []
    out.append('')
    out.append('#include <string.h>')
    out.append('#include "%s_xml.h"' % page_name)
    out.append('')
    out.append('UIView *%s(%s *ids)' % (page_var, ids_type))
    out.append('{')
    out.append('    if (ids) memset(ids, 0, sizeof(*ids));')
    out.append('    const int32_t screen_w = lv_disp_get_hor_res(NULL);')
    out.append('    const int32_t screen_h = lv_disp_get_ver_res(NULL);')
    out.append('')

    _counter[0] = 0
    page_bg = parse_color(root.attrib.get('bg', '@color/bg'))
    out.append('    UIView *page = UIView_create(NULL);')
    out.append('    UIView_set_background_color(page, %s);' % page_bg)
    out.append('    UIView_set_frame(page, 0, 0, screen_w, screen_h);')
    if 'id' in root.attrib:
        ids_fields.append('    UIView *%s;' % root.attrib['id'])
        ids_assigns.append('    ids->%s = page;' % root.attrib['id'])

    for child in list(root):
        emit_element(out, child, 'page', 'screen_w', ids_fields, ids_assigns)

    out.append('')
    for a in ids_assigns:
        out.append(a)
    out.append('    return page;')
    out.append('}')
    out.append('')

    header = [
        '',
        '#ifndef %s' % guard,
        '#define %s' % guard,
        '',
        '#include "UIKit.h"',
        '',
        '#ifdef __cplusplus',
        'extern "C" {',
        '#endif',
        '',
        '/* id -> widget map for the %s page (controllers bind events via these) */' % page_name,
        'typedef struct {',
    ]
    header += ids_fields
    header += [
        '} %s;' % ids_type,
        '',
        'UIView *%s(%s *ids);' % (page_var, ids_type),
        '',
        '#ifdef __cplusplus',
        '}',
        '#endif',
        '',
        '#endif /* %s */' % guard,
        '',
    ]

    with open(c_path, 'w', encoding='utf-8') as f:
        f.write('\n'.join(out))
    with open(h_path, 'w', encoding='utf-8') as f:
        f.write('\n'.join(header))
    print('xml2c: %s -> %s (%d lines)' % (xml_path, c_path, len(out)))


if __name__ == '__main__':
    main()
