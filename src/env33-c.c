// scatr-check: CXX-A1002

typedef struct FILE {} FILE;

extern int system(const char *);
extern FILE *popen(const char *, const char *);
extern FILE *_popen(const char *, const char *);

void f(void) {
  // It is permissible to check for the presence of a command processor.
  system(0);

  // [CXX-A1002]: "`system` uses a the command processor to execute a command"
  system("test");

  // [CXX-A1002]: "`popen` uses a the command processor to execute a command"
  popen("test", "test");

  // [CXX-A1002]: "`_popen` uses a the command processor to execute a command"
  _popen("test", "test");
}
