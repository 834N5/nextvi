static char *kmap_en[256] = {
	[0] = "en",
};

static char *kmap_fa[256] = {
	[0] = "fa",
	['`'] = "‍",
	['1'] = "۱",
	['2'] = "۲",
	['3'] = "۳",
	['4'] = "۴",
	['5'] = "۵",
	['6'] = "۶",
	['7'] = "۷",
	['8'] = "۸",
	['9'] = "۹",
	['0'] = "۰",
	['-'] = "-",
	['='] = "=",
	['q'] = "ض",
	['w'] = "ص",
	['e'] = "ث",
	['r'] = "ق",
	['t'] = "ف",
	['y'] = "غ",
	['u'] = "ع",
	['i'] = "ه",
	['o'] = "خ",
	['p'] = "ح",
	['['] = "ج",
	[']'] = "چ",
	['a'] = "ش",
	['s'] = "س",
	['d'] = "ی",
	['f'] = "ب",
	['g'] = "ل",
	['h'] = "ا",
	['j'] = "ت",
	['k'] = "ن",
	['l'] = "م",
	[';'] = "ک",
	['\''] = "گ",
	['z'] = "ظ",
	['x'] = "ط",
	['c'] = "ز",
	['v'] = "ر",
	['b'] = "ذ",
	['n'] = "د",
	['m'] = "پ",
	[','] = "و",
	['.'] = ".",
	['/'] = "/",
	['\\'] = "\\",
	['~'] = "÷",
	['!'] = "!",
	['@'] = "٬",
	['#'] = "٫",
	['$'] = "﷼",
	['%'] = "٪",
	['^'] = "×",
	['&'] = "،",
	['*'] = "*",
	['('] = "(",
	[')'] = ")",
	['_'] = "ـ",
	['+'] = "+",
	['Q'] = "ْ",
	['W'] = "ٌ",
	['E'] = "ٍ",
	['R'] = "ً",
	['T'] = "ُ",
	['Y'] = "ِ",
	['U'] = "َ",
	['I'] = "ّ",
	['O'] = "[",
	['P'] = "]",
	['{'] = "{",
	['}'] = "}",
	['A'] = "ؤ",
	['S'] = "ئ",
	['D'] = "ي",
	['F'] = "إ",
	['G'] = "أ",
	['H'] = "آ",
	['J'] = "ة",
	['K'] = "«",
	['L'] = "»",
	[':'] = ":",
	['"'] = "؛",
	['Z'] = "ك",
	['X'] = "ٓ",
	['C'] = "ژ",
	['V'] = "ٰ",
	['B'] = "‌",
	['N'] = "ٔ",
	['M'] = "ء",
	['<'] = "<",
	['>'] = ">",
	['?'] = "؟",
	['|'] = "|",
};

static char *kmap_ru[256] = {
	[0] = "ru",
	['q'] = "й",
	['w'] = "ц",
	['e'] = "у",
	['r'] = "к",
	['t'] = "е",
	['y'] = "н",
	['u'] = "г",
	['i'] = "ш",
	['o'] = "щ",
	['p'] = "з",
	['['] = "х",
	[']'] = "ь",
	['a'] = "ф",
	['s'] = "ы",
	['d'] = "в",
	['f'] = "а",
	['g'] = "п",
	['h'] = "р",
	['j'] = "о",
	['k'] = "л",
	['l'] = "д",
	[';'] = "ж",
	['\''] = "э",
	['z'] = "я",
	['x'] = "ч",
	['c'] = "с",
	['v'] = "м",
	['b'] = "и",
	['n'] = "т",
	['m'] = "ь",
	[','] = "б",
	['.'] = "ю",
	['/'] = ".",
	['\\'] = "\\",
	['~'] = "÷",
	['!'] = "!",
	['@'] = "\"",
	['#'] = "#",
	['$'] = ";",
	['%'] = "%",
	['^'] = ":",
	['&'] = "?",
	['*'] = "*",
	['('] = "(",
	[')'] = ")",
	['_'] = "_",
	['+'] = "+",
	['Q'] = "Й",
	['W'] = "Ц",
	['E'] = "У",
	['R'] = "К",
	['T'] = "Е",
	['Y'] = "Н",
	['U'] = "Г",
	['I'] = "Ш",
	['O'] = "Щ",
	['P'] = "З",
	['{'] = "Х",
	['}'] = "Ъ",
	['A'] = "Ф",
	['S'] = "Ы",
	['D'] = "В",
	['F'] = "А",
	['G'] = "П",
	['H'] = "Р",
	['J'] = "О",
	['K'] = "Л",
	['L'] = "Д",
	[':'] = "Ж",
	['"'] = "Э",
	['Z'] = "Я",
	['X'] = "Ч",
	['C'] = "С",
	['V'] = "М",
	['B'] = "И",
	['N'] = "Т",
	['M'] = "Ь",
	['<'] = "Б",
	['>'] = "Ю",
	['?'] = ",",
	['|'] = "|",
};

