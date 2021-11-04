/* line editing and drawing */

typedef struct tern {
	char word;
	char type;
	struct tern* l_child;
	struct tern* r_child;
	struct tern* m_child;
} tern_t;
static tern_t* ROOT = &(tern_t){.word= 'a'};
static sbuf *suggestsb;

//creats a tern_t, allocates memory, and initializes it.
static tern_t* create_node(char w);
//inserts a null-terminated word into the tree.
static tern_t* insert_node(const char* string, tern_t* node);
//finds the node where the given prefix ends. Helper function for 'search'
static tern_t* find_node(const char* string, int l, tern_t* node);
//frees allocated memory. Note: Does not free the root node of the tree.
static void delete(tern_t* root, tern_t* node);
//finds the words with prefix 'pattern' and prints out the results to 'out'.
static int search(const char* pattern, int l, tern_t* node);

static tern_t* create_node(char w)
{
	tern_t *node = (tern_t*)malloc(sizeof(tern_t));
	if (!node)
		abort();
	node->word = w;
	node->l_child = NULL;
	node->m_child = NULL;
	node->r_child= NULL;
	node->type = 0;
	return node;
}

static tern_t* insert_node(const char* string, tern_t* node)
{
	int i = strlen(string);
	if (!node)
		node = create_node(string[0]);
	if (string[0] < node->word)
		node->l_child = insert_node(string, node->l_child);
	else if (string[0] > node->word)
		node->r_child = insert_node(string, node->r_child);
	else {
		//go one level down the tree if the word has not been found here.
		if (i == 1) {
			node->type = 1;
			return node;
		} else
			node->m_child = insert_node(++string, node->m_child);
	}
	return node;
}

static tern_t* find_node(const char* string, int l, tern_t* node)
{
	int i = 0;
	tern_t* currentNode = node;
	while (i < l) {
		if (!currentNode)
			break;
		//look to the left of word
		if (string[i] < currentNode->word)
			currentNode = currentNode->l_child;
		//look to the right of word
		else if (string[i] > currentNode->word)
			currentNode = currentNode->r_child;
		//if out of characters, prefix ends on the current node. Now start search
		else {
			if (i++ == l - 1)
				return currentNode;
			else
				currentNode = currentNode->m_child;
		}
	}
	return NULL;
}

static void deep_search(const char* pattern, int len, tern_t* start)
{
	if (start->type) {
		sbuf_mem(suggestsb, pattern, len)
		sbuf_chr(suggestsb, start->word)
		sbuf_chr(suggestsb, '\n')
	}
	if (start->l_child)
		deep_search(pattern, len, start->l_child);
	if (start->r_child)
		deep_search(pattern, len, start->r_child);
	if (start->m_child) {
		char _pattern[++len];
		memcpy(_pattern, pattern, len);
		_pattern[len-1] = start->word;
		_pattern[len] = '\0';
		deep_search(_pattern, len, start->m_child);
	}
}

static int search(const char* pattern, int l, tern_t* node)
{
	sbuf_cut(suggestsb, 0)
	//finds the node where the prefix ends.
	tern_t* current = find_node(pattern, l, node);
	if (!current)
		return 0;
	else {
		if (current->m_child) {
			deep_search(pattern, l, current->m_child);
			sbuf_null(suggestsb)
			return 1;
		}
	}
	return -1;
}

static void delete(tern_t* root, tern_t* node)
{
	if (node) {
		if (node->l_child) {
			delete(root, node->l_child);
			node->l_child = NULL;
		}
		if (node->r_child) {
			delete(root, node->r_child);
			node->r_child = NULL;
		}
		if (node->m_child) {
			delete(root, node->m_child);
			node->m_child = NULL;
		}
		if (!node->l_child && !node->r_child && !node->m_child) {
			if (node != root)
				free(node);
			return;
		}
	}
}

int dstrlen(const char *s, char delim)
{
	register const char* i;
	for (i=s; *i && *i != delim; ++i);
	return i-s;
}

