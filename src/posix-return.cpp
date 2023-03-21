// scatr-check: CXX-W2013
// RUN: %check_clang_tidy %s bugprone-posix-return %t

#define NULL nullptr
#define ZERO 0
#define NEGATIVE_ONE -1

typedef int pid_t;
typedef long off_t;
typedef decltype(sizeof(int)) size_t;
typedef struct __posix_spawn_file_actions* posix_spawn_file_actions_t;
typedef struct __posix_spawnattr* posix_spawnattr_t;
# define __CPU_SETSIZE 1024
# define __NCPUBITS (8 * sizeof (__cpu_mask))
typedef unsigned long int __cpu_mask;
typedef struct
{
  __cpu_mask __bits[__CPU_SETSIZE / __NCPUBITS];
} cpu_set_t;
typedef struct _opaque_pthread_t *__darwin_pthread_t;
typedef __darwin_pthread_t pthread_t;
typedef struct pthread_attr_t_ *pthread_attr_t;

extern "C" int posix_fadvise(int fd, off_t offset, off_t len, int advice);
extern "C" int posix_fallocate(int fd, off_t offset, off_t len);
extern "C" int posix_madvise(void *addr, size_t len, int advice);
extern "C" int posix_memalign(void **memptr, size_t alignment, size_t size);
extern "C" int posix_openpt(int flags);
extern "C" int posix_spawn(pid_t *pid, const char *path,
                const posix_spawn_file_actions_t *file_actions,
                const posix_spawnattr_t *attrp,
                char *const argv[], char *const envp[]);
extern "C" int posix_spawnp(pid_t *pid, const char *file,
                 const posix_spawn_file_actions_t *file_actions,
                 const posix_spawnattr_t *attrp,
                 char *const argv[], char *const envp[]);
extern "C" int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
extern "C" int pthread_attr_setaffinity_np(pthread_attr_t *attr, size_t cpusetsize, const cpu_set_t *cpuset);
extern "C" int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
extern "C" int pthread_attr_init(pthread_attr_t *attr);
extern "C" int pthread_yield(void);


void warningLessThanZero() {
  // [CXX-W2013]: 7 "The comparison always evaluates to `false` as `posix_fadvise(..)` always returns non-negative values"
  if (posix_fadvise(0, 0, 0, 0) < 0) {}
  // [CXX-W2013]
  if (posix_fallocate(0, 0, 0) < 0) {}
  // [CXX-W2013]
  if (posix_madvise(NULL, 0, 0) < 0) {}
  // [CXX-W2013]
  if (posix_memalign(NULL, 0, 0) < 0) {}
  // [CXX-W2013]
  if (posix_spawn(NULL, NULL, NULL, NULL, {NULL}, {NULL}) < 0) {}
  // [CXX-W2013]
  if (posix_spawnp(NULL, NULL, NULL, NULL, {NULL}, {NULL}) < 0) {}
  // [CXX-W2013]
  if (pthread_create(NULL, NULL, NULL, NULL) < 0) {}
  // [CXX-W2013]
  if (pthread_attr_setaffinity_np(NULL, 0, NULL) < 0) {}
  // [CXX-W2013]
  if (pthread_attr_setschedpolicy(NULL, 0) < 0) {}
  // [CXX-W2013]
  if (pthread_attr_init(NULL) < 0) {}
  // [CXX-W2013]
  if (pthread_yield() < 0) {}

}

void warningAlwaysTrue() {
  // [CXX-W2013]: 7 "The comparison always evaluates to `true` as `posix_fadvise(..)` always returns non-negative values"
  if (posix_fadvise(0, 0, 0, 0) >= 0) {}
  // [CXX-W2013]: 7 "The comparison always evaluates to `true` as `pthread_create(..)` always returns non-negative values"
  if (pthread_create(NULL, NULL, NULL, NULL) >= 0) {}
  // [CXX-W2013]: 7 "The comparison always evaluates to `true` as `pthread_attr_setaffinity_np(..)` always returns non-negative values"
  if (pthread_attr_setaffinity_np(NULL, 0, NULL) >= 0) {}
  // [CXX-W2013]
  if (pthread_attr_setschedpolicy(NULL, 0) >= 0) {}
  // [CXX-W2013]
  if (pthread_attr_init(NULL) >= 0) {}
  // [CXX-W2013]
  if (pthread_yield() >= 0) {}

}

