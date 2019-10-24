////////q///////f///////3///////4///////z///////.///////-///////
// Program the process transforming figure arrangement in A into
// that in B. A figure in a cell can be transferred into any one
// of neighbouring empty cells; in the case of such transformati-
// ons the cell previously occupied by the figure becomes empty.
// +---+---+---+---+                   z   +---+---+---+---+   *
// | 7 | 3 | 5 | e |                    z  | 1 | 2 | 3 | 4 |   `
// +---+---+---+---+                  ,    +---+---+---+---+   .
// |   | 4 | 9 | d |                .      | 5 | 6 | 7 | 8 |   o
// +---+---+---+---+ --------------------> +---+---+---+---+   '
// | 1 |   | 2 | a |             _*        | 9 | a | b | c |   .
// +---+---+---+---+        /. ,`          +---+---+---+---+   _
// | b | 8 | c | 6 | A     /==|            | d | e |   |   | B *
// +---+---+---+---+      /====.           +---+---+---+---+   `
////////0///////g///////-/======?------->///////,```````________
#define _GNU_SOURCE//     |    |               
#include <stdio.h> //     |    |                          oo  ..
//^^^^^^^^^^^^^^^^^^^^^---|_n__|________________________________
#include <stdint.h> //--3-------4-------5-------6-------7-------
#define NZR 2 // number of void squares
#define SIZ 16 /* Number of vertices         //________________*/
int coor[][NZR] = {{0, 1},{0, 2},{2, 2},{1, 0},  // oo   /\-    /`
/* you can't do */ {1, 1},{2, 0},{3, 3},{0, 0},  //    -//\\-  (
/* this things  */ {1, 3},{2, 1},{3, 2},{0, 3},  //^^^^^|n | .. `|
/* in python:)  */ {2, 3},{3, 1},{3, 0},{1, 2}}; //------*********
int *dzero[NZR] = {coor[0], coor[SIZ - 1]}; // also dhis ,.`
char *CSI = "\033["; // Control Blah Blah Blah   ... .. `
int ORIG[] = {4, 3}; // Canvas coorsys origin   i
int sqrfig[] = {-1, // square figure   /\_____________     _-, -
/*            */ 7,  3,  5, 14,  //   /  \_____________  _*   `-
/*            */ 0,  4,  9, 13,  //  /    \_____________`      -
/*            */ 1, 15,  2, 10,  //---`   `    ..      1--------
/*            */11,  8, 12,  6}; //___`_n_`____ `______1_______-
int left[]  = {-1,  0}; //  //-------\\                \ z-z   -
int right[] = { 1,  0}; // //---------\\________________\      -
int up[]    = { 0, -1}; //  | []   [] |,, ,,,,  ,,,, ,,|       -
int down[]  = { 0,  1}; //__|----n----|___i____________|__&____-
int sqrcoor[][NZR] = {{},                           // s / q / r
/*	 _	   */ {0, 0},{1, 0},{2, 0},{3, 0},  // e \ c \ o
/*     o   o       */ {0, 1},{1, 1},{2, 1},{3, 1},  // o / r / d
/*       _         */ {0, 2},{1, 2},{2, 2},{3, 2},  // i \ n \ a
/*       =         */ {0, 3},{1, 3},{2, 3},{3, 3}}; // t / e / s
/* Ve can look at the board as a graph, with each square being *
 * a vertex, and neighbours being the corresponding horizontal *
 * and vertical squares.                       //______________*/
int nbrz[] = { 2, 3, 3, 2,   // n um b  e r of  o_______________
               3, 4, 4, 3,   // ne ig h bo u_   o____...__[ ]___
               3, 4, 4, 3,   // r s       (..)  o____. .________
               2, 3, 3, 2 }; //_______m___(  )__o    . . -------