static char **kmaps[] = {kmap_en, kmap_fa, kmap_ru};

static char *digraphs[][2] = {
	{"cq", "’"},
	{"pl", "+"},
	{"hy", "-"},
	{"sl", "/"},
	{"eq", "="},
	{"dq", "\""},
	{"rs", "\\"},
	{"ru", "_"},
	{"ul", "_"},
	{"oq", "‘"},
	{"or", "|"},
	{"!!", "¡"},
	{"r!", "¡"},
	{"c|", "¢"},
	{"ct", "¢"},
	{"L-", "£"},
	{"ps", "£"},
	{"xo", "¤"},
	{"cr", "¤"},
	{"Y-", "¥"},
	{"yn", "¥"},
	{"||", "¦"},
	{"so", "§"},
	{"sc", "§"},
	{"co", "©"},
	{"a_", "ª"},
	{"<<", "«"},
	{"Fo", "«"},
	{"-,", "¬"},
	{"no", "¬"},
	{"ro", "®"},
	{"rg", "®"},
	{"0^", "°"},
	{"de", "°"},
	{"+-", "±"},
	{"2^", "²"},
	{"3^", "³"},
	{"P!", "¶"},
	{"pg" "¶"},
	{".^", "·"},
	{"1^", "¹"},
	{"o_", "º"},
	{">>", "»"},
	{"Fc", "»"},
	{"14", "¼"},
	{"12", "½"},
	{"34", "¾"},
	{"??", "¿"},
	{"r?", "¿"},
	{"A`", "À"},
	{"A'", "Á"},
	{"A^", "Â"},
	{"A~", "Ã"},
	{"A-", "Ā"},
	{"A:", "Ä"},
	{"A\"", "A:"},
	{"A*", "Å"},
	{"AE", "Æ"},
	{"C,", "Ç"},
	{"E`", "È"},
	{"E'", "É"},
	{"E^", "Ê"},
	{"E-", "Ē"},
	{"E:", "Ë"},
	{"I`", "Ì"},
	{"I'", "Í"},
	{"I^", "Î"},
	{"I-", "Ī"},
	{"I:", "Ï"},
	{"D-", "Ð"},
	{"N~", "Ñ"},
	{"O`", "Ò"},
	{"O'", "Ó"},
	{"O^", "Ô"},
	{"O~", "Õ"},
	{"O-", "Ō"},
	{"O:", "Ö"},
	{"xx", "×"},
	{"mu", "×"},
	{"O/", "Ø"},
	{"U`", "Ù"},
	{"U'", "Ú"},
	{"U^", "Û"},
	{"U-", "Ū"},
	{"U:", "Ü"},
	{"Y'", "Ý"},
	{"TH", "Þ"},
	{"ss", "ß"},
	{"a`", "à"},
	{"a'", "á"},
	{"a^", "â"},
	{"a-", "ā"},
	{"a~", "ã"},
	{"a:", "ä"},
	{"a*", "å"},
	{"ae", "æ"},
	{"c,", "ç"},
	{"e`", "è"},
	{"e'", "é"},
	{"e^", "ê"},
	{"e:", "ë"},
	{"e-", "ē"},
	{"i`", "ì"},
	{"i'", "í"},
	{"i^", "î"},
	{"i-", "ī"},
	{"i:", "ï"},
	{"d-", "ð"},
	{"n~", "ñ"},
	{"o`", "ò"},
	{"o'", "ó"},
	{"o^", "ô"},
	{"o~", "õ"},
	{"o-", "ō"},
	{"o:", "ö"},
	{"di", "÷"},
	{"-:", "÷"},
	{"o/", "ø"},
	{"u`", "ù"},
	{"u'", "ú"},
	{"u^", "û"},
	{"u-", "ū"},
	{"u:", "ü"},
	{"y'", "ý"},
	{"y-", "ȳ"},
	{"th", "þ"},
	{"y:", "ÿ"},
	{"C<", "Č"},
	{"c<", "č"},
	{"D<", "Ď"},
	{"d<", "ď"},
	{"e<", "ě"},
	{"n<", "ň"},
	{"O\"", "Ő"},
	{"o\"", "ő"},
	{"R<", "Ř"},
	{"r<", "ř"},
	{"S<", "Š"},
	{"s<", "š"},
	{"T<", "Ť"},
	{"t<", "ť"},
	{"U*", "Ů"},
	{"u*", "ů"},
	{"U\"", "Ű"},
	{"u\"", "ű"},
	{"Y:", "Ÿ"},
	{"Y-", "Ȳ"},
	{"Z<", "Ž"},
	{"z<", "ž"},
	{"fn", "ƒ"},
	{",,", "¸"},
	{",a", "¸"},
	{"aa", "´"},
	{"\\'", "´"},
	{"-a", "¯"},
	{"\"\"", "¨"},
	{":a", "¨"},
	{"^", "ˆ"},
	{"^a", "ˆ"},
	{"va", "ˇ"},
	{"Ua", "˘"},
	{".a", "˙"},
	{"oa", "˚"},
	{"Ca", "˛"},
	{"\"a", "˝"},
	{"~", "˜"},
	{"*A", "Α"},
	{"*B", "Β"},
	{"*G", "Γ"},
	{"*E", "Ε"},
	{"*Z", "Ζ"},
	{"*Y", "Η"},
	{"*H", "Θ"},
	{"*I", "Ι"},
	{"*K", "Κ"},
	{"*L", "Λ"},
	{"*M", "Μ"},
	{"*N", "Ν"},
	{"*C", "Ξ"},
	{"*O", "Ο"},
	{"*P", "Π"},
	{"*R", "Ρ"},
	{"*S", "Σ"},
	{"*T", "Τ"},
	{"*U", "Υ"},
	{"*F", "Φ"},
	{"*X", "Χ"},
	{"*Q", "Ψ"},
	{"*W", "Ω"},
	{"*a", "α"},
	{"*b", "β"},
	{"*g", "γ"},
	{"*d", "δ"},
	{"*e", "ε"},
	{"*z", "ζ"},
	{"*y", "η"},
	{"*h", "θ"},
	{"*i", "ι"},
	{"*k", "κ"},
	{"*l", "λ"},
	{"*m", "μ"},
	{"/u", "µ"},
	{"*n", "ν"},
	{"*c", "ξ"},
	{"*o", "ο"},
	{"*p", "π"},
	{"*r", "ρ"},
	{"ts", "ς"},
	{"*s", "σ"},
	{"*t", "τ"},
	{"*u", "υ"},
	{"*f", "φ"},
	{"*x", "χ"},
	{"*q", "ψ"},
	{"*w", "ω"},
	{"en", "–"},
	{"\\-", "–"},
	{"em", "—"},
	{"--", "—"},
	{"bq", "‚"},
	{"``", "“"},
	{"lq", "“"},
	{"''", "”"},
	{"rq", "”"},
	{"dg", "†"},
	{"dd", "‡"},
	{"bu", "•"},
	{"el", "…"},
	{"%0", "‰"},
	{"fm", "′"},
	{"fo", "‹"},
	{"fc", "›"},
	{"fr", "⁄"},
	{"If", "ℑ"},
	{"ws", "ℛ"},
	{"Rf", "ℜ"},
	{"af", "ℵ"},
	{"<-", "←"},
	{"ua", "↑"},
	{"->", "→"},
	{"da", "↓"},
	{"<>", "↔"},
	{"ab", "↔"},
	{"CR", "↵"},
	{"fa", "∀"},
	{"pd", "∂"},
	{"te", "∃"},
	{"es", "∅"},
	{"*D", "Δ"},
	{"gr", "∇"},
	{"mo", "∈"},
	{"!m", "∉"},
	{"st", "∋"},
	{"pr", "∏"},
	{"su", "∑"},
	{"mi", "−"},
	{"-+", "∓"},
	{"**", "∗"},
	{"sr", "√"},
	{"pt", "∝"},
	{"if", "∞"},
	{"an", "∠"},
	{"l&", "∧"},
	{"l|", "∨"},
	{"ca", "∩"},
	{"cu", "∪"},
	{"is", "∫"},
	{"tf", "∴"},
	{"ap", "∼"},
	{"cg", "≅"},
	{"=~", "≅"},
	{"~~", "≈"},
	{"!=", "≠"},
	{"==", "≡"},
	{"<=", "≤"},
	{">=", "≥"},
	{"sb", "⊂"},
	{"sp", "⊃"},
	{"!b", "⊄"},
	{"ib", "⊆"},
	{"ip", "⊇"},
	{"O+", "⊕"},
	{"Ox", "⊗"},
	{"pp", "⊥"},
	{"c.", "⋅"},
	{"b<", "〈"},
	{"b>", "〉"},
	{"lz", "◊"},
	{"ci", "○"},
	{"la", "⟨"},
	{"ra", "⟩"},
	{"co", ""},
	{"rg", ""},
	{"tm", ""},
	{"rn", ""},
	{"av", ""},
	{"ah", ""},
	{"RG", ""},
	{"CO", ""},
	{"TM", ""},
	{"LT", ""},
	{"br", ""},
	{"LX", ""},
	{"LB", ""},
	{"LT", "⎛"},
	{"LX", "⎜"},
	{"LB", "⎝"},
	{"lc", ""},
	{"lx", ""},
	{"lf", ""},
	{"lc", "⎡"},
	{"lx", "⎢"},
	{"lf", "⎣"},
	{"lt", ""},
	{"lk", ""},
	{"lb", ""},
	{"lt", "⎧"},
	{"lk", "⎨"},
	{"lb", "⎩"},
	{"bv", "⎪"},
	{"RT", ""},
	{"RX", ""},
	{"RB", ""},
	{"RT", "⎞"},
	{"RX", "⎟"},
	{"RB", "⎠"},
	{"rc", ""},
	{"rx", ""},
	{"rf", ""},
	{"rc", "⎤"},
	{"rx", "⎥"},
	{"rf", "⎦"},
	{"rt", ""},
	{"rk", ""},
	{"rb", ""},
	{"rt", "⎫"},
	{"rk", "⎬"},
	{"rb", "⎭"},
	{"ff", "ﬀ"},
	{"fi", "ﬁ"},
	{"fl", "ﬂ"},
	{"ffi", "ﬃ"},
	{"Fi", "ﬃ"},
	{"ffl", "ﬄ"},
	{"Fl", "ﬄ"},
};