static void file_ternary(struct lbuf* buf)
{
	char reg[] = "[^\t ;:,`.<>[\\]^%$#@*!?+\\-|/=\\\\{}&\\()'\"\n]*";
	int len, sidx;
	char **ss = lbuf_buf(buf);
	int ln_n = lbuf_len(buf);
	int subs[2];
	rset *rs = rset_make(1, (char*[]){xacreg ? xacreg->s : reg}, xic ? REG_ICASE : 0);
	if (!rs)
		return;
	for (int i = 0; i < ln_n; i++) {
		sidx = 0;
		while (rset_find(rs, ss[i]+sidx, 1, subs, 0) >= 0) {
			len = subs[1] - subs[0];
			if (len > 1) {
				char part[len];
				memcpy(part, ss[i]+sidx+subs[0], len);
				part[len] = '\0';
				if (search(part, len > 2 ? len-1 : len, ROOT) >= 0)
					if (!(suggestsb->s_n && dstrlen(suggestsb->s, '\n') == len))
						insert_node(part, ROOT);
			}
			sidx += subs[1] > 0 ? subs[1] : 1;
		}
	}
	rset_free(rs);
}

static char *kmap_map(int kmap, int c)
{
	static char cs[4];
	char **keymap = conf_kmap(kmap);
	cs[0] = c;
	return keymap[c] ? keymap[c] : cs;
}

static int led_posctx(int dir, int pos, int beg, int end)
{
	return dir >= 0 ? pos - beg : end - pos - 1;
}

/* map cursor horizontal position to terminal column number */
int led_pos(char *s, int pos)
{
	return led_posctx(dir_context(s), pos, xleft, xleft + xcols);
}

static int led_offdir(char **chrs, int *pos, int i)
{
	if (pos[i] + ren_cwid(chrs[i], pos[i]) == pos[i + 1])
		return +1;
	if (pos[i + 1] + ren_cwid(chrs[i + 1], pos[i + 1]) == pos[i])
		return -1;
	return 0;
}

/* highlight text in reverse direction */
static void led_markrev(int n, char **chrs, int *pos, int *att)
{
	int i = 0, j;
	int hl = conf_hlrev();
	while (i + 1 < n) {
		int dir = led_offdir(chrs, pos, i);
		int beg = i;
		while (i + 1 < n && led_offdir(chrs, pos, i) == dir)
			i++;
		if (dir < 0)
			for (j = beg; j <= i; j++)
				att[j] = syn_merge(hl, att[j]);
		if (i == beg)
			i++;
	}
}

void led_bounds(sbuf *out, int *off, char **chrs, int cbeg, int cend)
{
	int l, i = cbeg;
	int pad = ren_torg;
	while (i < cend) {
		int o = off[i - cbeg];
		if (o >= 0) {
			if (pad) {
				char pd[i - cbeg];
				memset(pd, ' ', i - cbeg);
				sbuf_mem(out, pd, i - cbeg)
				pad = 0;
			}
			uc_len(l, chrs[o])
			sbuf_mem(out, chrs[o], l)
			for (; off[i - cbeg] == o; i++);
		} else
			i++;
	}
	sbuf_null(out)
}

#define print_ch1(out) sbuf_mem(out, chrs[o], l)
#define print_ch2(out) sbuf_mem(out, *chrs[o] == ' ' ? "_" : chrs[o], l)

#define hid_ch1(out) sbuf_set(out, ' ', i - l)
#define hid_ch2(out) \
int pre = out->s_n; \
sbuf_set(out, *chrs[o] == '\n' ? '\\' : '-', i - l) \
if (ctx > 0 && *chrs[o] == '\t') \
out->s[out->s_n-1] = '>'; \
else if (*chrs[o] == '\t') \
out->s[pre] = '<'; \

