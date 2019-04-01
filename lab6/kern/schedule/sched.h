#ifndef __KERN_SCHEDULE_SCHED_H__
#define __KERN_SCHEDULE_SCHED_H__

#include <defs.h>
#include <list.h>
#include <skew_heap.h>

#define MAX_TIME_SLICE 5

struct proc_struct;

struct run_queue;

// The introduction of scheduling classes is borrrowed from Linux, and makes the
// core scheduler quite extensible. These classes (the scheduler modules)
// encapsulate the scheduling policies.
struct sched_class {
  // the name of sched_class
  const char *name;
  // Init the run queue
  void (*init)(struct run_queue *rq);
  // put the proc into runqueue, and this function must be called with rq_lock
  void (*enqueue)(struct run_queue *rq, struct proc_struct *proc);
  // get the proc out runqueue, and this function must be called with rq_lock
  void (*dequeue)(struct run_queue *rq, struct proc_struct *proc);
  // choose the next runnable task
  struct proc_struct *(*pick_next)(struct run_queue *rq);
  // dealer of the time-tick
  void (*proc_tick)(struct run_queue *rq, struct proc_struct *proc);
  /* for SMP support in the future
   *  load_balance
   *     void (*load_balance)(struct rq* rq);
   *  get some proc from this rq, used in load_balance,
   *  return value is the num of gotten proc
   *  int (*get_proc)(struct rq* rq, struct proc* procs_moved[]);
   */
};

struct run_queue {
  list_entry_t run_list;
  unsigned int proc_num;
  int max_time_slice;
  // For LAB6 ONLY
  skew_heap_entry_t *lab6_run_pool;
  // 在使用优先队列的实现中表示当前优先队列的头元素，如果优先队列为空，则其指向空指针（NULL）。
  // 之前使用的是run_list 来管理，当使用优先队列之后，所以需要使用新的指针的来位置
};

void sched_init(void);
void wakeup_proc(struct proc_struct *proc);
void schedule(void);
void sched_class_proc_tick(struct proc_struct *proc);

#endif /* !__KERN_SCHEDULE_SCHED_H__ */
