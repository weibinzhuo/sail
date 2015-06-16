
#ifndef _SAIL_ATOMIC_H
#define _SAIL_ATOMIC_H

/*GCC4.1 builtin atomic operations */
#define HAVE_ATOMIC_OPS1

typedef long                atomic_int_t;
typedef unsigned long       atomic_uint_t;

typedef volatile atomic_uint_t  	atomic_t;

#define atomic_cmp_set(lock,old, set) \
__sync_bool_compare_and_swap(lock,old, set)

#define atomic_fetch_add(value,add) \
__sync_fetch_and_add(value,add)

#define atomic_add_fetch(value,add) \
__sync_add_and_fetch(value,add)

#define atomic_set(ptr,value) \
__sync_lock_test_and_set(ptr, value)

/*gccversion <= 4.2, __sync_synchronize is empty implement, which is a bug.
gccversion >= 4.4, __sync_synchronize corresponds to "mfence" the assembly instructions.
useto ensure instruction sequence
*/
#define memory_barrier()    __sync_synchronize()


#endif/*_SAIL_ATOMIC_H*/