#define led_out(out, n) \
{ int l, att_old = 0, i = 0; \
while (i < cend) { \
	int o = off[i]; \
	int att_new = 0; \
	if (o >= 0) { \
		for (l = i; off[i] == o; i++); \
		att_new = att[o]; \
		if (att_new != att_old) \
			sbuf_str(out, term_att(att_new)) \
		char *s = ren_translate(chrs[o], s0); \
		if (s) \
			sbuf_str(out, s) \
		else if (uc_isprint(chrs[o])) { \
			uc_len(l, chrs[o]) \
			print_ch##n(out) \
		} else { \
			hid_ch##n(out) \
		} \
	} else { \
		if (ctx < 0) \
			sbuf_chr(out, ' ') \
		i++; \
	} \
	att_old = att_new; \
} sbufn_str(out, term_att(0)) } \

/* set xtd and return its old value */
static void td_set(int td)
{
	xotd = xtd;
	xtd = td;
}

#define off_for()\
for (i = 0; i < n; i++) { \
	int curwid = ren_cwid(chrs[i], pos[i]); \
	int curbeg = pos[i] - cbeg; \
	int curend = (pos[i] + curwid - 1) - cbeg; \
	/* initialise off[] using pos[] */ \
	if (curbeg >= 0 && curbeg < cterm && \
			curend >= 0 && curend < cterm) \
		for (j = 0; j < curwid; j++) \
			off[pos[i] + j - cbeg] = i; \
} \

#define off_rev()\
for (i = 0; i < n; i++) { \
	int curwid = ren_cwid(chrs[i], pos[i]); \
	int curbeg = cend - pos[i] - 1; \
	int curend = cend - (pos[i] + curwid - 1) - 1; \
	/* initialise off[] using pos[] */ \
	if (curbeg >= 0 && curbeg < cterm && \
			curend >= 0 && curend < cterm) \
		for (j = 0; j < curwid; j++) \
			off[cend - (pos[i] + j - 1) - 2] = i; \
} \

#define cull_line(name)\
	led_bounds(name, off, chrs, cbeg, cend); \
	s0 = name->s; \
	cbeg = 0; \
	cend = cterm; \
	memset(off, -1, (cterm+1) * sizeof(off[0])); \
	pos = ren_position(s0, &chrs, &n); \

/* render and highlight a line */
void led_render(char *s0, int row, int cbeg, int cend)
{
	int i, j, n, cterm = cend - cbeg;
	sbuf *bsb, *bound = NULL;
	int *pos;		/* pos[i]: the screen position of the i-th character */
	char **chrs;		/* chrs[i]: the i-th character in s1 */
	int off[cterm+1];	/* off[i]: the character at screen position i */
	int att[cterm+1];	/* att[i]: the attributes of i-th character */
	int ctx = dir_context(s0);
	memset(off, -1, (cterm+1) * sizeof(off[0]));
	memset(att, 0, (cterm+1) * sizeof(att[0]));
	pos = ren_position(s0, &chrs, &n);
	if (ctx < 0) {
		off_rev()
		if (pos[n] > xcols || cbeg)
		{
			td_set(-2);
			ren_torg = cbeg;
			sbuf_make(bsb, xcols)
			cull_line(bsb)
			off_rev()
			sbuf_make(bound, xcols)
			cull_line(bound)
			off_rev()
			sbuf_free(bsb)
			ren_torg = cbeg;
			td_set(xotd);
		}
	} else {
		off_for()
		if (pos[n] > xcols || cbeg)
		{
			ren_torg = cbeg;
			int xord = xorder;
			xorder = 0;
			sbuf_make(bound, xcols)
			cull_line(bound)
			off_for()
			xorder = xord;
			ren_torg = cbeg;
		}
	}
	syn_highlight(att, s0, n);
	if (xhlr)
		led_markrev(n, chrs, pos, att);
	/* generate term output */
	term_pos(row, 0);
	term_kill();
	if (vi_hidch)
		led_out(term_sbuf, 2)
	else
		led_out(term_sbuf, 1)
	if (bound)
		sbuf_free(bound)
	if (!term_record)
		term_commit();
}