void warningEqualsNegative() {
  // [CXX-W2013]: 7 "Function `posix_fadvise(..)` only returns non-negative values"
  if (posix_fadvise(0, 0, 0, 0) == -1) {}
  // [CXX-W2013]: 7 "Function `posix_fadvise(..)` only returns non-negative values"
  if (posix_fadvise(0, 0, 0, 0) != -1) {}
  // [CXX-W2013]: 7 "Function `posix_fadvise(..)` only returns non-negative values"
  if (posix_fadvise(0, 0, 0, 0) <= -1) {}
  // [CXX-W2013]
  if (posix_fadvise(0, 0, 0, 0) < -1) {}
  // [CXX-W2013]
  if (posix_fallocate(0, 0, 0) == -1) {}
  // [CXX-W2013]
  if (posix_madvise(NULL, 0, 0) == -1) {}
  // [CXX-W2013]
  if (posix_memalign(NULL, 0, 0) == -1) {}
  // [CXX-W2013]
  if (posix_spawn(NULL, NULL, NULL, NULL, {NULL}, {NULL}) == -1) {}
  // [CXX-W2013]
  if (posix_spawnp(NULL, NULL, NULL, NULL, {NULL}, {NULL}) == -1) {}
  // [CXX-W2013]
  if (pthread_create(NULL, NULL, NULL, NULL) == -1) {}
  // [CXX-W2013]
  if (pthread_create(NULL, NULL, NULL, NULL) != -1) {}
  // [CXX-W2013]
  if (pthread_create(NULL, NULL, NULL, NULL) <= -1) {}
  // [CXX-W2013]
  if (pthread_create(NULL, NULL, NULL, NULL) < -1) {}

}

void WarningWithMacro() {
  // [CXX-W2013]
  if (posix_fadvise(0, 0, 0, 0) < ZERO) {}
  // [CXX-W2013]
  if (posix_fadvise(0, 0, 0, 0) >= ZERO) {}
  // [CXX-W2013]
  if (posix_fadvise(0, 0, 0, 0) == NEGATIVE_ONE) {}
  // [CXX-W2013]
  if (posix_fadvise(0, 0, 0, 0) != NEGATIVE_ONE) {}
  // [CXX-W2013]
  if (posix_fadvise(0, 0, 0, 0) <= NEGATIVE_ONE) {}
  // [CXX-W2013]
  if (posix_fadvise(0, 0, 0, 0) < NEGATIVE_ONE) {}
  // [CXX-W2013]
  if (pthread_create(NULL, NULL, NULL, NULL) < ZERO) {}
  // [CXX-W2013]
  if (pthread_create(NULL, NULL, NULL, NULL) >= ZERO) {}
  // [CXX-W2013]
  if (pthread_create(NULL, NULL, NULL, NULL) == NEGATIVE_ONE) {}
  // [CXX-W2013]
  if (pthread_create(NULL, NULL, NULL, NULL) != NEGATIVE_ONE) {}
  // [CXX-W2013]
  if (pthread_create(NULL, NULL, NULL, NULL) <= NEGATIVE_ONE) {}
  // [CXX-W2013]
  if (pthread_create(NULL, NULL, NULL, NULL) < NEGATIVE_ONE) {}

}

void noWarning() {
  if (posix_openpt(0) < 0) {}
  if (posix_openpt(0) <= 0) {}
  if (posix_openpt(0) == -1) {}
  if (posix_openpt(0) != -1) {}
  if (posix_openpt(0) <= -1) {}
  if (posix_openpt(0) < -1) {}
  if (posix_fadvise(0, 0, 0, 0) <= 0) {}
  if (posix_fadvise(0, 0, 0, 0) == 1) {}
}

namespace i {
int posix_fadvise(int fd, off_t offset, off_t len, int advice);
int pthread_yield(void);

void noWarning() {
    if (posix_fadvise(0, 0, 0, 0) < 0) {}
    if (posix_fadvise(0, 0, 0, 0) >= 0) {}
    if (posix_fadvise(0, 0, 0, 0) == -1) {}
    if (posix_fadvise(0, 0, 0, 0) != -1) {}
    if (posix_fadvise(0, 0, 0, 0) <= -1) {}
    if (posix_fadvise(0, 0, 0, 0) < -1) {}
    if (pthread_yield() < 0) {}
    if (pthread_yield() >= 0) {}
    if (pthread_yield() == -1) {}
    if (pthread_yield() != -1) {}
    if (pthread_yield() <= -1) {}
    if (pthread_yield() < -1) {}
}

} // namespace i

class G {
 public:
  int posix_fadvise(int fd, off_t offset, off_t len, int advice);
  int pthread_yield(void);

  void noWarning() {
    if (posix_fadvise(0, 0, 0, 0) < 0) {}
    if (posix_fadvise(0, 0, 0, 0) >= 0) {}
    if (posix_fadvise(0, 0, 0, 0) == -1) {}
    if (posix_fadvise(0, 0, 0, 0) != -1) {}
    if (posix_fadvise(0, 0, 0, 0) <= -1) {}
    if (posix_fadvise(0, 0, 0, 0) < -1) {}
    if (pthread_yield() < 0) {}
    if (pthread_yield() >= 0) {}
    if (pthread_yield() == -1) {}
    if (pthread_yield() != -1) {}
    if (pthread_yield() <= -1) {}
    if (pthread_yield() < -1) {}
  }
};
