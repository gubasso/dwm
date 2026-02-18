/* See LICENSE file for copyright and license details. */
#ifndef VANITYGAPS_H
#define VANITYGAPS_H

/* Key binding functions */
static void defaultgaps(const Arg *arg);
static void incrgaps(const Arg *arg);
static void incrigaps(const Arg *arg);
static void incrogaps(const Arg *arg);
static void incrohgaps(const Arg *arg);
static void incrovgaps(const Arg *arg);
static void incrihgaps(const Arg *arg);
static void incrivgaps(const Arg *arg);
static void togglegaps(const Arg *arg);

/* Layouts */
static void bstack(Monitor *m);
static void bstackhoriz(Monitor *m);
static void centeredmaster(Monitor *m);
static void centeredfloatingmaster(Monitor *m);
static void deck(Monitor *m);
static void dwindle(Monitor *m);
static void gaplessgrid(Monitor *m);
static void grid(Monitor *m);
static void horizgrid(Monitor *m);
static void nrowgrid(Monitor *m);
static void spiral(Monitor *m);
static void tile(Monitor *m);

#endif /* VANITYGAPS_H */