/* print a line on the screen; for ex messages */
void led_printmsg(char *s, int row)
{
	td_set(+2);
	led_print(s, row);
	td_set(xotd);
}

static int led_lastchar(char *s)
{
	char *r = *s ? strchr(s, '\0') : s;
	if (r != s)
		r = uc_beg(s, r - 1);
	return r - s;
}

static int led_lastword(char *s)
{
	char *r = *s ? uc_beg(s, strchr(s, '\0') - 1) : s;
	int kind;
	while (r > s && uc_isspace(r))
		r = uc_beg(s, r - 1);
	kind = r > s ? uc_kind(r) : 0;
	while (r > s && uc_kind(uc_beg(s, r - 1)) == kind)
		r = uc_beg(s, r - 1);
	return r - s;
}

static void led_printparts(char *ai, char *pref, char *main,
		char *post, int kmap)
{
	sbuf *ln;
	int off, pos;
	int idir = 0;
	sbuf_make(ln, xcols)
	sbuf_str(ln, ai)
	sbuf_str(ln, pref)
	sbufn_str(ln, main)
	off = uc_slen(ln->s);
	/* cursor position for inserting the next character */
	if (*pref || *main || *ai) {
		int len = ln->s_n;
		sbuf_str(ln, kmap_map(kmap, 'a'))
		sbufn_str(ln, post)
		idir = ren_pos(ln->s, off) -
			ren_pos(ln->s, off - 1) < 0 ? -1 : +1;
		sbuf_cut(ln, len)
	}
	term_record = 1;
	sbufn_str(ln, post)
	pos = ren_cursor(ln->s, ren_pos(ln->s, MAX(0, off - 1)));
	if (pos >= xleft + xcols)
		xleft = pos - xcols / 2;
	if (pos < xleft)
		xleft = pos < xcols ? 0 : pos - xcols / 2;
	vi_mod = 2;
	syn_blockhl = 0;
	led_print(ln->s, -1);
	term_pos(-1, led_pos(ln->s, pos + idir));
	sbuf_free(ln)
	term_commit();
}

/* continue reading the character starting with c */
char *led_readchar(int c, int kmap)
{
	static char buf[8];
	int c1, c2;
	int i, n;
	if (c == TK_CTL('v')) {		/* literal character */
		buf[0] = term_read();
		buf[1] = '\0';
		return buf;
	}
	if (c == TK_CTL('k')) {		/* digraph */
		c1 = term_read();
		if (TK_INT(c1))
			return NULL;
		c2 = term_read();
		if (TK_INT(c2))
			return NULL;
		return conf_digraph(c1, c2);
	}
	if ((c & 0xc0) == 0xc0) {	/* utf-8 character */
		buf[0] = c;
		uc_len(n, buf)
		for (i = 1; i < n; i++)
			buf[i] = term_read();
		buf[n] = '\0';
		return buf;
	}
	return kmap_map(kmap, c);
}

/* read a character from the terminal */
char *led_read(int *kmap)
{
	int c = term_read();
	while (!TK_INT(c)) {
		switch (c) {
		case TK_CTL('f'):
			*kmap = xkmap_alt;
			break;
		case TK_CTL('e'):
			*kmap = 0;
			break;
		default:
			return led_readchar(c, *kmap);
		}
		c = term_read();
	}
	return NULL;
}

