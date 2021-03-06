// PR c/25682
// { dg-do compile }
// Test whether we don't ICE on questionable constructs where offsetof
// should have been used instead.

struct S
{
  char a[4];
  int b;
};

extern void bar (char *, char *);

void
foo (void)
{
  char g[(char *) &((struct S *) 0)->b - (char *) 0]; // { dg-error "constant" }
  char h[(__SIZE_TYPE__) &((struct S *) 8)->b];	      // { dg-error "constant" }
  bar (g, h);
}