int ghood[] = {     // global hood                             /
         1,  4,     //                 _____                   /
         2,  5,  0, //                /     |\                 /
         3,  6,  1, //               /      | \                /
             7,  2, //              /       |  \               /
     0,  5,  8,     //             /        |   \              /
     1,  6,  9,  4, //            /   /|    |    \             /
     2,  7, 10,  5, //           /   /_|    |____ \            /
     3,     11,  6, //          /                |\\           /
     4,  9, 12,     //         /______      _____| \\          /
     5, 10, 13,  8, //         \      |    |\     \ \\         /
     6, 11, 14,  9, //          \     |    | \     \ \\        /
     7,     15, 10, //           \    |____|  \     \ \\       /
     8, 13,         //            \    \    \  \     \ \\      /
     9, 14,     12, //             \    \    \  \     \ \\     /
    10, 15,     13, //              \    \    \  \     \ \\    /
    11,         14  //               \    \    \  \     \ \\   /
};                  //                \    \    \  \     \ \\  /
int *hood[SIZ];     // neighborhood for\each\vertex \     \ \\ /
char *clrstr(int type, int *rgb) /*  ,  \ .  \       \ .   \ \*/
{ /*      .     `       -           .    \    \ `     \     \ */
    static char *code[] = {";38;2", ";48;2"}; /* fgr, bgr  . \*/
    char *buf = code[type]; /*   ,         \    \       \     */
    /*    .        .                      ` \    \.      \    */
    for (int j = 0; j < 3; j++) { /*         \    \     . \   */
	asprintf(&buf, "%s;%d", buf, rgb[j]); /* , \       \  */
    } /*  *           .  ,                     \    \       \ */
    return buf; /*                `  ,          \    \ `     \*/
} /*    .           `                      .*    \    \       */
char *sgrstr(int *sgr) /*    `  *                 \ x  \      */
{ /*                               .               \    \     */
    char *buf = ""; /*                        .     \    \    */
    /*                  '             `              \.   \   */
    while (*sgr != -1) { /*                           \    \  */
	asprintf(&buf, "%s;%d", buf, *sgr++); /*  ,    \    \ */
    } /*               `                    ,       .   \    \*/
    return buf; /*   .          -         .              \-   */
} /*                                                      \   */
void display(int x, int y, int *fgr, int *bgr, int *sgr, char *t)
{ /*        .      *            ,                           \ */
    char *fgrstr = clrstr(0, fgr); /*                .       \*/
    char *bgrstr = bgr ? clrstr(1, bgr) : ""; /*              */
    /*         .         '                  .    `         .  */
    printf("%s%d;%dH", CSI, y, x); /*  `                   `  */
    printf("%s%s%s%sm", CSI, fgrstr, bgrstr, sgrstr(sgr)); /* */
    printf("%s", t); /*         *                   *         */
    printf("%sm", CSI); /*       ,             .              */
} /*         *                 ,                      '       */
void draw_board(void) /*                 .                    */
 { /*             ,                                           */
    char *t1 = "+---+---+---+---+"; /*              .         */
    char *t2 = "|   |   |   |   |"; /*     *                  */
    /*          ,                                      *      */
    int fgr[] = {20, 130, 240}; /*        .                   */
    int sgr[] = {1, -1}; /*             .               .     */
    /*               .                         .              */
    int x = ORIG[0]; /*   ,                                   */
    int y = ORIG[1]; /*        . .                            */
    /*                   x                  .                 */
    int j; /*    ,                                   x        */
    for (j = 0; j < 8; j += 2) { /*              .      .     */
	display(x, y + j    , fgr, NULL, sgr, t1); /*         */
	display(x, y + j + 1, fgr, NULL, sgr, t2); /*         */
    } /*             .                                  .     */
    display(x, y + j, fgr, NULL, sgr, t1); /*                 */
} /*                    .                              .      */
int b2c(int dir, int coor) /*          .                      */
{ /*            ,                          ,                  */
    static int  scale[] = {4, 2}; /*               . *        */
    static int offset[] = {2, 1}; /*     ,                *   */
    /*   ,      .                                             */
    return ORIG[dir] + coor * scale[dir] + offset[dir]; /* ,  */
} /*.. .   .. ....  .  .    .    ... ... ..... . .  . . . .  .*/
int b2s(int *board) /* board to square [1, 16]                */
{ /*                                                  .       */
    int i = board[0]; /* board square x coor    ,             */
    int j = board[1]; /* board square y coor           .      */
    /*                      .                                 */
    return (j << 2) + i + 1; /* square name        ,          */
} /*. `. -, *    - ,  '  , .     '   .-    "     .      oo  . */
void draw_figure(int figure) /*                               */
{ /*      ,                         ,               .         */
    if (!figure || figure == SIZ - 1) return; /*              */
    int fgr[] = {80, 10, 180}; /*              .     .        */
    int sgr[] = {22, -1}; /*                                  */
    int x = b2c(0, coor[figure][0]); /*                       */
    int y = b2c(1, coor[figure][1]); /*   .                   */
    char *t; /*   .                            .    *     .   */
    /*                       .             .                  */
    asprintf(&t, "%x", figure); /*                         .  */
    display(x, y, fgr, NULL, sgr, t); /*                      */
} /* ... ..   ..     ...    ..... . ......... .........    .. */
int lock[SIZ]; /* list of locked square indexes           .   */
int lsp = 0; /* lock stack pointer               .            */
void push_lock(int idx) /*   ,           *                    */
{ /*  .  `    .       .          .                 '          */
    lock[lsp++] = idx; /*                                     */
} /*                                      .                   */
int pop_lock(void) /*          .        .               x     */
{ /*                        u-u-u-3-7-s-3-ou-ou-7-e-e-e-g-e-7-*/
    return lock[--lsp]; /*                       .            */
} /*      '                 *        .                        */
int islocked(int idx) /*                                      */
{ /*             .                                   *        */
    for (int j = 0; j < lsp; j++) { /*            ,           */
        if (idx == lock[j]) return 1; /*                   .  */
    } /*  .                                                   */
    return 0; /*     *                                        */
} /*                                                      oo  */
////////////////////////////////////////////////////////////////
#include <string.h> /*  .                                     */
#include <stdlib.h> /*        *             .           *     */
void swap(void *dst, void *src, int n) /*    .                */
{ /*                                                          */
    void *tmp = malloc(n); /*       *               .         */
    /*                                                        */
    memcpy(tmp, src, n); /*           *                       */
    memcpy(src, dst, n); /*                      *            */
    memcpy(dst, tmp, n); /*                           .       */
    /*                         .                              */
    free(tmp); /*                                             */
} /*   * *   *  ******   **** **** *  * ***  * *     ***   ** */
#define svap(PTR, I, J) /*         .                          */\
    swap((PTR) + (I), (PTR) + (J), sizeof *((PTR) + 0)) /*    */