/* read a line from the terminal */
static char *led_line(char *pref, char *post, char *ai,
		int ai_max, int *key, int *kmap,
		char *insert)
{
	sbuf *sb;
	int ai_len = strlen(ai), len;
	int c, lnmode, last_sug = 0, i = 0;
	char *cs, *sug = NULL, *_sug = NULL;
	time_t quickexit = 0;
	vi_insmov = 0;
	sbufn_make(sb, xcols)
	if (insert)
		sbufn_str(sb, insert)
	if (!pref)
		pref = "";
	if (!post)
		post = "";
	while (1) {
		led_printparts(ai, pref, sb->s, post, *kmap);
		len = sb->s_n;
		c = term_read();
		switch (c) {
		case TK_CTL('f'):
			*kmap = xkmap_alt;
			continue;
		case TK_CTL('e'):
			*kmap = 0;
			continue;
		case TK_CTL('h'):
		case 127:
			if (len)
				sbufn_cut(sb, led_lastchar(sb->s))
			else {
				vi_insmov = c;
				goto kleave;
			}
			break;
		case TK_CTL('u'):
			sbufn_cut(sb, 0)
			break;
		case TK_CTL('w'):
			if (len)
				sbufn_cut(sb, led_lastword(sb->s))
			else {
				vi_insmov = c;
				goto kleave;
			}
			break;
		case TK_CTL('t'):
			if (ai_len < ai_max) {
				ai[ai_len++] = '\t';
				ai[ai_len] = '\0';
			}
			break;
		case TK_CTL('d'):
			/* when ai and pref are empty, remove the first space of sb */
			if (ai_len == 0 && !pref[0]) {
				if (sb->s[0] == ' ' || sb->s[0] == '\t') {
					sbuf_cut(sb, 0)
					sbufn_str(sb, sb->s+1)
				}
			}
			if (ai_len > 0)
				ai[--ai_len] = '\0';
			break;
		case TK_CTL('p'):
			if (vi_regget(0, &lnmode))
				sbufn_str(sb, vi_regget(0, &lnmode))
			break;
		case TK_CTL('g'):
			if (!suggestsb)
				sbuf_make(suggestsb, 1)
			file_ternary(xb);
			break;
		case TK_CTL('y'):
			led_done();
			suggestsb = NULL;
			break;
		case TK_CTL('r'):
			if (!suggestsb || !suggestsb->s_n)
				continue;
			if (!sug)
				sug = suggestsb->s;
			if (suggestsb->s_n == sug - suggestsb->s)
				sug--;
			for (i = 0; sug != suggestsb->s; sug--) {
				if (!*sug) {
					i++;
					if (i == 3) {
						sug++;
						goto redo_suggest;
					} else
						*sug = '\n';
				}
			}
			goto redo_suggest;
		case TK_CTL('n'):
			if (!suggestsb)
				continue;
			if (_sug) {
				if (suggestsb->s_n == sug - suggestsb->s)
					continue;
				redo_suggest:
				if (!(_sug = strchr(sug, '\n'))) {
					sug = suggestsb->s;
					goto lookup;
				}
				suggest:
				*_sug = '\0';
				if ((i = led_lastword(sb->s)) > last_sug)
					i = last_sug;
				sbuf_cut(sb, i)
				last_sug = sb->s_n;
				sbufn_str(sb, sug)
				sug = _sug+1;
				continue;
			}
			lookup:
			cs = sb->s;
			if ((i = led_lastword(cs)) > last_sug)
				i = last_sug;
			if (search(cs + i, len - i, ROOT) == 1) {
				sug = suggestsb->s;
				if (!(_sug = strchr(sug, '\n')))
					continue;
				goto suggest;
			}
			continue;
		case TK_CTL('b'):
			if (ai_max > 0)
				continue;
			xquit = 0;
			td_set(xotd);
			temp_write(0, sb->s);
			temp_switch(0);
			vi();
			temp_switch(0);
			vi(); //redraw past screen
			syn_setft("---");
			td_set(+2);
			xquit = 2;
			i = 0;
			goto cur_histstr;
		case TK_CTL('v'):
			if (ai_max > 0)
				break;
			cur_histstr:
			cs = temp_curstr(0, i);
			if (cs) {
				cs[dstrlen(cs, '\n')] = '\0';
				sbuf_cut(sb, 0)
				sbufn_str(sb, cs)
				i++;
			} else if (i) {
				i = 0;
				goto cur_histstr;
			}
			break;
		case TK_CTL('x'):
			goto kleave;
		case TK_CTL('l'):
			term_clean();
			continue;
		case 'j':
			if (xqexit &&
				(difftime(time(0), quickexit) * 1000) < 1000)
			{
				if (sb->s_n) {
					i = led_lastchar(sb->s);
					if (sb->s[i] != 'k')
						goto _default;
					sbuf_cut(sb, i)
				}
				*key = TK_ESC;
				goto leave;
			}
			goto _default;
		case 'k':
			quickexit = time(0);
		default:
_default:
			if (c == '\n' || TK_INT(c))
				break;
			if ((cs = led_readchar(c, *kmap)))
				sbufn_str(sb, cs)
		}
		if (sb->s_n > len)
			last_sug = sb->s_n;
		sug = NULL; _sug = NULL;
		if (c == '\n' || TK_INT(c))
			break;
	}
kleave:
	*key = c;
leave:
	sbufn_done(sb)
}