#define NX 4  /* namba of x and y squares                     */
#define NY NX /*                              `     .         */
////////////////////////////////////////////////////////////////
void dump_sqrfig(void) /*      .                       .      */
{ /*            .                `          .        .        */
    for (int f = 1; f <= SIZ; f++) { /*                       */
	printf("%2d", sqrfig[f]); /*            .             */
	printf((f % NX)? ",": "\n"); /*                '      */
    } /*                                                      */
} /*       **  *  ***    ***** **    ******* ***** * **    ** */
void dump_coor(void) /*             .                    .    */
{ /*                                     *        .           */
    for (int s = 0; s < SIZ; s++) { /*                        */
	printf("(%2d,%2d)", coor[s][0], coor[s][1]); /*       */
	printf(((s+1) % NX)? " ": "\n"); /*                   */
    } /*                                        `             */
} /*  **** **** *  **    *    *  * ******** ***  ******      /*/
void svitch(int j, int *dst) /*       *                  .  / */
{ /*                                                       /  */
    static int dz[] = {0, SIZ - 1}; /* coor dzero indzjex /   */
    int srcsqr = b2s(dzero[j]); /*                       /    */
    int dstsqr = b2s(dst); /*               *        *  /     */
    int dstfig = sqrfig[dstsqr]; /*     .              /      */
 /*                                  `            .   /    .  */
    svap(sqrfig, srcsqr, dstsqr); /*                 /        */
    svap(coor, dz[j], dstfig); /*         `         /         */
} /*         * *    ..               .  .  *     * /          */
void erase(int *dst) /*                           /           */
{ /*                            `            .   /            */
    int x = b2c(0, dst[0]); /*        *         /     .       */
    int y = b2c(1, dst[1]); /*                 /              */
 /*                                           /               */
    printf("%s%d;%dH", CSI, y, x); /*        /      .         */
    printf("%s1X", CSI); /*                 /                 */
} /*   .            *    .    . .. *    .  /         .        */
#include <unistd.h> /*     .              / ..            .   */
void pooz(int sec) /*   .           *    /         *          */
{ /*                          x        ./               ,     */
    fflush(stdout); /*                 /         *            */
    sleep(sec); /*           .        /                       */
} /*  * .   .        *  * .      .   /     .    *           ' */
//,,////////////////////////////////////////////////////////////
void init(void) /*        .        /                          */
{ /*                 .            /        .      .     *     */
    int offset = 0; /*      .    /      .           .         */
    /*         .                /                             */
    for (int j = 0; j < SIZ; j++) { /*        .               */
	hood[j] = ghood + offset; /*              *           */
	offset += nbrz[j];   /*                          .    */
    } /*                    /        .  .      .              */
    printf("%s?25l", CSI); /* hidé cursor         .           */
    printf("%s1;1H%s2J", CSI, CSI); /*        .       x       */
    draw_board(); /*     /     .                              */
    for (int j = 1; j < SIZ - 1; j++) { /*           .        */
	draw_figure(j);/*                              .      */
    } /*   .          /          . .              .           */
} /*         .       /                             `          */
void fin(void) /*   /     *              *                 *  */
{ /*               /                                          */
    printf("%s%d;%dH", CSI, ORIG[1] + 10, 0); /*       *      */
    printf("%s?25h", CSI); /* show cursor          .          */
} /*    .       /            *                 .              */
////////////////////////////////////////////////////////////////
struct sqrnode { /*         .                 .               */
    int idx; /* square index (hood)         .            .    */
    struct sqrnode *parent; /* path search                    */
}; /*      /      .                                  .        */
#define STKSIZ 256 /* stack size    `        .                */
int recstk[STKSIZ]; /* record stack                           */
struct sqrnode *parstk[STKSIZ]; /* parent stack    `   ,   .  */
struct sqrnode *cldstk[STKSIZ]; /* child stack             oo */
////////////////////////////////////////////////////////////////
int recstk_sp = 0; /* record stack stack pointer              */
int parstk_sp = 0; /* parent stack stack pointer        `     */
int cldstk_sp = 0; /* child stack stack pointer    .          */
void push_recstk(int rec) /*  .                               */
{ /*             .                    .    *           `      */
    recstk[recstk_sp++] = rec; /*             `               */
} /*              .                                       .   */
void push_parstk(struct sqrnode *par) /*          ` ,         */
{ /*         ,                                                */
    parstk[parstk_sp++] = par; /*         .   x         *     */
} /*           .                                    .         */
void push_cldstk(struct sqrnode *cld) /*                      */
{ /*              ,                      ,                    */
    cldstk[cldstk_sp++] = cld; /*                      .      */
} /*                                    `     oo              */
////////////////////////////////////////////////////////////////
int ckrec(int rec) /*   .            .                        */
{ /*          .           .       .          .            .   */
    for (int j = 0; j < recstk_sp; j++) { /*    .             */
	if (recstk[j] == rec) return 0; /*            *       */
    } /*                   .                           .      */
    return 1; /*     .                . *                     */
} /*       .                                                  */
////////////////////////////////////////////////////////////////
struct sqrnode *cons(int idx, struct sqrnode *parent) /*      */
{ /*                    .                                     */
    struct sqrnode *node = malloc(sizeof *node); /*           */
    /*                                                .       */
    node->idx = idx; /*          .        *             .     */
    node->parent = parent; /*        .                        */
    /*              .                  .                      */
    return node; /*   .       *                  .            */
} /*                                                       .  */
////////////////////////////////////////////////////////////////
struct sqrnode *find_path(int src, int dst) /*     0 1 2 3    */
{ /*                 .    .           .            4 5 6 7    */
    int i = 0, j, k, idx, nxt; /*  .          .    8 9 a b  * */
    int *sp[] = {&parstk_sp, &cldstk_sp}; /*       c d e f    */
    void (*push_stk[])(struct sqrnode *) = { /*               */
	push_parstk, push_cldstk /*               .     .     */
    }; /*         .                 .                         */
    struct sqrnode **stk[] = {parstk, cldstk}; /*      ,      */
    /*  .               .             .                       */
    recstk_sp = 0; /*      .   *                 +    .       */
    parstk_sp = 0; /*                    .                    */
    cldstk_sp = 0; /* ,      +                   .            */
    push_recstk(src); /*         .    .  *                 .  */
    push_stk[i](cons(src, NULL)); /*               .          */
    /*          .                      .                      */
    for (int depth = 0; depth != -1; depth++) { /*    .      /*/
	for (j = 0; j < *sp[i]; j++) { /* parent loop   .   / */
	    idx = stk[i][j]->idx; /*                       /  */
	    if (idx == dst) return stk[i][j]; /*    .     /   */
	    for (k = 0; k < nbrz[idx]; k++) { /*         / .  */
		nxt = hood[idx][k]; /*                  /     */
		if (ckrec(nxt) && !islocked(nxt)) { /* /      */
		    push_recstk(nxt); /*         .    /      /*/
		    push_stk[!i](cons(nxt, stk[i][j])); /*  / */
		} /* .                 .            /      /  */
	    } /*       .          .         .      /   .  /   */
	} /*       .                           .  /      /   /*/
	*sp[i] = 0; /* clear stack        *      /      /   / */
	i = !i; /* switch            .          /      /.  /  */
    } /*          .                            /   .  /   /   */
} /*                  .                       /      /   /    */
////////////////////////////////////////////////////////////////
void get_nodes(struct sqrnode *node) /*     /      /   .      */
{ /*        .                              /   .  /   /       */
    recstk_sp = 0; /*      .         .    /      /   /        */
    while (node) { /*              .     /+     /   /         */
	push_recstk(node->idx); /*      /      /  ./          */
	node = node->parent; /*    *   /      /   /           */
    } /*      .                       /      / * /     .      */
} /*                  .              /      /   /             */
////////////////////////////////////////////////////////////////
void journey(int j, int *target) /*/      /   /               */
{ /*                              /      /   / .    .         */
    int *missile = dzero[j]; /*  /      /              .      */
    int src = b2s(missile) - 1; /*     / .                    */
    int dst = b2s(target) - 1; /*  .  /        +              */
    int fig, sqr; /*     .    /      /   /         .          */
    int *dst_coor; /*  `        .                          .  */
    /*                 +    /                       .   .     */
    pooz(1); /*    .       /      /        .                  */
    get_nodes(find_path(src, dst)); /*                        */
    for (int i = recstk_sp - 2; i > -1; i--) { /*    +        */
	sqr = recstk[i] + 1; /*    .              .           */
	fig = sqrfig[sqr]; /*         .        .              */
	dst_coor = coor[fig]; /*                              */
	erase(dst_coor); /*         +                     .   */
	svitch(j, dst_coor); /*                               */
	draw_figure(fig); /*                                  */
	pooz(1);  /*     /        .                           */
    } /*         /               .                .           */
} /*            /      /                                      */
////////////////////////////////////////////////////////////////
int *add(int *a, int *b) /*  .                                */
{ /*         /    .              .         .           .      */
    static int res[NZR]; /*                  .                */
     /*                                                       */
    for (int j = 0; j < NZR; j++) { /*                        */
	res[j] = a[j] + b[j]; /*                      .       */
    } /*                            .         +               */
    return res; /*   .                                        */
} /*                            .                             */
////////////////////////////////////////////////////////////////
int *get_next_dst(int *missile, int *target) /*               */
{ /*                                            .             */
    int mx = missile[0]; /*                                   */
    int my = missile[1]; /*           *                       */
    int tx = target[0]; /*                             .      */
    int ty = target[1]; /*       -                            */
    /*              ,        `               ..               */
    int *dst; /*  .                                           */
    int fig; /*                    .                      *   */
    /*                  "                                     */
    if (tx != mx) { /*         X            -                 */
	if (tx < mx) { /*                                     */
	    dst = add(missile, left); /*          *           */
	} else { /*                                           */
	    dst = add(missile, right); /*                ,    */
	} /*                                                  */
	if (!islocked(b2s(dst) - 1)) { /*                     */
	    return dst; /*      .                 +           */
	} /*                                                  */
    } /*                          ,                           */
    if (ty != my) { /*       .                                */
	if (ty < my) { /*                  `          .       */
	    dst = add(missile, up); /*                        */
	} else { /*                          *                */
	    dst = add(missile, down); /*                  ,   */
	} /*                .                                 */
	if (!islocked((b2s(dst) - 1))) { /*                   */
	    return dst; /*            x                       */
	} /*                                                  */
    } /*              .                         .             */
    return NULL; /*           '                               */
} /*                              ,                           */
////////////////////////////////////////////////////////////////
int *copy(int *src) { /* Copy Ninja Kakashi .   *     .   `    */
    int n = NZR * sizeof *src; /*                              */
    int *dst = malloc(n); /*           *        .              */
    memcpy(dst, src, n); /*      .                        "    */
    return dst; /*     .           `                ,          */
} /*    .       /          -               ,                   */
////////////////////////////////////////////////////////////////
void escort(int fig) /*                                       */
{ /*                                     .           .    .   */
    int *target = sqrcoor[fig]; /*                            */
    int *missile = coor[fig]; /*           .         .        */
    int *dst; /*                                              */
    /*                   .                                    */
    journey(0, target); /* put first zero at target position  */
    push_lock(fig - 1); /* lock that square                   */
    target = copy(add(target, down)); /* next target          */
    while (dst = get_next_dst(missile, target)) { /* 2nd dst  */
	push_lock(b2s(missile) - 1); /* lock temporarily      */
	journey(1, dst); /* clear path for the missile        */
	pop_lock(); /* unlock missile square                  */
	erase(missile); /*                                    */
	svitch(1, missile); /*      .                         */
	draw_figure(fig); /*                                  */
	pooz(1); /*                                           */
    } /*                                                      */
    erase(missile); /*                          +             */
    svitch(0, missile); /*                                    */
    draw_figure(fig); /*                .                     */
    pooz(1); /*                                               */
} /*                                                          */
////////////////////////////////////////////////////////////////
/* OK each figure takes 4 bits and there are 16 squares so we  *
 * can represent each position with 4x16 = 64 bits             */