/* read an ex command */
char *led_prompt(char *pref, char *post, char *insert,
		int *kmap)
{
	int key;
	td_set(+2);
	temp_open(0, "/hist/", "/");
	char *s = led_line(pref, post, "", 0, &key, kmap, insert);
	td_set(xotd);
	if (key == '\n') {
		temp_write(0, s);
		sbuf *sb; sbuf_make(sb, 256)
		if (pref)
			sbuf_str(sb, pref)
		sbuf_str(sb, s)
		if (post)
			sbuf_str(sb, post)
		free(s);
		sbufn_done(sb)
	}
	free(s);
	return NULL;
}

/* read visual command input */
char *led_input(char *pref, char *post, int *kmap, int cln)
{
	sbuf *sb; sbuf_make(sb, 256)
	char ai[128];
	int ai_max = sizeof(ai) - 1;
	int n = 0;
	int key;
	while (n < ai_max && (*pref == ' ' || *pref == '\t'))
		ai[n++] = *pref++;
	ai[n] = '\0';
	while (1) {
		char *ln = led_line(pref, post, ai, ai_max, &key, kmap, NULL);
		int ln_sp = 0;	/* number of initial spaces in ln */
		while (ln[ln_sp] && (ln[ln_sp] == ' ' || ln[ln_sp] == '\t'))
			ln_sp++;
		/* append the auto-indent only if there are other characters */
		if (ln[ln_sp] || (pref && pref[0]) ||
				(key != '\n' && post[0] && post[0] != '\n'))
			sbuf_str(sb, ai)
		if (pref)
			sbuf_str(sb, pref)
		sbuf_str(sb, ln)
		if (key == '\n')
			sbuf_chr(sb, '\n')
		led_printparts(ai, pref ? pref : "", uc_lastline(ln),
				key == '\n' ? "" : post, *kmap);
		if (key == '\n')
			term_chr('\n');
		if (!pref || !pref[0]) {	/* updating autoindent */
			int ai_len = ai_max ? strlen(ai) : 0;
			int ai_new = ln_sp;
			if (ai_len + ai_new > ai_max)
				ai_new = ai_max - ai_len;
			memcpy(ai + ai_len, ln, ai_new);
			ai[ai_len + ai_new] = '\0';
		}
		if (!xai)
			ai[0] = '\0';
		free(ln);
		if (key != '\n')
			break;
		term_room(1);
		pref = NULL;
		n = 0;
		while (xai && (post[n] == ' ' || post[n] == '\t'))
			n++;
		memmove(post, post + n, strlen(post) - n + 1);
		cln++;
	}
	sbuf_str(sb, post)
	if (TK_INT(key) || (cln && key != TK_CTL('x')))
		sbufn_done(sb)
	sbuf_free(sb)
	return NULL;
}

void led_done(void)
{
	if (suggestsb)
		sbuf_free(suggestsb)
	delete(ROOT, ROOT);
}