struct pos { /*                                                */
    int64_t     board; /*                                      */
    int         zero[NZR]; /* no figure squares                */
    struct pos *parent; /*                                     */
}; /*                                                          */
/* initial and final positions                                 */
struct pos ipos = { 0, {0, 0}, NULL }; /*                      */
struct pos fpos = { 0x123456789abcde00, { 0, 0 }, NULL }; /*   */
#define shift(SQR) ((SIZ - 1 - (SQR)) << 2) /*                 */
//^^////////////////////////////////////////////////////////////
/* .......................... get figure from ze board at sqr */
int get(int64_t board, int sqr) /*                            */
{ /*                                                          */
    int64_t mask = 0xf; /*                                    */
    int shift = shift(sqr); /*                                */
    /*                                                        */
    mask <<= shift; /*                                        */
    /*                                                        */
    return (board & mask) >> shift; /*                        */
} /*                                                          */
////////////////////////////////////////////////////////////////
void set(int64_t *board, int sqr, int64_t figure) /*          */
{ /*                                                          */
    int64_t mask = 0xf; /*                                    */
    int shift = shift(sqr); /*                                */
    /*                                                        */
    mask <<= shift; /*                                        */
    *board &= ~mask; /*                                       */
    figure <<= shift; /*                                      */
    *board |= figure; /*                                      */
} /*                                                          */
////////////////////////////////////////////////////////////////
void convert(void) /*                                         */
{ /*                                                          */
    int fig, sqr; /*                                          */
    for (int j = 1; j <= SIZ; j++) { /*                       */
	fig = sqrfig[j]; /*                                   */
	sqr = j - 1; /*                                       */
	if (fig == 0) { /*                                    */
	    ipos.zero[0] = sqr; /*                            */
	} else if (fig == SIZ - 1) { /*                       */
	    ipos.zero[1] = sqr; /*                            */
	} else { /*                                           */
	    set(&(ipos.board), sqr, fig); /*                  */
	} /*                                                  */
    } /*                                                      */
} /*                                                          */
////////////////////////////////////////////////////////////////
void xch(int64_t *board, int src, int dst) /*                 */
{ /*                                                          */
    int fig[] = { get(*board, src), get(*board, dst) }; /*    */
    /*                                                        */
    set(board, dst, fig[0]); /*                               */
    set(board, src, fig[1]); /*                               */
} /*                                                          */
////////////////////////////////////////////////////////////////
/* infut: p   - current position                               *
          j   - zero index                                     *
          dst - destination sqr                                *
   oufut: new position                                         */
struct pos *move(struct pos *p, int j, int dst) /*             */
{ /*                                                           */
    int src = p->zero[j]; /*                                   */
    struct pos *q = malloc(sizeof *q); /*                      */
    /*                                                         */
    *q = *p; /* Copy Ninja Kakashi                             */
    q->zero[j] = dst; /*                                       */
    xch(&(q->board), src, dst); /*                             */
    /*                                                         */
    return q; /*                                               */
}////////////////////////////////////////////////////////////////
/* OK zo we basically access the stack through a head vhich    *
 * pointer is named stack pointer and it stays fixed, all the  *
 * action is at the node following it (pushing and popping)    */
//**////////////////////////////////////////////////////////////
struct stack {           //     //      //  //  //////////node//
    struct pos   *pos;   //  ---////  ////     /////////////////
    struct stack *next;  //---  ////  ////  //  ////////////////
};                       //     ////  ////  //  ////////////////
//''////////////////////////////////////////////////////////////
// record stack
struct stack ahead_rec = { NULL, NULL }; // head
struct stack *rec = &ahead_rec;          // stkptr
////////////////////////////////////////////////////////////////
int push(struct stack *stkptr, struct pos *pos) /*            */
{ /*                                                          */
    struct stack *node = malloc(sizeof *node); /*             */
    /*                                                        */
    node->pos = pos; /*                                       */
    node->next = stkptr->next; /*                             */
    stkptr->next = node; /* put node just after the head      */
} /*                                                          */
////////////////////////////////////////////////////////////////
void dump_pos(struct pos *pos) /*                             */
{ /*                                                          */
    printf("%016llx\n", pos->board); /*                       */
    printf("%2d %2d\n", pos->zero[0], pos->zero[1]); /*       */
} /*                                                          */
////////////////////////////////////////////////////////////////
void dump_node(struct stack *node) /*                         */
{ /*                                                          */
    dump_pos(node->pos); /*                                   */
} /*                                                          */
////////////////////////////////////////////////////////////////
void del(struct stack *node) /*                               */
{ /*                                                          */
    free(node); /*                                            */
} /*                                                          */
////////////////////////////////////////////////////////////////
void loop(struct stack *stkptr, void (*f)(struct stack *)) /* */
{ /*                                                          */
    struct stack *node = stkptr->next; /*                     */
    struct stack *next; /*                                    */
    /*                                                        */
    while (node) { /*                                         */
	next = node->next; /*                                 */
	f(node); /*                                           */
	node = next; /*                                       */
    } /*                                                      */
} /*                                                          */
////////////////////////////////////////////////////////////////
int find(struct stack *stkptr, struct pos *pos) /*            */
{ /*                                                          */
    struct stack *node = stkptr; /*                           */
    /*                                                        */
    while (node = node->next) /*                              */
	if (node->pos->board == pos->board) return 1; /*      */
    /*                                                        */
    return 0; /*                                              */
} /*                                                          */
////////////////////////////////////////////////////////////////
int size(struct stack *stkptr) /*                             */
{ /*                                                          */
    int s = 0; /*                                             */
    struct stack *node = stkptr; /*                           */
    /*                                                        */
    while (node = node->next) s++; /*                         */
    /*                                                        */
    return s; /*                                              */
} /*                                                          */
void clear(struct stack *stkptr) /*                           */
{ /*                                                          */
    loop(stkptr, del); /*                                     */
    /*                                                        */
    stkptr->next = NULL; /*                                   */
} /*                                                          */
////////////////////////////////////////////////////////////////
struct pos *depth_srch(void) /*                               */
{ /*                                                          */
    /* parent stack                                           */
    struct stack ahead_pare = { NULL, NULL }; /*              */
    struct stack *pare = &ahead_pare; /*                      */
    /* children stack                                         */
    struct stack ahead_chld = { NULL, NULL }; /*              */
    struct stack *chld = &ahead_chld; /*                      */
    /*                                                        */
    push(pare, &ipos); /*                                     */
    push( rec, &ipos); /*                                     */
    /*                                                        */
    struct stack *it; /* iterator                             */
    struct pos *pos, *pos1; /* pos and pos prime              */
    int i, j, k; /* what is this?                             */
    /*                                                        */
    int depth = 0; /*                                         */
    while (1) { /* loop until find                            */
	it = pare; /*                                         */
	while (it = it->next) { /* parent loop                */
	    pos = it->pos; /*                                 */
	    /* are ve done?                                   */
	    if (pos->board == fpos.board) return pos; /*      */
	    /* ck possible moves for both zeroes              */
	    for (j = 0; j < NZR; j++) { /*                    */
		k = pos->zero[j]; /* void square namba        */
		/* loop over neighbors                        */
		for (i = 0; i < nbrz[k]; i++) { /*            */
		    if (hood[k][i] < 8) continue; /*          */
		    pos1 = move(pos, j, hood[k][i]); /*       */
		    /* ck if pos1 in rec stack                */
		    if (!find(rec, pos1)) { /*                */
			pos1->parent = pos; /*                */
			push(chld, pos1); /*                  */
			push(rec, pos1); /*                   */
		    } else { /*                               */
			free(pos1); /* we don't need that     */
		    } /*                                      */
		} /*                                          */
	    } /*                                              */
	} /*                                                  */
	clear(pare); /*                                       */
	it = pare; /* switch roles                            */
	pare = chld; /*                                       */
	chld = it; /*                                         */
    } /*                                                      */
} /*                                                          */
////////////////////////////////////////////////////////////////
void thinking(void) /*                                        */
{ /*                                                          */
    struct pos *pos = depth_srch(); /*                        */
    clear(rec); /*                                            */
    while (pos) { /*                                          */
    	push(rec, pos); /*                                    */
    	pos = pos->parent; /*                                 */
    } /*                                                      */
    struct stack *node = rec->next; /*                        */
    int *last = node->pos->zero; /*                           */
    /*                                                        */
    int j, sqr, fig; /*                                       */
    int *dst; /*                                              */
    while (node = node->next) { /*                            */
	for (j = 0; j < NZR; j++) { /*                        */
	    sqr = node->pos->zero[j]; /*                      */
	    if (sqr != last[j]) { /*                          */
		dst = sqrcoor[sqr + 1]; /*                    */
		fig = sqrfig[sqr + 1]; /*                     */
		erase(dst); /*                                */
		svitch(j, dst); /*                            */
		draw_figure(fig); /*                          */
		pooz(1); /*                                   */
	    } /*                                              */
	} /*                                                  */
	last = node->pos->zero; /*                            */
    } /*                                                      */
} /*                                                          */
////////////////////////////////////////////////////////////////
int main(void) /* B4epa u3npaTux egHa ncyBHs B KoCMoCa        */
{ /*                                                          */
    init(); /*                                                */
    for (int i = 1; i <= 8; i++) { /*                         */
	escort(i); /*                                         */
    } /*                                                      */
    convert(); /*                                             */
    thinking(); /*                                            */
    fin(); /*                                                 */
    /*                                                        */
    return 0; /*                                              */
} //_-__.._*:_*"*?:::?.--_._*""*-_.":.."?..:?_::.*_:*"_*:. bugs:
// oo .. xo .o  o_o  -- ,, o- ^^ ** `` oO OO .. :: .: .- *o ,.
//_......_.___...._..__.._..__..__._..__.___...___.._._._._ log:
// - let's fight!
